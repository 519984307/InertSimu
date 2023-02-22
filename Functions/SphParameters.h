#pragma execution_character_set("utf-8")
#include "Air.h"
#include "Fuel.h"
#include "InertGas.h"
#include "InoutList.h"
#include "SimulationParam.h"
#include "MainList.h"
#include "SXml.h"

#ifndef ENTITY_SPHPARAMETER_H
#define ENTITY_SPHPARAMETER_H
class SphParameters{
public:
    Air *airProperty;
    Fuel *fuelProperty;
    InertGas *inertGasProperty;
    InoutList *inoutList;
    SimulationParam *simulationParam;
    MainList *mainList;

public:
    SphParameters();
    ~SphParameters();

    void InitVars();

    // 从XML中读取并赋值到上述变量
    bool LoadXml(const QString &path);

    //输出变量信息
    void VisualFluidProperties();
    void VisualInoutProperties();
    void VisualSimulationProperties();
    void VisualMainList();
    void VisualALLProperties();

    //将所有变量信息保存在XML中
    bool SaveXml(const QString &path);

private:
    SXml sxml;

    bool LoadAir();
    bool LoadFuel();
    bool LoadInertGas();
    bool LoadFluid();
    bool LoadInoutList();
    bool LoadSimulationParam();
    bool LoadMainList();

    bool SaveAir();
    bool SaveFuel();
    bool SaveInertGas();
    bool SaveFluid();
    bool SaveInoutList();
    bool SaveSimulationParam();
    bool SaveMainList();
};
#endif //ENTITY_SPHPARAMETER_H
