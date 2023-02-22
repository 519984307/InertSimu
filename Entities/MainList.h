#ifndef FILLBOXGEOLIST_H
#define FILLBOXGEOLIST_H
#include "FillBox.h"
#include "Model.h"
#include <QObject>

class MainList {
private:
    QList<Model*> modelList;
    QList<FillBox*> fillBoxList;
public:
    MainList(){
    };

    const QList<Model *> &getModelList() const {
        return modelList;
    }
    void setModelList(const QList<Model *> &modelList) {
        MainList::modelList = modelList;
    }

    const QList<FillBox *> &getFillBoxList() const {
        return fillBoxList;
    }
    void setFillBoxList(const QList<FillBox *> &fillBoxList) {
        MainList::fillBoxList = fillBoxList;
    }

    void appendFillBox(FillBox* fillBox){
        this->fillBoxList.append(fillBox);
    }
    void appendModel(Model* model){
        this->modelList.append(model);
    }
};

#endif // FILLBOXGEOLIST_H
