#pragma execution_character_set("utf-8")
#ifndef TASKOPERATION_H
#define TASKOPERATION_H

#include <QDialog>
#include "PathConfig.h"

namespace Ui {
class TaskOperation;
}

class TaskOperation : public QDialog
{
    Q_OBJECT

public:
    explicit TaskOperation(QWidget *parent = nullptr);
    ~TaskOperation();

signals:
    void operation(int);

private:
    Ui::TaskOperation *ui;
    PathConfig *pathConfig;

    void setMessageVisibility(QString path_1, QString contain_1, QString suffix_1,QString path_2, QString contain_2, QString suffix_2);

};

#endif // TASKOPERATION_H
