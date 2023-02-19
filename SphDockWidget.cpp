﻿#pragma execution_character_set("utf-8")
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

    // 导入XML配置
    this->sphParam = new SphParameters();
    QObject::connect(this->Internals->Ui.btn_imp_config, &QPushButton::clicked, this,[&](){
        this->sphParam->LoadXml(this->pathConfig->getXmlParh());
        //this->sphParam->VisualALLProperties();
        emit this->showParam();
    });

    // 导出XML配置
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
//    ui.tableWidget->setSelectionMode(QAbstractItemView::NoSelection);		//设置不可选
//    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


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

void SphDockWidget::widgetConstraint(){
    Ui::SphDockWidget ui = this->Internals->Ui;

    // RegExpValidator worked but didn't pass the test
    QRegExpValidator *reg1 = new QRegExpValidator(QRegExp("^-?(1|0(\\.\\d{1,3})?)$"));
    QRegExpValidator *reg10 = new QRegExpValidator(QRegExp("^-?(10|\\d?(\\.\\d{1,3})?)$"));
    QRegExpValidator *reg100 = new QRegExpValidator(QRegExp("^-?(100|[1-9]?\\d(\\.\\d{1,2})?)$"));
    QRegExpValidator *reg1000 = new QRegExpValidator(QRegExp("^-?(1000|[1-9]?\\d(\\.\\d{1,2})?)$"));
    QRegExpValidator *reg10000 = new QRegExpValidator(QRegExp("^-?(10000|[1-9]?\\d(\\.\\d{1,2})?)$"));
    {
        ui.simulation_dp->setValidator(reg1);

        ui.simulation_timemax->setValidator(reg1000);
        ui.simulation_timeout->setValidator(reg10);

        ui.simulation_g_x->setValidator(reg100);
        ui.simulation_g_y->setValidator(reg100);
        ui.simulation_g_z->setValidator(reg100);

        ui.simulation_pointmin_x->setValidator(reg10);
        ui.simulation_pointmin_y->setValidator(reg10);
        ui.simulation_pointmin_z->setValidator(reg10);

        ui.simulation_pointmax_x->setValidator(reg10);
        ui.simulation_pointmax_y->setValidator(reg10);
        ui.simulation_pointmax_z->setValidator(reg10);
    }

    {
        ui.multiphase_air_rhop->setValidator(reg10000);
        ui.multiphase_air_cs->setValidator(reg1000);
        ui.multiphase_air_gamma->setValidator(reg100);
        ui.multiphase_air_oxygen->setValidator(reg100);

        ui.multiphase_fuel_rhop->setValidator(reg10000);
        ui.multiphase_fuel_cs->setValidator(reg1000);
        ui.multiphase_fuel_gamma->setValidator(reg100);
        //ui.multiphase_fuel_volume->setValidator(reg100);

        ui.multiphase_inert_rhop->setValidator(reg10000);
        ui.multiphase_inert_cs->setValidator(reg1000);
        ui.multiphase_inert_gamma->setValidator(reg100);
        ui.multiphase_inert_oxygen->setValidator(reg100);
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
