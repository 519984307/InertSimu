#ifndef PQSOURCES_H
#define PQSOURCES_H

#include <QObject>
#include "FileTools.h"
#include <pqPipelineSource.h>
#include <pqLoadDataReaction.h>

class PQSources{
private:
    QMap<QString, QVector<pqPipelineSource *>> sourcesMap;
    QString taskName;
    QString workPath;
    QString outPath;

public:
    PQSources();
    void init(QString taskName, QString workPath);

    QMap<QString, QVector<pqPipelineSource *>> getSourcesMap() const;
    bool removeSource(QString name);
    void clearMap();
    QVector<pqPipelineSource *> getSource(QString name);

    bool insertAll(QString modelName, QString FuelSurName, QString FuelPartName, QString AirPartName, QString InertPartName);

    bool insertModel(QString name);
    bool insertFuelSur(QString name);
    bool insertFuelPart(QString name);
    bool insertAirPart(QString name);
    bool insertInertPart(QString name);
    bool insertSource(QString basePath, QString preFileName, QString suffix, QString name);
};
#endif // PQSOURCES_H
