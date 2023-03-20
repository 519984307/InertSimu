#pragma execution_character_set("utf-8")

#include "SphDockWidget.h"
#include "ui_SphDockWidget.h"
#include "QDebug"
#include "FileTools.h"
#include "QRegExpValidator"
#include "QRegExp"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <pqLoadDataReaction.h>
#include <pqDeleteReaction.h>
#include <pqAutoApplyReaction.h>
#include <pqShowHideAllReaction.h>
#include <pqDataRepresentation.h>
#include <pqPipelineSource.h>
#include <pqActiveObjects.h>
#include <vtkSMPropertyHelper.h>
//#include <vtkSMRenderViewProxy.h>
#include <io.h>
#include <iostream>

class SphDockWidget::pqInternals
{
public:
  Ui::SphDockWidget Ui;

  unsigned long ObserverId;

  pqInternals(SphDockWidget* self)
    : ObserverId(0)
  {
    this->Ui.setupUi(self);
  }

  ~pqInternals() = default;
};

SphDockWidget::SphDockWidget(QWidget *parent)
    : Superclass(parent)
    , Internals(new SphDockWidget::pqInternals(this))
{
    //_ASSERTE(_CrtCheckMemory());

    pqSources = new PQSources();

    this->widgetConstraint();
    this->widgetRegExpValidat();

    this->pathConfig = new PathConfig();
    this->pathConfig->init();

    sphThread = new SphThread();
    connect(sphThread, SIGNAL(threadSig_Text(QString)), this, SLOT(showText(QString)));
    connect(sphThread, SIGNAL(threadSig_State(QString)), this, SLOT(showState(QString)));
    connect(sphThread, SIGNAL(threadSig_Progress(int)), this, SLOT(showProgress(int)));
    connect(sphThread, SIGNAL(threadSig_Endtime(QString)), this, SLOT(showEndtime(QString)));

    qRegisterMetaType<StateType>("StateType");
    connect(sphThread, SIGNAL(threadSig_TaskStateChange(StateType)), this, SLOT(sphStateChange(StateType)));

    this->btnEvent();
}

void SphDockWidget::showParam(){
    Ui::SphDockWidget ui = this->Internals->Ui;


    {
        SimulationParam *sim = this->sphParam->simulationParam;

        ui.simulation_dp->setText(QString::number(sim->getDp()));
        ui.simulation_timemax->setText(QString::number(sim->getTimeMax()));
        ui.simulation_timeout->setText(QString::number(sim->getTimeOut()));

        ui.simulation_g_x->setText(QString::number(sim->getGravity().x));
        ui.simulation_g_y->setText(QString::number(sim->getGravity().y));
        ui.simulation_g_z->setText(QString::number(sim->getGravity().z));

        ui.simulation_pointmin_x->setText(QString::number(sim->getPointMin().x));
        ui.simulation_pointmin_y->setText(QString::number(sim->getPointMin().y));
        ui.simulation_pointmin_z->setText(QString::number(sim->getPointMin().z));

        ui.simulation_pointmax_x->setText(QString::number(sim->getPointMax().x));
        ui.simulation_pointmax_y->setText(QString::number(sim->getPointMax().y));
        ui.simulation_pointmax_z->setText(QString::number(sim->getPointMax().z));
    }

    {
        // only one model
        Model *model = this->sphParam->mainList->getModelList().at(0);

        ui.simulation_stl_path->setText(model->getFilePath());
        //ui.simulation_stl_angx->setText(QString::number(sim->getModelAngle().x));
        ui.simulation_stl_angy->setText(QString::number(model->getRotate().y));
        //ui.simulation_stl_angz->setText(QString::number(sim->getModelAngle().z));
    }

    {
        FillBox *fuelbox = this->sphParam->mainList->getFillBoxList().at(0);

        double size_z = fuelbox->getBoxsize().z;
        if(size_z==0.8)
            ui.multiphase_fuel_comboBox->setCurrentText("50%");
        else if(size_z==1.0)
            ui.multiphase_fuel_comboBox->setCurrentText("55%");
        else if(size_z==1.2)
            ui.multiphase_fuel_comboBox->setCurrentText("67%");
        else if(size_z==1.35)
            ui.multiphase_fuel_comboBox->setCurrentText("75%");
    }

    {
        Air *air = this->sphParam->airProperty;
        ui.multiphase_air_rhop->setText(QString::number(air->getRhop()));
        ui.multiphase_air_cs->setText(QString::number(air->getCs()));
        ui.multiphase_air_gamma->setText(QString::number(air->getGamma()));
        ui.multiphase_air_oxygen->setText(QString::number(air->getOxygenPercentage()));

        Fuel *fuel = this->sphParam->fuelProperty;
        ui.multiphase_fuel_rhop->setText(QString::number(fuel->getRhop()));
        ui.multiphase_fuel_cs->setText(QString::number(fuel->getCs()));
        ui.multiphase_fuel_gamma->setText(QString::number(fuel->getGamma()));
//        ui.multiphase_fuel_volume->setText(QString::number(fuel->getFuelVolume()));

        InertGas *inert = this->sphParam->inertGasProperty;
        ui.multiphase_inert_rhop->setText(QString::number(inert->getRhop()));
        ui.multiphase_inert_cs->setText(QString::number(inert->getCs()));
        ui.multiphase_inert_gamma->setText(QString::number(inert->getGamma()));
        ui.multiphase_inert_oxygen->setText(QString::number(inert->getOxygenPercentage()));
    }


    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置表格内容不可编辑
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    for(int i=0; i<this->sphParam->inoutList->getList().size(); i++){
        InoutZone* iz = this->sphParam->inoutList->getList().at(i);
        int Col = 0;
        int RowCont = ui.tableWidget->rowCount();
        ui.tableWidget->insertRow(RowCont);

        ui.tableWidget->setItem(RowCont, Col++, new QTableWidgetItem("ID: " + QString::number(iz->getId())));
        ui.tableWidget->item(RowCont,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui.tableWidget->setItem(RowCont, Col++, new QTableWidgetItem(iz->getName()));
        //ui.tableWidget->item(RowCont,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        {
            QPushButton *btConfig = new QPushButton("参数编辑");
            btConfig->setMaximumWidth(80);
            // 弹出出入口面板
            QObject::connect(btConfig, &QPushButton::clicked, this,[=](){
                inoutSetting = new InoutSetting(this, iz);
                inoutSetting->show();
            });

            QWidget *widget = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout;
            layout->setSpacing(0);
            layout->setMargin(0);
            layout->addWidget(btConfig);
            widget->setLayout(layout);
            ui.tableWidget->setCellWidget(RowCont, Col++, widget);
        }

        {
            QPushButton *btDel = new QPushButton("删除");
            btDel->setEnabled(false);
            btDel->setMaximumWidth(80);
            QWidget *widget = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout;
            layout->setSpacing(0);
            layout->setMargin(0);
            layout->addWidget(btDel);
            widget->setLayout(layout);
            ui.tableWidget->setCellWidget(RowCont, Col++, widget);
        }
    }
}

void SphDockWidget::saveParam(){
    Ui::SphDockWidget ui = this->Internals->Ui;

    {
        SimulationParam *sim = this->sphParam->simulationParam;

        sim->setDp(ui.simulation_dp->text().toDouble());
        sim->setTimeMax(ui.simulation_timemax->text().toDouble());
        sim->setTimeOut(ui.simulation_timeout->text().toDouble());

        Double3 gravity;
        gravity.x = ui.simulation_g_x->text().toDouble();
        gravity.y = ui.simulation_g_y->text().toDouble();
        gravity.z = ui.simulation_g_z->text().toDouble();
        sim->setGravity(gravity);

        Double3 pointmin;
        pointmin.x = ui.simulation_pointmin_x->text().toDouble();
        pointmin.y = ui.simulation_pointmin_y->text().toDouble();
        pointmin.z = ui.simulation_pointmin_z->text().toDouble();
        sim->setPointMin(pointmin);

        Double3 pointmax;
        pointmax.x = ui.simulation_pointmax_x->text().toDouble();
        pointmax.y = ui.simulation_pointmax_y->text().toDouble();
        pointmax.z = ui.simulation_pointmax_z->text().toDouble();
        sim->setPointMax(pointmax);
    }

    {
        Model *model = this->sphParam->mainList->getModelList().at(0);

        Int3 rotate;
        rotate.x = 0; rotate.z = 0;
        //rotate.x = ui.simulation_stl_angx->text().toInt();
        rotate.y = ui.simulation_stl_angy->text().toInt();
        //rotate.z = ui.simulation_stl_angz->text().toInt();
        model->setRotate(rotate);
    }

    {
        FillBox *fuelbox = this->sphParam->mainList->getFillBoxList().at(0);
        FillBox *airbox = this->sphParam->mainList->getFillBoxList().at(1);

        Double3 fuelsize = fuelbox->getBoxsize();
        Double3 airsize = airbox->getBoxsize();

        // Temporary set.
        // z = modelHeight(1.6) while model lay flatly.
        // z > modelHeight(1.6) while model lay at an angle.
        int index = ui.multiphase_fuel_comboBox->currentIndex();
        switch (index) {
        case 0:
                fuelsize.z = 0.8;
                airsize.z = 1.0;
            break;
        case 1:
                fuelsize.z = 1.0;
                airsize.z = 0.8;
            break;
        case 2:
                fuelsize.z = 1.2;
                airsize.z = 0.6;
            break;
        case 3:
                fuelsize.z = 1.35;
                airsize.z = 0.5;
            break;
        default:
            break;
        }

        fuelbox->setBoxsize(fuelsize);
        airbox->setBoxsize(airsize);
    }

    {
        Air *air = this->sphParam->airProperty;
        QString air_rhop = ui.multiphase_air_rhop->text();
        air->setRhop(air_rhop.toDouble());
        QString air_cs = ui.multiphase_air_cs->text();
        air->setCs(air_cs.toDouble());
        QString air_gamma = ui.multiphase_air_gamma->text();
        air->setGamma(air_gamma.toDouble());
        QString air_oxygen = ui.multiphase_air_oxygen->text();
        air->setOxygenPercentage(air_oxygen.toDouble());

        Fuel *fuel = this->sphParam->fuelProperty;
        fuel->setRhop(ui.multiphase_fuel_rhop->text().toDouble());
        fuel->setCs(ui.multiphase_fuel_cs->text().toDouble());
        fuel->setGamma(ui.multiphase_fuel_gamma->text().toDouble());
//        fuel->setFuelVolume(ui.multiphase_fuel_volume->text().toDouble());

        InertGas *inert = this->sphParam->inertGasProperty;
        inert->setRhop(ui.multiphase_inert_rhop->text().toDouble());
        inert->setCs(ui.multiphase_inert_cs->text().toDouble());
        inert->setGamma(ui.multiphase_inert_gamma->text().toDouble());
        inert->setOxygenPercentage(ui.multiphase_inert_oxygen->text().toDouble());
    }

    this->sphParam->SaveXml(this->pathConfig->getXmlSaveParh());
}

void SphDockWidget::btnEvent(){

    // 导入XML配置
    this->sphParam = new SphParameters();
    QObject::connect(this->Internals->Ui.btn_imp_config, &QPushButton::clicked, this,[&](){
        this->sphParam->LoadXml(this->pathConfig->getXmlParh());
        // this->sphParam->VisualALLProperties();
        this->showParam();
    });

    // 导出XML配置
    QObject::connect(this->Internals->Ui.btn_save_config, &QPushButton::clicked, this,[&](){
        this->saveParam();
        // this->sphParam->VisualALLProperties();
    });

    // 一键VTK
    QObject::connect(this->Internals->Ui.btn_imp_vtk, &QPushButton::clicked, this,[&](){
        QString basePath = this->pathConfig->getSphOutPath() + "/surface";
        QString preFileName = "Surface_";
        QString suffix = "vtk";

        QList<QStringList> files = FileTools::getLoadMultiDataPath(basePath, preFileName, suffix);
        QVector<pqPipelineSource *> Qvtkpointer;
        if(files.size() > 0) {
            Qvtkpointer = pqLoadDataReaction::loadFilesForSupportedTypes(files);  // 直接打开文件内容到渲染窗口
        }
    });

    // QVector<pqPipelineSource *> should be defined out of this connect and delete object from paraview before next choose.
    QObject::connect(this->Internals->Ui.combo_imp_vtk, QOverload<int>::of(&QComboBox::currentIndexChanged), this,[&](int index){
        pqDeleteReaction::deleteAll();
        pqAutoApplyReaction::setAutoApply(true);

        pqSources->init("Tank_config" + QString::number(index), this->pathConfig->getTankconfig());
        //pqSources->insertModel("model");
        bool res = pqSources->insertAll("model", "fuelSuf", "fuelPart", "airPart", "inertPart");
        if(res) {
            qDebug() << "insert success";
            pqPipelineSource * Qvtkpointer_model = pqSources->getSource("model").at(0);
            pqDataRepresentation* representation0 = Qvtkpointer_model->getRepresentation(Qvtkpointer_model->getViews().at(0));
            // Property name : \pv\Remoting\Views\Resources\views_and_representations.xml
            vtkSMPropertyHelper(representation0->getProxy(), "Opacity").Set(0.2);

            pqPipelineSource * Qvtkpointer_fuelSur = pqSources->getSource("fuelSuf").at(0);
            pqDataRepresentation* representation1 = Qvtkpointer_fuelSur->getRepresentation(Qvtkpointer_fuelSur->getViews().at(0));
            vtkSMPropertyHelper(representation1->getProxy(), "Opacity").Set(0.4);

            // set(*double, count)
            // double doubleArray[3]={0.5,0.5,0.5};
            // double *position = doubleArray;
            // vtkSMPropertyHelper(representation0->getProxy(), "Position").Set(position, 3);

            // or set(index, value)
            // vtkSMPropertyHelper(representation0->getProxy(), "Position").Set(0, 0.5);
            // vtkSMPropertyHelper(representation0->getProxy(), "Position").Set(1, 0.5);
            // vtkSMPropertyHelper(representation0->getProxy(), "Position").Set(2, 0.5);

            // vtkSMPropertyHelper(representation0->getProxy(), "Scale").Set(0, 0.5);
            // vtkSMPropertyHelper(representation0->getProxy(), "Scale").Set(1, 0.5);
            // vtkSMPropertyHelper(representation0->getProxy(), "Scale").Set(2, 0.5);

            // vtkSMPropertyHelper(representation0->getProxy(), "Orientation").Set(0, 60);
            // vtkSMPropertyHelper(representation0->getProxy(), "Orientation").Set(1, 60);
            // vtkSMPropertyHelper(representation0->getProxy(), "Orientation").Set(2, 0);

        }
        pqShowHideAllReaction::act(pqShowHideAllReaction::ActionType::Show);
    });

    // 导入模型
    QObject::connect(this->Internals->Ui.btn_simulation_stl, &QPushButton::clicked, this,[&](){
        QString modelPath = this->on_OpenModelFilePushButton_clicked();
        this->Internals->Ui.simulation_stl_path->setText(modelPath);
    });

    // SPH 运行
    QObject::connect(this->Internals->Ui.btn_sphtask_start, &QPushButton::clicked, this,[&](){
        QString workPath = this->pathConfig->getSphWorkPath();
        QString batName = this->pathConfig->getSphBatName();

        this->Internals->Ui.sphtask_plainTextEdit->clear();

        // 设置bat路径
        (*sphThread).setParameters(workPath, batName);
        (*sphThread).start();
    });

    // 弹出任务操作选择面板
    QObject::connect(this, &SphDockWidget::showTaskOperation, this, [&](){
        taskOperation = new TaskOperation(this);
        QObject::connect(taskOperation, SIGNAL(operation(int)), this, SLOT(sphOperation(int)));
        taskOperation->show();
    });


    // SPH 中止
    QObject::connect(this->Internals->Ui.btn_sphtask_end, &QPushButton::clicked, this,[&](){
        (*sphThread).sphAbort();
        this->Internals->Ui.btn_sphtask_end->setEnabled(false);
        this->Internals->Ui.btn_sphtask_start->setEnabled(true);
        this->Internals->Ui.sphtask_plainTextEdit->clear();
    });
}

void SphDockWidget::sphOperation(int choose){
    if(choose == 1){// SPH 删除结果并重新开始
        // 开始后 可中止
        this->Internals->Ui.btn_sphtask_start->setEnabled(false);
        this->Internals->Ui.btn_sphtask_end->setEnabled(true);
        (*sphThread).sphContinue();
    }
    if(choose == 2){// SPH 中止
        (*sphThread).sphAbort();
        // 中止后需再次点击 开始仿真
        this->Internals->Ui.btn_sphtask_start->setEnabled(true);
        this->Internals->Ui.btn_sphtask_end->setEnabled(false);
        this->Internals->Ui.sphtask_plainTextEdit->clear();
    }
    if(choose == 3){// SPH 只进行后处理
        // 后处理必须等待完成
        this->Internals->Ui.btn_sphtask_start->setEnabled(false);
        this->Internals->Ui.btn_sphtask_end->setEnabled(false);
        (*sphThread).sphPostProcess();
    }
}

void SphDockWidget::sphStateChange(StateType state){
    if(state == state_noWork){
    }
    else if(state == state_waitSignal){
        emit showTaskOperation();
    }
    else if(state == state_init){
    }
    else if(state == state_compute){
    }
    else if(state == state_postProcess){
    }
    else if(state == state_finish){
        this->Internals->Ui.btn_sphtask_start->setEnabled(true);
        this->Internals->Ui.btn_sphtask_end->setEnabled(false);

        // Prompt that the task has been completed
        // Show the result in render view.
    }
    else if(state == state_exception){
    }
}

void SphDockWidget::widgetConstraint(){
    Ui::SphDockWidget ui = this->Internals->Ui;

    ui.simulation_stl_path->setEnabled(false);
    ui.simulation_stl_angy->setEnabled(false);

    connect(this->Internals->Ui.sphtask_plainTextEdit, SIGNAL(textChanged()), this, SLOT(moveToEnd()));
}

void SphDockWidget::widgetRegExpValidat(){
    Ui::SphDockWidget ui = this->Internals->Ui;

    // RegExpValidator worked but didn't pass the test
    QRegExpValidator *reg1p000 = new QRegExpValidator(QRegExp("^-?(1|0(\\.\\d{0,3})?)$"));
    QRegExpValidator *reg10p00 = new QRegExpValidator(QRegExp("^(10|d(\\.\\d{0,2})?)$"));
    QRegExpValidator *regne100p0 = new QRegExpValidator(QRegExp("^-?(100|[1-9]?\\d(\\.\\d{0,1})?)$"));
    QRegExpValidator *reg100p0 = new QRegExpValidator(QRegExp("^((100)|[1-9]?\\d(\\.\\d{0,1})?)$"));
    QRegExpValidator *reg100 = new QRegExpValidator(QRegExp("^-?(100|[1-9]?\\d)$"));
    QRegExpValidator *reg10000 = new QRegExpValidator(QRegExp("^(10000|[1-9]\\d{0,3}|0)$"));
    {
        ui.simulation_dp->setValidator(reg1p000);

        ui.simulation_timemax->setValidator(reg10000);
        ui.simulation_timeout->setValidator(reg10p00);

        ui.simulation_g_x->setValidator(regne100p0);
        ui.simulation_g_y->setValidator(regne100p0);
        ui.simulation_g_z->setValidator(regne100p0);

        ui.simulation_pointmin_x->setValidator(regne100p0);
        ui.simulation_pointmin_y->setValidator(regne100p0);
        ui.simulation_pointmin_z->setValidator(regne100p0);

        ui.simulation_pointmax_x->setValidator(regne100p0);
        ui.simulation_pointmax_y->setValidator(regne100p0);
        ui.simulation_pointmax_z->setValidator(regne100p0);
    }

    {
        ui.multiphase_air_rhop->setValidator(reg10000);
        ui.multiphase_air_cs->setValidator(reg10000);
        ui.multiphase_air_gamma->setValidator(reg100p0);
        ui.multiphase_air_oxygen->setValidator(reg100);

        ui.multiphase_fuel_rhop->setValidator(reg10000);
        ui.multiphase_fuel_cs->setValidator(reg10000);
        ui.multiphase_fuel_gamma->setValidator(reg100p0);
        //ui.multiphase_fuel_volume->setValidator(reg100);

        ui.multiphase_inert_rhop->setValidator(reg10000);
        ui.multiphase_inert_cs->setValidator(reg10000);
        ui.multiphase_inert_gamma->setValidator(reg100p0);
        ui.multiphase_inert_oxygen->setValidator(reg100);
    }
}

QString SphDockWidget::on_OpenModelFilePushButton_clicked()
{
    //文件夹路径
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "选择模型文件","/","模型文件 (*.stl *.vtk);; 所有文件 (*.*);; ");

    if(!filePath.isEmpty()) {
        qDebug() << "path=" << filePath;
    }
    return filePath;
}

QString SphDockWidget::getpath(){
    QString path=this->pathConfig->getSphOutPath();
    return path;
}

// textBrowser追加显示文本
void SphDockWidget::showText(QString outstr) {
    this->Internals->Ui.sphtask_plainTextEdit->appendPlainText(outstr);
}

// labelState显示 任务状态
void SphDockWidget::showState(QString state) {
    this->Internals->Ui.sphtask_state->setText(state);
}

// progressBar显示 进度
void SphDockWidget::showProgress(int progress) {
    this->Internals->Ui.sphtask_progressBar->setValue(progress);
}

// labelEstimatedEndtime显示 预估结束时间
void SphDockWidget::showEndtime(QString endtime){
    this->Internals->Ui.sphtask_endtime->setText(endtime);
}

void SphDockWidget::moveToEnd(){
    this->Internals->Ui.sphtask_plainTextEdit->moveCursor(QTextCursor::End);
}

SphDockWidget::~SphDockWidget()
{
    delete this->Internals;
    this->Internals = nullptr;
}
