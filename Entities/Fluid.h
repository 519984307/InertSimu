#pragma execution_character_set("utf-8")

#ifndef UNTITLED1_PROPERTIES_H
#define UNTITLED1_PROPERTIES_H

#include <QObject>

class Fluid {
private:
    QString name;
    double rhop;
    double gamma;
    double cs;
    double visco;
    int phaseType; // 0:water, 1.air
public:
    Fluid(){};
    Fluid(QString name, double rhop, double gamma, double cs, double visco, int phaseType){
        this->name = name;
        this->rhop = rhop;
        this->gamma = gamma;
        this->cs = cs;
        this->visco = visco;
        this->phaseType = phaseType;
    };

    void setName(QString name){
        this->name = name;
    };
    QString getName(){
        return this->name;
    };

    void setRhop(double rhop){
        this->rhop = rhop;
    };
    double getRhop(){
        return this->rhop;
    };

    void setGamma(double gamma){
        this->gamma = gamma;
    };
    double getGamma(){
        return this->gamma;
    };

    void setCs(double cs){
        this->cs = cs;
    };
    double getCs(){
        return this->cs;
    };

    void setVisco(double visco){
        this->visco = visco;
    };
    double getVisco(){
        return this->visco;
    };

    void setPhaseType(int phaseType){
        this->phaseType = phaseType;
    };
    int getPhaseType(){
        return this->phaseType;
    };
};

#endif //UNTITLED1_PROPERTIES_H

