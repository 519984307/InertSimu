#pragma execution_character_set("utf-8")
#ifndef SPHTHREAD_H
#define SPHTHREAD_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include "SphException.h"

typedef enum{
    state_noWork=0,         //任务未开始
    state_init=1,           //正在初始化
    state_compute=2,        //正在计算
    state_postProcess=3,    //正在后处理
    state_finish=4,         //任务结束
    state_exception=5       //异常状态
}StateType;

#define StateCode "StateCode-S"
#define ErrorCode "SphExcept-E"

class SphThread:public QThread
{
    Q_OBJECT
public:
    SphThread();
    SphThread(QString &workPath, QString &batName);

    void setParameters(QString &workPath, QString &batName);
    void run();

    QString stateText();
    void filter(QString);
    float getNumFromStr(QString);
    void extractProgress(QString);
    void detectState(QString);
    void detectError(QString);

    //getter and setter
    int getProgress() const{ return progress; };
    QString getLabelState() const{ return labelState; };
    StateType getState() const { return state; };
    QString getEndtime() const { return endtime; };
    int getCurStep() const{ return curStep; };
    int getMaxStep() const{ return maxStep; };


private:
    QProcess *process;
    QString workPath;
    QString batName;
    SphException *sphExcep;

private:
    int curStep;                //current step from text
    int maxStep;                //Max step in SPH
    int progress;               //progress for ui
    StateType state;
    QString labelState;         //statetext for ui
    QString endtime;   // Endtime for ui

    void init();
    void initParam();
    void setProgress(int progress);
    void setLabelState(QString labelState);
    void setState(StateType state);
    void setEndtime(QString endtime);
    void setCurStep(int curStep);
    void setMaxStep(int maxStep);
    void closeProcess();

public slots:
    void sphStarted();
    void sphFinished(int exitCode,QProcess::ExitStatus exitStatus);
    void sphStream();
    void sphContinue();
    void sphPostProcess();


signals:
    void threadSig_Text(QString);
    void threadSig_State(QString);
    void threadSig_Progress(int);
    void threadSig_Endtime(QString);
};

#endif // SPHTHREAD_H
