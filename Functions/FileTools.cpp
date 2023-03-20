#pragma execution_character_set("utf-8")
#include "FileTools.h"
#include <QDebug>
#include <QDir>

FileTools::FileTools(QObject *parent)
    : QObject{parent}
{
}

bool FileTools::deleteFiles(QString path){
    QDir qdir(path);
    if(!qdir.exists()){
        return true;
    }
    qdir.removeRecursively();
    return true;
}

bool FileTools::dirExist(QString path){
    QDir qdir(path);
    if(qdir.exists()){
        return true;
    }
    return false;
}

bool FileTools::fileExist(QString path){
    QFileInfo fileInfo(path);
    if(fileInfo.isFile()){
        return true;
    }
    return false;
}


// path : 路径
// contain : 文件名包含的字符串
// suffix : 后缀
// return : 文件数量
int FileTools::CountAllFilesOnCurFolder(QString path, QString contain, QString suffix){
    //可以定义后面的后缀为*.exe，，.txt等来查找特定后缀的文件，，.*是通配符，，配所有类型,路径连接符最好是左斜杠/，，跨平台
    QString s = "*";
    if(contain.size() > 0)
        s = contain + "*";

    if(suffix.size() > 0)
        s += "." + suffix;
    else
        s += ".*";

    __finddata64_t file_info;
    int file_num = 0;
    QString current_path = path + "/" + s;

    //cout << path + "\\" + s << endl;

    __int64 handle = _findfirst64(current_path.toStdString().c_str(), &file_info);
    //返回值为--则查找失败
    if (-1 == handle) {
        qDebug() << "files not exist! check the path";
        qDebug() << current_path;
        _findclose(handle);
        return 0;
    }

    do
    {
        QString attribute;
        if (file_info.attrib == _A_SUBDIR) //是目录
            attribute = "dir";
        else
            attribute = "file";

        //获得的最后修改时间是time_t格式的长整型
        //cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl;
        file_num++;

    } while (!_findnext64(handle, &file_info));

    //关闭文件句柄
    _findclose(handle);
    return file_num;
}


// path : 路径
// return : 文件数量
int FileTools::CountAllFilesOnCurFolder(QString path){
    return FileTools::CountAllFilesOnCurFolder(path, "", "");
}


// path : 路径
// contain : 文件名包含的字符串
// suffix : 后缀
// return : 文件路径列表
QList<QStringList> FileTools::getLoadMultiDataPath(QString basePath, QString preFileName, QString suffix){
    //传入参数：basePath, preFileName, suffix
    int fileNum = FileTools::CountAllFilesOnCurFolder(basePath, preFileName, suffix);

    QList<QStringList> files;

    if(fileNum>0) {
        QStringList filesPath;
        basePath = basePath +  "/" + preFileName;

        for (qint16 i = 0; i < fileNum; i++) {
            QString file_number = "";
            if (i < 10) {
                file_number += "000" + QString::number(i,10);
            } else if (i < 100) {
                file_number += "00" + QString::number(i,10);
            } else if (i < 1000) {
                file_number += "0" + QString::number(i,10);
            } else{
                file_number += QString::number(i,10);
            }
            QString file_url = basePath + file_number + ".vtk";
            filesPath << file_url;
        }
        files << filesPath;    // 将路径拼接成所需要的格式
    }

    return files;
}
