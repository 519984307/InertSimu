#pragma execution_character_set("utf-8")
#include "TaskOperation.h"
#include "FileTools.h"
#include "SphDockWidget.h"
#include "ui_TaskOperation.h"
#include "QWidget"

TaskOperation::TaskOperation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskOperation)
{
    ui->setupUi(this);
    SphDockWidget sphDockWidget;
    TaskOperation::setMessageVisibility(sphDockWidget.getpath());

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

void TaskOperation::setMessageVisibility(QString path)
{
    int countData=FileTools::CountAllFilesOnCurFolder(path+"/data","part","bi4");
    int countParticles=FileTools::CountAllFilesOnCurFolder(path+"/particles","PartFluid_","vtk");
    if(countData!=countParticles)
    {
        this->ui->sph_label->setText("检测到上次模拟任务有"+QString::number(countData)+"个数据文件。");
        delete this->ui->horizontalLayout_2;
        this->ui->btn_sph_abort->setVisible(false);
        this->resize(QSize(320,100));
    }
    if(countData==countParticles)
    {
        this->ui->sph_label->setText("检测到上次模拟任务已完成。");
        delete this->ui->horizontalLayout;
        this->ui->btn_sph_continue->setVisible(false);
        this->resize(QSize(320,100));
    }
}

TaskOperation::~TaskOperation()
{
    delete ui;
}
