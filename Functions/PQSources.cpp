#include "PQSources.h"
#include <QDebug>
#include <pqDataRepresentation.h>
#include <vtkSMPropertyHelper.h>

PQSources::PQSources(){
    this->taskName = "";
    this->workPath = "";
    this->outPath = "";
}

void PQSources::init(QString taskName, QString workPath){
    this->taskName = taskName;
    this->workPath = workPath;
    this->outPath = workPath + "/" + taskName + "_out";
}

bool PQSources::insertAll(QString modelName, QString FuelSurName, QString FuelPartName, QString AirPartName, QString InertPartName){
    try {
        bool res1 = insertModel(modelName);
        bool res2 = insertFuelSur(FuelSurName);
        bool res3 = insertFuelPart(FuelPartName);
        bool res4 = insertAirPart(AirPartName);
        bool res5 = insertInertPart(InertPartName);

        if(!(res1 && res2 && res3 && res4 && res5))
            return false;
    } catch (...) {
        return false;
    }
    return true;
}

bool PQSources::insertModel(QString name){
    QString fileName = this->outPath + "/" + taskName + "__Actual.vtk";
    QVector<pqPipelineSource *> Qvtkpointer;

    if(FileTools::fileExist(fileName)){
        QStringList fileList;
        QList<QStringList> files0;
        fileList << fileName;
        files0 << fileList;
        Qvtkpointer = pqLoadDataReaction::loadFilesForSupportedTypes(files0);  // 直接打开文件内容到渲染窗口

        this->sourcesMap.insert(name, Qvtkpointer);
        return true;
    }
    return false;
};

bool PQSources::insertFuelSur(QString name){
    QString basePath = this->outPath + "/surface";
    QString preFileName = "Surface_fuel_";
    QString suffix = "vtk";

    return PQSources::insertSource(basePath, preFileName, suffix, name);
}

bool PQSources::insertFuelPart(QString name){
    QString basePath = this->outPath + "/particles";
    QString preFileName = "Particle_fuel_";
    QString suffix = "vtk";

    return PQSources::insertSource(basePath, preFileName, suffix, name);
}

bool PQSources::insertAirPart(QString name){
    QString basePath = this->outPath + "/particles";
    QString preFileName = "Particle_air_";
    QString suffix = "vtk";

    return PQSources::insertSource(basePath, preFileName, suffix, name);
}

bool PQSources::insertInertPart(QString name){
    QString basePath = this->outPath + "/particles";
    QString preFileName = "Particle_inert_";
    QString suffix = "vtk";

    return PQSources::insertSource(basePath, preFileName, suffix, name);
}

bool PQSources::insertSource(QString basePath, QString preFileName, QString suffix, QString name){

    QVector<pqPipelineSource *> Qvtkpointer;
    QList<QStringList> files = FileTools::getLoadMultiDataPath(basePath, preFileName, suffix);
    if(files.size() > 0) {
        Qvtkpointer = pqLoadDataReaction::loadFilesForSupportedTypes(files);  // 直接打开文件内容到渲染窗口
        this->sourcesMap.insert(name, Qvtkpointer);
        return true;
    }
    return false;
}

QMap<QString, QVector<pqPipelineSource *>> PQSources::getSourcesMap() const {
    return this->sourcesMap;
}

bool PQSources::removeSource(QString name){
    return this->sourcesMap.remove(name);
}

QVector<pqPipelineSource *> PQSources::getSource(QString name){
    return this->sourcesMap.value(name);
}
