#pragma execution_character_set("utf-8")
#ifndef PATHCONFIG_H
#define PATHCONFIG_H

#include <QObject>
#include <QJsonDocument>       // 提供了一种读写JSON文档的方法
#include <QJsonObject>         // 封装了一个JSON对象
#include <QJsonValue>          // 表示json格式中的一个值
#include <QFile>               // 用于文件操作
#include <QDebug>
#include <QDir>


class PathConfig {
// temporary path　configuration for coding in early stage.
// it should be automatic load after installed
private:
    QString xmlPath;
    QString xmlSavePath;
    QString sphBinPath;
    QString sphWorkPath;
    QString sphOutPath;
    QString sphBatName;

    QString tankconfig;

public:

    void init(){
        QString currentPath = QDir::currentPath();
        setSphOutPath(currentPath + "/DualSPHysics/work/01_DamBreak/CaseDambreak_out");
        setTankconfig(currentPath + "/DualSPHysics/work/caseconfig");
        setXmlPath(currentPath + "/DualSPHysics/work/caseconfig/Tank_config3_Def.xml");
        setXmlSavePath(currentPath + "/DualSPHysics/work/caseconfig/Tank_config3_Def.xml");
        setSphBinPath(currentPath + "/DualSPHysics/bin");
        setSphWorkPath(currentPath + "/DualSPHysics/work/01_DamBreak");
        setSphBatName("wCaseDambreak_win64_CPU.bat");
    };

    void setXmlPath(QString path){
        this->xmlPath = path;
    };
    QString getXmlParh(){
        return xmlPath;
    };

    void setXmlSavePath(QString path){
        this->xmlSavePath = path;
    };
    QString getXmlSaveParh(){
        return xmlSavePath;
    };

    void setSphBinPath(QString path){
        this->sphBinPath = path;
    };
    QString getSphBinParh(){
        return sphBinPath;
    };

    void setSphWorkPath(QString path){
        this->sphWorkPath = path;
    };
    QString getSphWorkPath(){
        return sphWorkPath;
    };

    void setSphOutPath(QString path){
        this->sphOutPath = path;
    };
    QString getSphOutPath(){
        return sphOutPath;
    };

    void setSphBatName(QString path){
        this->sphBatName = path;
    };
    QString getSphBatName(){
        return sphBatName;
    };

    void setTankconfig(QString path){
        this->tankconfig = path;
    };
    QString getTankconfig(){
        return tankconfig;
    };
};

#endif // PATHCONFIG_H
