#pragma execution_character_set("utf-8")
#ifndef PATHCONFIG_H
#define PATHCONFIG_H

#include <QObject>
#include <QJsonDocument>       // 提供了一种读写JSON文档的方法
#include <QJsonObject>         // 封装了一个JSON对象
#include <QJsonValue>          // 表示json格式中的一个值
#include <QFile>               // 用于文件操作
#include <QDebug>


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

public:

    // temporary path configuration load from json file for coding in early stage.
    // it should be automatic config after installed
    void init(){
        Q_INIT_RESOURCE(SPHResource);

        //读json文件
        QFile file(":/config.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray data = file.readAll();
        file.close();
        Q_CLEANUP_RESOURCE(SPHResource);

        QJsonParseError jError;	//创建QJsonParseError对象
        //使用QJsonDocument的fromJson函数读取json串，并将QJsonParseError对象传入获取错误值
        QJsonDocument doc = QJsonDocument::fromJson(data, &jError);
        //判断QJsonParseError对象获取的error是否包含错误，包含则返回0
        if(jError.error != QJsonParseError::NoError){
            qDebug() << jError.error;
            return ;
        }

        if(doc.isObject())
        {
            QJsonObject obj = doc.object();
            QJsonValue value = obj.value("path");
            if(value.isObject())
            {
                QJsonObject subobj = value.toObject();
                setXmlPath(subobj.value("xmlPath").toString());
                setXmlSavePath(subobj.value("xmlSavePath").toString());
                setSphBinPath(subobj.value("sphBinPath").toString());
                setSphWorkPath(subobj.value("sphWorkPath").toString());
                setSphOutPath(subobj.value("sphOutPath").toString());
                setSphBatName(subobj.value("sphBatName").toString());
                qDebug() << "path set successed";
            }
        }

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
};

#endif // PATHCONFIG_H
