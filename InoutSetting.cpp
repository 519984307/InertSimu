#include "InoutSetting.h"
#include "ui_InoutSetting.h"

InoutSetting::InoutSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InoutSetting)
{
    ui->setupUi(this);
}

InoutSetting::~InoutSetting()
{
    delete ui;
}
