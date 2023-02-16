#ifndef INOUTSETTING_H
#define INOUTSETTING_H

#include <QDialog>

namespace Ui {
class InoutSetting;
}

class InoutSetting : public QDialog
{
    Q_OBJECT

public:
    explicit InoutSetting(QWidget *parent = nullptr);
    ~InoutSetting();

private:
    Ui::InoutSetting *ui;
};

#endif // INOUTSETTING_H
