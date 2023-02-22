#pragma execution_character_set("utf-8")
#include "TaskOperation.h"
#include "ui_TaskOperation.h"

TaskOperation::TaskOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskOperation)
{
    ui->setupUi(this);

    QObject::connect(this->ui->btn_sph_continue, &QPushButton::clicked, this,[&](){
        emit operation(1);
        this->close();
    });
    QObject::connect(this->ui->btn_sph_abort, &QPushButton::clicked, this,[&](){
        emit operation(2);
        this->close();
    });
    QObject::connect(this->ui->btn_sph_post, &QPushButton::clicked, this,[&](){
        emit operation(3);
        this->close();
    });
}

TaskOperation::~TaskOperation()
{
    delete ui;
}
