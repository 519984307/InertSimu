#pragma execution_character_set("utf-8")
#ifndef TASKOPERATION_H
#define TASKOPERATION_H

#include <QDialog>

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

    void setMessageVisibility();

};

#endif // TASKOPERATION_H
