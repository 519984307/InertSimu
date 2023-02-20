﻿#ifndef INOUTSETTING_H
#define INOUTSETTING_H

#include <QDialog>
#include "InoutZone.h"
#include "SType.h"

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

    AxisAngleD ToAxisAngle(Int3 EulerAngle);
    Int3 ToEulerAngle(AxisAngleD AxisAngle);
};

#endif // INOUTSETTING_H
