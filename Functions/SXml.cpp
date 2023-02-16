#pragma execution_character_set("utf-8")
#include "SXml.h"
#include <QDebug>

SXml::SXml(QObject *parent)
    : QObject{parent}
{
}

QDomElement SXml::GetNodeSimple(const QString &path) {
    QString pathx = path;
    QDomElement node;
    QDomElement base = doc.documentElement();

    while(pathx.length() && !base.isNull()){
      int pos=int(pathx.indexOf("."));
      QString txword=(pos>0? pathx.mid(0,pos): pathx);
      pathx=(pos>0? pathx.mid(pos+1): "");
      if(txword.length()){
        node = base;
        while(!node.isNull()){
//            QString curTagName = node.tagName();
            if(node.tagName() == txword){
                break;
            } else {
                node = node.nextSiblingElement();
            }
        }
        base = node.firstChild().toElement();
//        QString curBaseTagName = base.tagName();
      }
    }

    if(node.isNull())
        qDebug() << "Can't find xml node.   path : " << path;

    return node;
}

QDomElement SXml::GetNodeSimple(const QString &path, const QString &attributeName, const QString &attributeValue){
    QDomElement node = SXml::GetNodeSimple(path);

    //匹配 node.attribute(attributeValue) = attributeValue
    while(!node.isNull() && node.attribute(attributeName) != attributeValue) {
        node = node.nextSiblingElement();
    }

    if(node.isNull())
        qDebug() << "Can't find xml node.   path : " << path << "  attributeName:" << attributeName << "  attributeValue:" << attributeValue;

    return node;
}

// filename：xml文件完整路径
// 从文件中读取xml内容
bool SXml::LoadXmlFile(const QString &filename){
    QFile xmlFile(filename);
    if (!xmlFile.open(QFile::ReadOnly))
        return false;

    // for debug
    QString err;
    int errline;
    int errcol;

    if (!doc.setContent(&xmlFile,false,&err,&errline,&errcol))
    {
        qDebug() << "XML Load failed";
        qDebug() << "err=" << err << "  errline=" << errline << "  errcol=" << errcol;
        xmlFile.close();
        return false;
    }
    xmlFile.close();
    return true;
}

// filename：xml文件完整路径
// 将xml内容保存到文件中
bool SXml::SaveXmlFile(const QString &filename){
    QFile xmlFile(filename);

    if (xmlFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream outFile(&xmlFile);
        outFile.setCodec("UTF-8");
        doc.save(outFile, QDomNode::EncodingFromDocument);
        xmlFile.close();
    } else return false;
    return true;
}
