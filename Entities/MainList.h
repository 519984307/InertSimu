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

    void appendFillBox(FillBox* fillBox){
        this->fillBoxList.append(fillBox);
    }
    void appendModel(Model* model){
        this->modelList.append(model);
    }
};

#endif // FILLBOXGEOLIST_H
