#pragma execution_character_set("utf-8")
#ifndef FILETOOLS_H
#define FILETOOLS_H

#include<io.h>
#include<iostream>
#include <QObject>

class FileTools : public QObject
{
    Q_OBJECT
public:
    explicit FileTools(QObject *parent = nullptr);
    static int CountAllFilesOnCurFolder(QString path);
    static int CountAllFilesOnCurFolder(QString path, QString contain, QString suffix);
    static QList<QStringList> getLoadMultiDataPath(QString basePath, QString preFileName, QString suffix);
    static bool deleteFiles(QString path);
    static bool fileExist(QString path);

signals:

};

#endif // FILETOOLS_H
