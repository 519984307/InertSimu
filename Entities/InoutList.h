#pragma execution_character_set("utf-8")
#include "InoutZone.h"
#include <QObject>
#ifndef UNTITLED1_INOUTLIST_H
#define UNTITLED1_INOUTLIST_H

#endif //UNTITLED1_INOUTLIST_H
class InoutList
{
private:
    int idmax;
    QList<InoutZone*> list;
public:

    InoutList(){
        this->idmax = 0;
    };
    InoutList(QList<InoutZone*> list){
        this->idmax = 0;
        this->list = list;
    };

    void setIdmax(int idmax){
        this->idmax = idmax;
    }
    int getIdmax(){
        return this->idmax;
    }

    void setList(QList<InoutZone*> list){
        this->list = list;
    };
    void appendZone(InoutZone* zone){
        this->list.append(zone);
    }
    QList<InoutZone*> getList(){
        return this->list;
    };
};
