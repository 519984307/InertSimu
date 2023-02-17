#pragma execution_character_set("utf-8")
#include "SphDockWidget.h"
#include "ui_SphDockWidget.h"
#include "QDebug"
#include "FileTools.h"
#include "QRegExpValidator"
#include "QRegExp"

#include <pqLoadDataReaction.h>
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
    this->pathConfig = new PathConfig();
    this->pathConfig->init();

    this->widgetConstraint();

    // 导入配置
    this->sphParam = new SphParameters();
    QObject::connect(this->Internals->Ui.btn_imp_config, &QPushButton::clicked, this,[&](){
        emit this->showParam();
    });

    // 导出配置
    QObject::connect(this->Internals->Ui.btn_save_config, &QPushButton::clicked, this,[&](){
        emit this->saveParam();
    });

    // 一键VTK
    QObject::connect(this->Internals->Ui.btn_imp_vtk, &QPushButton::clicked, this,[=](){
        QString basePath = this->pathConfig->getSphOutPath() + "/surface";
        QString preFileName = "Surface_";
        QString suffix = "vtk";

        QList<QStringList> files = FileTools::getLoadMultiDataPath(basePath, preFileName, suffix);
        if(files.size() > 0)
            pqLoadDataReaction::loadFilesForSupportedTypes(files);  // 直接打开文件内容到渲染窗口
    });


    // 暂时弹出出入口面板
    QObject::connect(this->Internals->Ui.inout_btn_set, &QPushButton::clicked, this,[=](){
        inoutSetting = new InoutSetting(this);
        inoutSetting->show();
    });

    sphThread = new SphThread();

    connect(sphThread, SIGNAL(threadSig_Text(QString)), this, SLOT(showText(QString)));
    connect(sphThread, SIGNAL(threadSig_State(QString)), this, SLOT(showState(QString)));
    connect(sphThread, SIGNAL(threadSig_Progress(int)), this, SLOT(showProgress(int)));
    connect(sphThread, SIGNAL(threadSig_Endtime(QString)), this, SLOT(showEndtime(QString)));


    // SPH重新开始
    QObject::connect(this->Internals->Ui.sphtask_btn_restart, &QPushButton::clicked, this,[=](){
        (*sphThread).sphContinue();
    });

    // 后处理
    QObject::connect(this->Internals->Ui.sphtask_btn_post, &QPushButton::clicked, this,[=](){
        (*sphThread).sphPostProcess();
    });

    // 运行SPH
    QObject::connect(this->Internals->Ui.sphtask_btn_start, &QPushButton::clicked, this,[=](){
        QString workPath = this->pathConfig->getSphWorkPath();
        QString batName = this->pathConfig->getSphBatName();

        // 设置bat路径
        (*sphThread).setParameters(workPath, batName);
        (*sphThread).start();
    });
}

void SphDockWidget::showParam(){

    this->sphParam->LoadXml(this->pathConfig->getXmlParh());
    //this->sphParam->VisualALLProperties();

    {
        QString dp = QString("%4").arg(this->sphParam->simulationParam->getDp());
        this->Internals->Ui.simulation_dp->setText(dp);

        QString timemax = QString("%4").arg(this->sphParam->simulationParam->getTimeMax());
        this->Internals->Ui.simulation_timemax->setText(timemax);
        QString timeout = QString("%4").arg(this->sphParam->simulationParam->getTimeOut());
        this->Internals->Ui.simulation_timeout->setText(timeout);

        QString g_x = QString("%4").arg(this->sphParam->simulationParam->getGravity().x);
        this->Internals->Ui.simulation_g_x->setText(g_x);
        QString g_y = QString("%4").arg(this->sphParam->simulationParam->getGravity().y);
        this->Internals->Ui.simulation_g_y->setText(g_y);
        QString g_z = QString("%4").arg(this->sphParam->simulationParam->getGravity().z);
        this->Internals->Ui.simulation_g_z->setText(g_z);

        QString pointmin_x = QString("%4").arg(this->sphParam->simulationParam->getPointMin().x);
        this->Internals->Ui.simulation_pointmin_x->setText(pointmin_x);
        QString pointmin_y = QString("%4").arg(this->sphParam->simulationParam->getPointMin().y);
        this->Internals->Ui.simulation_pointmin_y->setText(pointmin_y);
        QString pointmin_z = QString("%4").arg(this->sphParam->simulationParam->getPointMin().z);
        this->Internals->Ui.simulation_pointmin_z->setText(pointmin_z);

        QString pointmax_x = QString("%4").arg(this->sphParam->simulationParam->getPointMax().x);
        this->Internals->Ui.simulation_pointmax_x->setText(pointmax_x);
        QString pointmax_y = QString("%4").arg(this->sphParam->simulationParam->getPointMax().y);
        this->Internals->Ui.simulation_pointmax_y->setText(pointmax_y);
        QString pointmax_z = QString("%4").arg(this->sphParam->simulationParam->getPointMax().z);
        this->Internals->Ui.simulation_pointmax_z->setText(pointmax_z);
    }


    {
        QString air_rhop = QString("%4").arg(this->sphParam->airProperty->getRhop());
        this->Internals->Ui.multiphase_air_rhop->setText(air_rhop);
        QString air_cs = QString("%4").arg(this->sphParam->airProperty->getCs());
        this->Internals->Ui.multiphase_air_cs->setText(air_cs);
        QString air_gamma = QString("%4").arg(this->sphParam->airProperty->getGamma());
        this->Internals->Ui.multiphase_air_gamma->setText(air_gamma);
        QString air_oxygen = QString("%4").arg(this->sphParam->airProperty->getOxygenPercentage());
        this->Internals->Ui.multiphase_air_oxygen->setText(air_oxygen);

        QString fuel_rhop = QString("%4").arg(this->sphParam->fuelProperty->getRhop());
        this->Internals->Ui.multiphase_fuel_rhop->setText(fuel_rhop);
        QString fuel_cs = QString("%4").arg(this->sphParam->fuelProperty->getCs());
        this->Internals->Ui.multiphase_fuel_cs->setText(fuel_cs);
        QString fuel_gamma = QString("%4").arg(this->sphParam->fuelProperty->getGamma());
        this->Internals->Ui.multiphase_fuel_gamma->setText(fuel_gamma);
        QString fuel_volume = QString("%4").arg(this->sphParam->fuelProperty->getFuelVolume());
        this->Internals->Ui.multiphase_fuel_volume->setText(fuel_volume);

        QString inert_rhop = QString("%4").arg(this->sphParam->interGasProperty->getRhop());
        this->Internals->Ui.multiphase_inert_rhop->setText(inert_rhop);
        QString inert_cs = QString("%4").arg(this->sphParam->interGasProperty->getCs());
        this->Internals->Ui.multiphase_inert_cs->setText(inert_cs);
        QString inert_gamma = QString("%4").arg(this->sphParam->interGasProperty->getGamma());
        this->Internals->Ui.multiphase_inert_gamma->setText(inert_gamma);
        QString inert_oxygen = QString("%4").arg(this->sphParam->interGasProperty->getOxygenPercentage());
        this->Internals->Ui.multiphase_inert_oxygen->setText(inert_oxygen);
    }

    /**
    {
        for(int i=0; i<this->sphParam->inoutList->getList().size(); i++){
            InoutZone* iz = this->sphParam->inoutList->getList().at(i);
            QString id = QString("%2").arg(iz->getId());
            QString name = QString("%10").arg(iz->getName());
            QString layers = QString("%1").arg(iz->getLayers());
            QString inputTreatment = QString("%1").arg(iz->getInputTreatment());
            QString inoutPhase = QString("%2").arg(iz->getInoutPhase());
            QString radius = QString("%2").arg(iz->getCircle().radius);
            QString direction_x = QString("%4").arg(iz->getCircle().direction.x);
            QString direction_y = QString("%4").arg(iz->getCircle().direction.y);
            QString direction_z = QString("%4").arg(iz->getCircle().direction.z);

            QString point_x = QString("%4").arg(iz->getCircle().point.x);
            QString point_y = QString("%4").arg(iz->getCircle().point.y);
            QString point_z = QString("%4").arg(iz->getCircle().point.z);

            QString angle = QString("%2").arg(iz->getCircle().rotateAxis.angle);
            QString point1_x = QString("%4").arg(iz->getCircle().rotateAxis.point1.x);
            QString point1_y = QString("%4").arg(iz->getCircle().rotateAxis.point1.y);
            QString point1_z = QString("%4").arg(iz->getCircle().rotateAxis.point1.z);

            QString point2_x = QString("%4").arg(iz->getCircle().rotateAxis.point2.x);
            QString point2_y = QString("%4").arg(iz->getCircle().rotateAxis.point2.y);
            QString point2_z = QString("%4").arg(iz->getCircle().rotateAxis.point2.z);
        }
    }
    **/

}

void SphDockWidget::saveParam(){
    {
        QString dp = this->Internals->Ui.simulation_dp->text();
        this->sphParam->simulationParam->setDp(dp.toDouble());

        QString timemax = this->Internals->Ui.simulation_timemax->text();
        this->sphParam->simulationParam->setTimeMax(timemax.toDouble());
        QString timeout = this->Internals->Ui.simulation_timeout->text();
        this->sphParam->simulationParam->setTimeOut(timeout.toDouble());


        Double3 gravity;
        gravity.x = this->Internals->Ui.simulation_g_x->text().toDouble();
        gravity.y = this->Internals->Ui.simulation_g_y->text().toDouble();
        gravity.z = this->Internals->Ui.simulation_g_z->text().toDouble();
        this->sphParam->simulationParam->setGravity(gravity);

        Double3 pointmin;
        pointmin.x = this->Internals->Ui.simulation_pointmin_x->text().toDouble();
        pointmin.y = this->Internals->Ui.simulation_pointmin_y->text().toDouble();
        pointmin.z = this->Internals->Ui.simulation_pointmin_z->text().toDouble();
        this->sphParam->simulationParam->setPointMin(pointmin);

        Double3 pointmax;
        pointmax.x = this->Internals->Ui.simulation_pointmax_x->text().toDouble();
        pointmax.y = this->Internals->Ui.simulation_pointmax_y->text().toDouble();
        pointmax.z = this->Internals->Ui.simulation_pointmax_z->text().toDouble();
        this->sphParam->simulationParam->setPointMax(pointmax);
    }

    {
        QString air_rhop = this->Internals->Ui.multiphase_air_rhop->text();
        this->sphParam->airProperty->setRhop(air_rhop.toDouble());
        QString air_cs = this->Internals->Ui.multiphase_air_cs->text();
        this->sphParam->airProperty->setCs(air_cs.toDouble());
        QString air_gamma = this->Internals->Ui.multiphase_air_gamma->text();
        this->sphParam->airProperty->setGamma(air_gamma.toDouble());
        QString air_oxygen = this->Internals->Ui.multiphase_air_oxygen->text();
        this->sphParam->airProperty->setOxygenPercentage(air_oxygen.toDouble());

        QString fuel_rhop = this->Internals->Ui.multiphase_fuel_rhop->text();
        this->sphParam->fuelProperty->setRhop(fuel_rhop.toDouble());
        QString fuel_cs = this->Internals->Ui.multiphase_fuel_cs->text();
        this->sphParam->fuelProperty->setCs(fuel_cs.toDouble());
        QString fuel_gamma = this->Internals->Ui.multiphase_fuel_gamma->text();
        this->sphParam->fuelProperty->setGamma(fuel_gamma.toDouble());
        QString fuel_volume = this->Internals->Ui.multiphase_fuel_volume->text();
        this->sphParam->fuelProperty->setFuelVolume(fuel_volume.toDouble());


        QString inert_rhop = this->Internals->Ui.multiphase_inert_rhop->text();
        this->sphParam->interGasProperty->setRhop(inert_rhop.toDouble());
        QString inert_cs = this->Internals->Ui.multiphase_inert_cs->text();
        this->sphParam->interGasProperty->setCs(inert_cs.toDouble());
        QString inert_gamma = this->Internals->Ui.multiphase_inert_gamma->text();
        this->sphParam->interGasProperty->setGamma(inert_gamma.toDouble());
        QString inert_oxygen = this->Internals->Ui.multiphase_inert_oxygen->text();
        this->sphParam->interGasProperty->setOxygenPercentage(inert_oxygen.toDouble());
    }

    this->sphParam->SaveXml(this->pathConfig->getXmlSaveParh());
}

void SphDockWidget::widgetConstraint(){
    // RegExpValidator worked but didn't pass the test
    QRegExpValidator *reg1 = new QRegExpValidator(QRegExp("^-?(1|0(\\.\\d{1,3})?)$"));
    QRegExpValidator *reg10 = new QRegExpValidator(QRegExp("^-?(10|\\d?(\\.\\d{1,3})?)$"));
    QRegExpValidator *reg100 = new QRegExpValidator(QRegExp("^-?(100|[1-9]?\\d(\\.\\d{1,2})?)$"));
    QRegExpValidator *reg1000 = new QRegExpValidator(QRegExp("^-?(1000|[1-9]?\\d(\\.\\d{1,2})?)$"));
    QRegExpValidator *reg10000 = new QRegExpValidator(QRegExp("^-?(10000|[1-9]?\\d(\\.\\d{1,2})?)$"));
    {
        this->Internals->Ui.simulation_dp->setValidator(reg1);

        this->Internals->Ui.simulation_timemax->setValidator(reg1000);
        this->Internals->Ui.simulation_timeout->setValidator(reg10);

        this->Internals->Ui.simulation_g_x->setValidator(reg100);
        this->Internals->Ui.simulation_g_y->setValidator(reg100);
        this->Internals->Ui.simulation_g_z->setValidator(reg100);

        this->Internals->Ui.simulation_pointmin_x->setValidator(reg10);
        this->Internals->Ui.simulation_pointmin_y->setValidator(reg10);
        this->Internals->Ui.simulation_pointmin_z->setValidator(reg10);

        this->Internals->Ui.simulation_pointmax_x->setValidator(reg10);
        this->Internals->Ui.simulation_pointmax_y->setValidator(reg10);
        this->Internals->Ui.simulation_pointmax_z->setValidator(reg10);
    }

    {
        this->Internals->Ui.multiphase_air_rhop->setValidator(reg10000);
        this->Internals->Ui.multiphase_air_cs->setValidator(reg1000);
        this->Internals->Ui.multiphase_air_gamma->setValidator(reg100);
        this->Internals->Ui.multiphase_air_oxygen->setValidator(reg100);

        this->Internals->Ui.multiphase_fuel_rhop->setValidator(reg10000);
        this->Internals->Ui.multiphase_fuel_cs->setValidator(reg1000);
        this->Internals->Ui.multiphase_fuel_gamma->setValidator(reg100);
        this->Internals->Ui.multiphase_fuel_volume->setValidator(reg100);

        this->Internals->Ui.multiphase_inert_rhop->setValidator(reg10000);
        this->Internals->Ui.multiphase_inert_cs->setValidator(reg1000);
        this->Internals->Ui.multiphase_inert_gamma->setValidator(reg100);
        this->Internals->Ui.multiphase_inert_oxygen->setValidator(reg100);
    }
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

SphDockWidget::~SphDockWidget()
{
    delete this->Internals;
    this->Internals = nullptr;
}
