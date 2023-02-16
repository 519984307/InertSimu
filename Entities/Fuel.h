#pragma execution_character_set("utf-8")

#include "Fluid.h"
#ifndef UNTITLED1_FLUE_H
#define UNTITLED1_FLUE_H
class Fuel: public Fluid{
private:
    double fuelVolume=0;
public:
    Fuel(){};
    Fuel(double fuelVolume){
        this->fuelVolume = fuelVolume;
    };

    double getFuelVolume() {
        return this->fuelVolume;
    };
    void setFuelVolume(double fuelVolume){
        this->fuelVolume = fuelVolume;
    }
};
#endif //UNTITLED1_FLUE_H
