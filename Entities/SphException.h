#pragma execution_character_set("utf-8")
#pragma once
#define SPHExceptions
#include <iostream>
#include <QObject>
#include <QDebug>

class SphException
{
private:
    QString stage;
    QString type;
    QString degree;
    QString code;       //异常代码
    QString details;    //异常内容
    QString className;  //异常类

public:
    SphException();
    SphException(QString code, QString details){
        this->code = code;
        this->details = details;
    };
    SphException(QString code, QString stage, QString type, QString className, QString degree, QString details){
        this->code = code;
        this->details = details;
        this->className = className;
        this->stage = stage;
        this->type = type;
        this->degree = degree;
    };

    void setStage(QString stage){
        this->stage = stage;
    };
    QString getStage(){
        return this->stage;
    };

    void setType(QString type){
        this->type = type;
    };
    QString getType(){
        return this->type;
    };

    void setDegree(QString degree){
        this->degree = degree;
    };
    QString getDegree(){
        return this->degree;
    };

    void setCode(QString code){
        this->code = code;
    };
    QString getCode(){
        return this->code;
    };

    void setDetails(QString details){
        this->details = details;
    };
    QString getDetails(){
        return this->details;
    };

    void setClassName(QString className){
        this->className = className;
    };
    QString getClassName(){
        return this->className;
    };
    
    QString outputMessage(){
        QString excepInfo;
        excepInfo.append("Exception Code:"+this->code).append("\n");
        excepInfo.append("Details:"+this->details).append("\n");
        if(!this->type.isNull()) excepInfo.append("Type:"+this->type).append("\t\t");
        if(!this->type.isNull()) excepInfo.append("Stage:"+this->stage).append("\n");
        if(!this->type.isNull()) excepInfo.append("ClassName:"+this->className).append("\t\t");
        if(!this->type.isNull()) excepInfo.append("Degree:"+this->degree+"\n");

        qDebug().noquote() << excepInfo;

        return excepInfo;
    };

};
