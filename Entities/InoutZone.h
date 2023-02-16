#pragma execution_character_set("utf-8")
#ifndef UNTITLED1_INOUTZONE_H
#define UNTITLED1_INOUTZONE_H
#include <QObject>
#include "SType.h"

typedef struct{
    int angle;
    Double3 point1;
    Double3 point2;
}RotateAxis;

typedef struct{ //Input zone for 3-D simulations
    Double3 point;
    double radius;
    Int3 direction;
    RotateAxis rotateAxis;
}Circle;

typedef struct {
    int mode; //Imposed velocity 0:fixed value, 1:variable value, 2:Extrapolated value (default=0)
    double velocity;
    QString filePath;
}ImposeVelocity;

class InoutZone{
private:
    int id;
    QString name;
    int inoutPhase; //inoutphase mode. 0:water, 1.air (default=0)
    int inputTreatment; //Treatment of fluid entering the zone. 0:No changes, 1:Convert fluid (necessary for outlet), 2:Remove fluid (recommended for inlet)
    int layers; //Number of inlet/outlet particle layers
    Circle circle;
    ImposeVelocity imposeVelocity;

public:
    InoutZone(){};
    InoutZone(QString name, int inoutPhase, int inputTreatment, int layers, Circle circle, ImposeVelocity imposeVelocity){
        this->name = name;
        this->inoutPhase = inoutPhase;
        this->inputTreatment = inputTreatment;
        this->layers = layers;
        this->circle = circle;
        this->imposeVelocity = imposeVelocity;
    };
    void setId(int id){
        this->id = id;
    }
    int getId(){
        return this->id;
    }
    void setName(QString name){
        this->name = name;
    };
    QString getName(){
        return this->name;
    };

    void setInoutPhase(int ip){
        this->inoutPhase = ip;
    };
    int getInoutPhase(){
        return this->inoutPhase;
    };

    void setInputTreatment(int it){
        this->inputTreatment = it;
    };
    int getInputTreatment(){
        return this->inputTreatment;
    };

    void setLayers(int layers){
        this->layers = layers;
    };
    int getLayers(){
        return this->layers;
    };

    void setCircle(Circle circle){
        this->circle = circle;
    };
    Circle getCircle(){
        return this->circle;
    };

    void setImposeVelocity(ImposeVelocity iv){
        this->imposeVelocity = iv;
    };
    ImposeVelocity getImposeVelocity(){
        return this->imposeVelocity;
    };
};

#endif //UNTITLED1_INOUTZONE_H
