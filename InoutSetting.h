#ifndef INOUTSETTING_H
#define INOUTSETTING_H

#include <QDialog>
#include "InoutZone.h"
#include "SType.h"
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class InoutSetting;
}

class InoutSetting : public QDialog
{
    Q_OBJECT

public:
    explicit InoutSetting(QWidget *parent = nullptr, InoutZone* iz = nullptr);
    ~InoutSetting();

private:
    Ui::InoutSetting *ui;
    InoutZone *zone;
    void showParam();
    void saveParam();
    void widgetConstraint();
    void widgetRegExpValidat();
    void btnEvent();
    QString on_OpenVelocityFileButton_clicked();

    AxisAngleD ToAxisAngle(Int3 EulerAngle);
    Int3 ToEulerAngle(AxisAngleD AxisAngle);
};

#endif // INOUTSETTING_H
