#pragma execution_character_set("utf-8")

#include "Fluid.h"
#ifndef UNTITLED1_INERTGAS_H
#define UNTITLED1_INERTGAS_H
class InertGas: public Fluid{
private:
    double oxygenPercentage=0;
    double nitrogenPercentage=0;

public:
    InertGas(){};
    InertGas(double oxygenPercentage, double nitrogenPercentage){
        this->oxygenPercentage = oxygenPercentage;
        this->nitrogenPercentage = nitrogenPercentage;
    };
    void setOxygenPercentage(double op){
        this->oxygenPercentage = op;
    };
    double getOxygenPercentage(){
        return this->oxygenPercentage;
    };

    void setNitrogenPercentage(double np){
        this->nitrogenPercentage = np;
    };
    double getNitrogenPercentage(){
        return this->nitrogenPercentage;
    };
};
#endif //UNTITLED1_INERTGAS_H
