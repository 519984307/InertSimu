#pragma execution_character_set("utf-8")
#include <iostream>
using namespace std;

#include <vector>
#include "SphException.h"
#include "ExceptionTool.h"
#include <QFile>
#include <QDebug>

ExceptionTool::ExceptionTool(QObject *parent)
    : QObject{parent}
{
}

QString ExceptionTool::ReadCSV(QString exceptionCode)
{
    Q_INIT_RESOURCE(SPHResource);

    // Open csv-file
    QFile file(":/exception1.csv");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (!file.isOpen()) // 判断文件是否打开
    {
        qDebug() << "open file failed...";
        return 0;
    }

    // Read data from file
    QTextStream stream(&file);
    QString separator(",");
    QString line ;

    while (stream.atEnd() == false)
    {
        line = stream.readLine();
        if(line.split(separator).at(0) == exceptionCode)
            break;
    }

    file.close();
    Q_CLEANUP_RESOURCE(SPHResource);
    return line;
}


SphException* ExceptionTool::getSphException(QString exceptionCode){

    QString exceptionLine = ExceptionTool::ReadCSV(exceptionCode);
    qDebug() << exceptionLine;

    QString separator(",");
    QStringList qlineArray = exceptionLine.split(separator);

    // lineArray to variable
    QString excep_code = qlineArray[0];
    QString excep_stage = qlineArray[1];
    QString excep_type = qlineArray[2];
    QString excep_className = qlineArray[3];
    QString excep_degree = qlineArray[4];
    QString excep_message = qlineArray[5];

    for(int i=6; i<qlineArray.size(); i++){
        excep_message.append(","+qlineArray[i]);
    }

    // variable to entity
    SphException *sphException = new SphException(exceptionCode, excep_stage, excep_type, excep_className, excep_degree, excep_message);

    return sphException;
}
