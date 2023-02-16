#pragma execution_character_set("utf-8")
#include "SphThread.h"
#include "ExceptionTool.h"
#include <QDebug>
#include <iostream>
#include <QRegularExpression>
using namespace std;

SphThread::SphThread()
{}

SphThread::SphThread(QString &workPath, QString &batName)
{
    this->workPath = workPath;
    this->batName = batName;
}

void SphThread::setParameters(QString &workPath, QString &batName)
{
    this->workPath = workPath;
    this->batName = batName;
}

void SphThread::init()
{
    setCurStep(0);
    setMaxStep(0);
    setProgress(0);
    setState(StateType(0));
    setLabelState("");
    setEndtime("");

    this->process = NULL;
    this->sphExcep = NULL;
}

void SphThread::initParam()
{
    setCurStep(0);
    setMaxStep(0);
    setProgress(0);
    setState(StateType(0));
    setLabelState("");
    setEndtime("");
}

//========================================================================================
// setter
void SphThread::setProgress(int progress)
{
    if(progress<0) {
        qDebug() << "error progress value:" << progress;
        progress = 0;

    }
    if(progress>100) {
        qDebug() << "error progress value:" << progress;
        progress = 100;

    }
    this->progress = progress;
    emit threadSig_Progress(progress);
};

void SphThread::setLabelState(QString labelState)
{
    this->labelState = labelState;
}

void SphThread::setState(StateType stateType)
{
    this->state = stateType;
    emit threadSig_State(stateText());
}

void SphThread::setEndtime(QString endtime)
{
    this->endtime = endtime;
    emit threadSig_Endtime(endtime);
};

void SphThread::setCurStep(int curStep)
{
    if(curStep<0) {
        qDebug() << "error newCurStep value:" << curStep;
        curStep = 0;
    }
    this->curStep = curStep;
}

void SphThread::setMaxStep(int maxStep)
{
    if(maxStep<0) {
        qDebug() << "error newMaxStep value:" << maxStep;
        maxStep = 0;
    }
    this->maxStep = maxStep;
}
//========================================================================================

void SphThread::detectState(QString text){
    if(text.lastIndexOf(StateCode) != -1) {
        QString scode = text.at(text.indexOf(StateCode)+11);
        qDebug() << "detected: " << StateCode << scode.toInt();
        setState(StateType(scode.toInt()));
        qDebug() << "State:" << getState();
    }
}

void SphThread::detectError(QString text){
    if(text.lastIndexOf(ErrorCode) != -1) {
        QString ecode = text.mid(text.indexOf(ErrorCode)+11, 4);
        qDebug() << "detected: " << ErrorCode << ecode;
        this->sphExcep = ExceptionTool::getSphException("SphExcep-E"+ecode);
        // Handle Exception
    }
}


// 从文本中提取数字
float SphThread::getNumFromStr(QString subText){
    QString numText;
    for(int j = 0; j < subText.length(); j++)
    {
        if((subText[j] >= '0' && subText[j] < '9') || (subText[j] == '.'))
        numText.append(subText[j]);
    }
    return numText.toFloat();
}

// 计算进度
void SphThread::extractProgress(QString text){
    int oldStep = getCurStep();

    // 使用正则匹配时间，字符串匹配当前计算步数
    static QRegularExpression re("(\\d{2})-(\\d{2})-(\\d{4})\\s(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    QString str = QString("Part_%1").arg(oldStep+1, 4, 10, QLatin1Char('0'));

    while(text.indexOf(str) != -1)
    {
        oldStep++;
        str = QString("Part_%1").arg(oldStep+1, 4, 10, QLatin1Char('0'));

        QRegularExpressionMatch match = re.match(text);
        if (match.hasMatch()) {
            // 年-月-日 时:分:秒
            // QString endTime = match.captured(3) + "-" + match.captured(2) + "-" + match.captured(1) + " " +
            //                    match.captured(4) + ":" + match.captured(5) + ":" + match.captured(6);

            // 月-日 时:分:秒
            QString endTime = match.captured(2) + "-" + match.captured(1) + " " +
                    match.captured(4) + ":" + match.captured(5) + ":" + match.captured(6);

            qDebug() << "endtime:" << endTime;
            setEndtime(endTime);
        } else {
            qDebug() << "endtime match failed";
        }

    }
    setCurStep(oldStep);
    qDebug() << "CurStep:" << getCurStep();
    qDebug() << "MaxStep:" << getMaxStep();

    // 计算进度最大值为95%，完成后处理后为100%
    int newProgress  = (getCurStep()/(float)getMaxStep())*95;
    if(newProgress>getProgress()) {setProgress(newProgress);}
}

// 处理输出流文本
void SphThread::filter(QString text){
    // call StateCode & ErrorCode detect function.
    detectState(text);
    detectError(text);

    if(getMaxStep()==0) {
        // 获取timeMax和timePart
        QString timeMaxText = text.mid(text.indexOf("TimeMax="), 15);
        float timeMax = getNumFromStr(timeMaxText);
        QString timePartText = text.mid(text.indexOf("TimePart="), 15);
        float timePart = getNumFromStr(timePartText);
        if(timeMax!=0 && timePart!=0){
            // qDebug() << "\n\nMaxStep:" << timeMax/timePart;
            setMaxStep(timeMax/timePart);
        }
    }
    if(getState() == state_compute) {
        // 模拟计算中，提取计算进度
        extractProgress(text);
    }
    if(getState() == state_finish) {
        setProgress(100);
        closeProcess();
    }
}

void SphThread::sphContinue()
{
    this->initParam();
    this->process->write("1\n\r");
    qDebug()<<"sph Continue";
}

void SphThread::sphPostProcess()
{
    this->initParam();
    this->process->write("2\n\r");
    qDebug()<<"sph Continue";
}

void SphThread::closeProcess()
{
    sleep(1);
    this->process->write("exit\n\r");
}

void SphThread::sphStarted()
{
    setState(state_init);
    setCurStep(0);
    qDebug()<<"started";
}

void SphThread::sphFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    setState(state_finish);
    qDebug()<<"finished";
    qDebug()<<exitCode;
    qDebug()<<exitStatus;
}

void SphThread::sphStream()
{
    //能解析转义字符
    QString outstr = process->readAllStandardOutput().data();
    //cout << outstr.toStdString();
    //强制清空缓冲区
    fflush(stdout);

    filter(outstr);
    emit threadSig_Text(outstr);
}

// 获取状态文本
QString SphThread::stateText(){
    QString labeltext;
    switch(this->state){
        case state_noWork:          labeltext = "未开始";break;
        case state_init:            labeltext = "正在初始化";break;
        case state_compute:         labeltext = "正在计算";break;
        case state_postProcess:     labeltext = "正在生成文件";break;
        case state_finish:          labeltext = "完成模拟";break;
        case state_exception:       labeltext = "任务异常";break;
        default: labeltext = "未知状态";
    }
    return labeltext;
}

void SphThread::run()
{
    this->init();

    this->process = new QProcess();
    this->process->setProcessChannelMode(QProcess::MergedChannels);
    this->process->setWorkingDirectory(workPath);

    //process槽函数 -> this
    connect(this->process, SIGNAL(started()), this, SLOT(sphStarted()));
    connect(this->process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(sphFinished(int,QProcess::ExitStatus)));
    connect(this->process, SIGNAL(readyReadStandardOutput()), this, SLOT(sphStream()));

    //开始进程并阻塞至运行结束
    this->process->setProgram(workPath + "/" + batName);
    this->process->start(); //开始
    this->process->waitForStarted(-1);
    this->process->waitForFinished(-1);
    sleep(2);//等待标准读结束
    this->process->close(); //关闭进程
}
