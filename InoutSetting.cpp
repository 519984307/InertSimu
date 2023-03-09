#include "InoutSetting.h"
#include "qmath.h"
#include "ui_InoutSetting.h"
#include "InoutZone.h"

InoutSetting::InoutSetting(QWidget *parent, InoutZone *iz) :
    QDialog(parent),
    ui(new Ui::InoutSetting)
{
    QRegExpValidator *reg100p0 = new QRegExpValidator(QRegExp("^((100|[1-9]?\\d)(\\.\\d{0,1})?)$"));
    QRegExpValidator *reg1p00 = new QRegExpValidator(QRegExp("^-?(1|0(\\.\\d{0,2})?)$"));
    QRegExpValidator *regne100p00 = new QRegExpValidator(QRegExp("^-?((100|[1-9]?\\d)(\\.\\d{0,2})?)$"));
    QRegExpValidator *regne1000 = new QRegExpValidator(QRegExp("^-?(1000|[1-9]\\d{0,2}|0)$"));

    ui->setupUi(this);
    this->zone = iz;
    this->widgetConstraint();
    this->widgetRegExpValidat();
    this->btnEvent();
    this->showParam();

    ui->inout_v_velocity->setValidator(reg100p0);
    ui->inout_radius->setValidator(reg1p00);
    ui->inout_rotate_x->setValidator(regne1000);
    ui->inout_rotate_y->setValidator(regne1000);
    ui->inout_rotate_z->setValidator(regne1000);
    ui->inout_position_x->setValidator(regne100p00);
    ui->inout_position_y->setValidator(regne100p00);
    ui->inout_position_z->setValidator(regne100p00);
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

    // ToEulerAngle here
    AxisAngleD axisAngle;
    axisAngle.angle = zone->getCircle().rotateAxis.angle;
    axisAngle.axis = zone->getCircle().rotateAxis.point2;
    axisAngle.axis.x += zone->getCircle().rotateAxis.point1.x;
    axisAngle.axis.y += zone->getCircle().rotateAxis.point1.y;
    axisAngle.axis.z += zone->getCircle().rotateAxis.point1.z;

    Int3 eulerAngle = ToEulerAngle(axisAngle);
    this->ui->inout_rotate_x->setText(QString::number(eulerAngle.x - 90));
    this->ui->inout_rotate_y->setText(QString::number(eulerAngle.y));
    this->ui->inout_rotate_z->setText(QString::number(eulerAngle.z));

    if(this->ui->inout_v_mode->currentIndex()==0){
        this->ui->inout_v_velocity->setEnabled(true);
        this->ui->btn_inout_v_path->setEnabled(false);
    }
    if(this->ui->inout_v_mode->currentIndex()==1){
        this->ui->inout_v_velocity->setEnabled(false);
        this->ui->btn_inout_v_path->setEnabled(true);
    }
}

void InoutSetting::saveParam(){
    zone->setName(this->ui->inout_name->text());
    zone->setLayers(this->ui->inout_layers->currentIndex() + 1);
    zone->setInputTreatment(this->ui->inout_treatment->currentIndex());  // 0:inlet,  1:outlet
    zone->setInoutPhase(this->ui->inout_phase->currentIndex());  // 0:fluid,  1:air

    ImposeVelocity iv = zone->getImposeVelocity();
    iv.mode = this->ui->inout_v_mode->currentIndex();
    iv.velocity = this->ui->inout_v_velocity->text().toDouble();
    iv.filePath = this->ui->inout_v_filepath->text();
    zone->setImposeVelocity(iv);

    Circle circle = zone->getCircle();
    circle.radius = this->ui->inout_radius->text().toDouble();
    circle.point.x = this->ui->inout_position_x->text().toDouble();
    circle.point.y = this->ui->inout_position_y->text().toDouble();
    circle.point.z = this->ui->inout_position_z->text().toDouble();

    circle.direction.x = 0;
    circle.direction.y = 1;
    circle.direction.x = 0;

    // ToAxisAngle here
    Int3 eulerAngle;
    eulerAngle.x = this->ui->inout_rotate_x->text().toInt() + 90;
    eulerAngle.y = this->ui->inout_rotate_y->text().toInt();
    eulerAngle.z = this->ui->inout_rotate_z->text().toInt();

    AxisAngleD axisAngle = ToAxisAngle(eulerAngle);
    circle.rotateAxis.angle = axisAngle.angle;
    circle.rotateAxis.point1 = circle.point;
    circle.rotateAxis.point2.x = axisAngle.axis.x - circle.point.x;
    circle.rotateAxis.point2.y = axisAngle.axis.y - circle.point.y;
    circle.rotateAxis.point2.z = axisAngle.axis.z - circle.point.z;

    zone->setCircle(circle);
}

void InoutSetting::widgetConstraint(){
    this->ui->inout_v_filepath->setEnabled(false);

}

void InoutSetting::widgetRegExpValidat(){

}

void InoutSetting::btnEvent(){
    // 导入速度文件
    QObject::connect(this->ui->btn_inout_v_path, &QPushButton::clicked, this,[&](){
        QString vPath = this->on_OpenVelocityFileButton_clicked();
        this->ui->inout_v_filepath->setText(vPath);
    });

    //重载函数，QT5的信号槽语法要使用函数指针语法连接到这个信号，需要使用static_cast转换指针
    QObject::connect(this->ui->inout_v_mode, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,[&](int index){
        if(index==0){
            this->ui->inout_v_velocity->setEnabled(true);
            this->ui->btn_inout_v_path->setEnabled(false);
        }
        if(index==1){
            this->ui->inout_v_velocity->setEnabled(false);
            this->ui->btn_inout_v_path->setEnabled(true);
        }
    });

    // 保存参数
    QObject::connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this,[&](){
        this->saveParam();
    });
}


QString InoutSetting::on_OpenVelocityFileButton_clicked()
{
    //文件夹路径
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "选择速度文件","/","速度文件 (*.csv);; 所有文件 (*.*);; ");

    if(!filePath.isEmpty()) {
        qDebug() << "path=" << filePath;
    }
    return filePath;
}

AxisAngleD InoutSetting::ToAxisAngle(Int3 EulerAngle){
    AxisAngleD AxisAngle;
    double a=EulerAngle.x*M_PI/180;
    double b=EulerAngle.y*M_PI/180;
    double c=EulerAngle.z*M_PI/180;
    AxisAngle.angle=acos(cos(a/2)*cos(b/2)*cos(c/2)+sin(a/2)*sin(b/2)*sin(c/2))*360/M_PI;
    AxisAngle.axis.x=(sin(a/2)*cos(b/2)*cos(c/2)-cos(a/2)*sin(b/2)*sin(c/2))/sin(AxisAngle.angle*M_PI/360);
    AxisAngle.axis.y=(cos(a/2)*sin(b/2)*cos(c/2)+sin(a/2)*cos(b/2)*sin(c/2))/sin(AxisAngle.angle*M_PI/360);
    AxisAngle.axis.z=(cos(a/2)*cos(b/2)*sin(c/2)-sin(a/2)*sin(b/2)*cos(c/2))/sin(AxisAngle.angle*M_PI/360);
    return AxisAngle;
}
Int3 InoutSetting::ToEulerAngle(AxisAngleD AxisAngle){
    Int3 EulerAngle;
    double w=cos(AxisAngle.angle*M_PI/360), x=AxisAngle.axis.x*sin(AxisAngle.angle*M_PI/360), y=AxisAngle.axis.y*sin(AxisAngle.angle*M_PI/360), z=AxisAngle.axis.z*sin(AxisAngle.angle*M_PI/360);
    EulerAngle.x = (int)(atan2(2 * y * z + 2 * w * x, -2 * x * x - 2 * y* y + 1)*180/M_PI+0.5);
    EulerAngle.y = (int)(asin(-2 * x * z + 2 * w* y)*180/M_PI+0.5);
    EulerAngle.z = (int)(atan2(2 * x * y + 2 * w * z, -2 * y*y - 2 * z * z + 1)*180/M_PI+0.5);
    return EulerAngle;
}

InoutSetting::~InoutSetting()
{
    delete ui;
}
