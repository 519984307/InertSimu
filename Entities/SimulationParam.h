#pragma execution_character_set("utf-8")
#ifndef UNTITLED1_SIMULATIONPARAMETERS_H
#define UNTITLED1_SIMULATIONPARAMETERS_H
#include "SType.h"
#include <QString>

class SimulationParam{
private:

    double dp; //metres(m)
    double timeMax; //Time of simulation, seconds
    double timeOut; //Time out data
    Double3 gravity; // Gravitational acceleration (m/s^2)
    Double3 pointMin; //Minimum boundary point
    Double3 pointMax; //Maximum boundary point

    Int3 modelAngle;
    QString modelPath;

public:
    SimulationParam(){};
    SimulationParam(double dp, double timeMax, double timeOut, Double3 gravity, Double3 pointMin,
                    Double3 pointMax, Int3 modelAngle, QString modelPath){
        this->dp = dp;
        this->timeMax = timeMax;
        this->timeOut = timeOut;
        this->gravity = gravity;
        this->pointMin = pointMin;
        this->pointMax = pointMax;
        this->modelAngle = modelAngle;
        this->modelPath = modelPath;
    };
    void setDp(double dp){
        this->dp = dp;
    };
    double getDp(){
        return this->dp;
    };

    void setTimeMax(double timeMax){
        this->timeMax = timeMax;
    };
    double getTimeMax(){
        return this->timeMax;
    };

    void setTimeOut(double timeOut){
        this->timeOut = timeOut;
    };
    double getTimeOut() {
        return this->timeOut;
    };

    void setGravity(Double3 gravity){
        this->gravity = gravity;
    };
    Double3 getGravity() {
        return this->gravity;
    };

    void setPointMin(Double3 pointMin){
        this->pointMin = pointMin;
    };
    Double3 getPointMin() {
        return this->pointMin;
    };

    void setPointMax(Double3 pointMax){
        this->pointMax = pointMax;
    };
    Double3 getPointMax() {
        return this->pointMax;
    };

    void setModelAngle(Int3 modelAngle){
        this->modelAngle = modelAngle;
    };
    Int3 getModelAngle() {
        return this->modelAngle;
    };

    void setModelPath(QString modelPath){
        this->modelPath = modelPath;
    };
    QString getModelPath() {
        return this->modelPath;
    };
};

#endif //UNTITLED1_SIMULATIONPARAMETERS_H
