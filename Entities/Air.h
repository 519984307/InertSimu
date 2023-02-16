#pragma execution_character_set("utf-8")

#include "Fluid.h"
#ifndef UNTITLED1_AIR_H
#define UNTITLED1_AIR_H
class Air: public Fluid{

private:
    double oxygenPercentage=0;
    double nitrogenPercentage=0;

public:
    void setOxygenPercentage(double op){
        this->oxygenPercentage = op;
    }
    double getOxygenPercentage(){
        return this->oxygenPercentage;
    }

    void setNitrogenPercentage(double np){
        this->nitrogenPercentage = np;
    }
    double getNitrogenPercentage(){
        return this->nitrogenPercentage;
    }
};
#endif //UNTITLED1_AIR_H
