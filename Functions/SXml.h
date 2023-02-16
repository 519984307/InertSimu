#pragma execution_character_set("utf-8")
#ifndef SXML_H
#define SXML_H

#include <QObject>
#include <QFile>
#include <QXmlStreamAttribute>  //单属性
#include <QXmlStreamAttributes> //属性列表
#include <QXmlStreamReader> //操作者
#include <QIODevice>    //文件输入输出设备
#include <QtXml>
#include <QDomDocument>


class SXml : public QObject
{
    Q_OBJECT
public:
    QDomDocument doc;

    explicit SXml(QObject *parent = nullptr);
    void parseDOM(const QString &filename);

    QDomElement GetNodeSimple(const QString &path, const QString &attributeName, const QString &attributeValue);
    QDomElement GetNodeSimple(const QString &path);

    bool LoadXmlFile(const QString &filename);
    bool SaveXmlFile(const QString &filename);
signals:

};

#endif // SXML_H
