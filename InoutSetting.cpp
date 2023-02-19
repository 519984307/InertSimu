#include "InoutSetting.h"
#include "ui_InoutSetting.h"

InoutSetting::InoutSetting(QWidget *parent, InoutZone *iz) :
    QDialog(parent),
    ui(new Ui::InoutSetting)
{
    ui->setupUi(this);
    this->zone = iz;
    this->showParam();
}

void InoutSetting::showParam(){
    this->ui->inout_name->setText(zone->getName());
    this->ui->inout_layers->setCurrentIndex(zone->getLayers()-1);
    this->ui->inout_treatment->setCurrentIndex(zone->getInputTreatment());  // 0:inlet,  1:outlet
    this->ui->inout_phase->setCurrentIndex(zone->getInoutPhase());  // 0:fluid,  1:air
    this->ui->inout_v_mode->setCurrentIndex(zone->getImposeVelocity().mode); //0:fixed value, 1:variable value
    this->ui->inout_v_velocity->setText(QString::number(zone->getImposeVelocity().velocity));

    this->ui->inout_radius->setText(QString::number(zone->getCircle().radius));

    this->ui->inout_position_x->setText(QString::number(zone->getCircle().point.x));
    this->ui->inout_position_y->setText(QString::number(zone->getCircle().point.y));
    this->ui->inout_position_z->setText(QString::number(zone->getCircle().point.z));

    // ToEulerAngle
    this->ui->inout_rotate_x->setText(QString::number(zone->getCircle().rotateAxis.angle));
    this->ui->inout_rotate_y;
    this->ui->inout_rotate_y;

}

Double3 InoutSetting::ToAxisAngle(Int3 EulerAngle){

}
Int3 InoutSetting::ToEulerAngle(Double3 AxisAngle){

}

InoutSetting::~InoutSetting()
{
    delete ui;
}
