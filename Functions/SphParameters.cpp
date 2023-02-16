#pragma execution_character_set("utf-8")
#include "SphParameters.h"
#include <iostream>

using namespace std;

SphParameters::SphParameters(){
    airProperty = NULL;
    fuelProperty = NULL;
    interGasProperty = NULL;
    inoutList = NULL;
    simulationParam = NULL;
    InitVars();
}

SphParameters::~SphParameters(){
    delete airProperty;         airProperty = NULL;
    delete fuelProperty;        fuelProperty = NULL;
    delete interGasProperty;    interGasProperty = NULL;
    delete inoutList;           inoutList = NULL;
    delete simulationParam;     simulationParam = NULL;
}

void SphParameters::InitVars(){
}

// "case.casedef.properties.property"  name="air" 存储在Air airProperty
bool SphParameters::LoadAir(){
    airProperty = new Air();
    QDomElement node = sxml.GetNodeSimple("case.casedef.properties.property", "name", "air");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.tagName() == "rhop0") airProperty->setRhop(node.attribute("value").toDouble());
        if(node.tagName() == "gamma") airProperty->setGamma(node.attribute("value").toDouble());
        if(node.tagName() == "cs0") airProperty->setCs(node.attribute("value").toDouble());
        if(node.tagName() == "visco") airProperty->setVisco(node.attribute("value").toDouble());
        if(node.tagName() == "phasetype") airProperty->setPhaseType(node.attribute("value").toInt());
        node = node.nextSiblingElement();
    }
    return true;
}

// "case.casedef.properties.property"  name="water" 存储在Fuel fuelProperty
bool SphParameters::LoadFuel(){
    fuelProperty = new Fuel();

    QDomElement node = sxml.GetNodeSimple("case.casedef.properties.property", "name", "water");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.tagName() == "rhop0") fuelProperty->setRhop(node.attribute("value").toDouble());
        if(node.tagName() == "gamma") fuelProperty->setGamma(node.attribute("value").toDouble());
        if(node.tagName() == "cs0") fuelProperty->setCs(node.attribute("value").toDouble());
        if(node.tagName() == "visco") fuelProperty->setVisco(node.attribute("value").toDouble());
        if(node.tagName() == "phasetype") fuelProperty->setPhaseType(node.attribute("value").toInt());
        node = node.nextSiblingElement();
    }
    return true;
}

// "case.casedef.constantsdef" InterGas interGasProperty
bool SphParameters::LoadInterGas() {
    interGasProperty = new InterGas();

    QDomElement node = sxml.GetNodeSimple("case.casedef.constantsdef");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.tagName() == "rhop1") interGasProperty->setRhop(node.attribute("value").toDouble());
        if(node.tagName() == "gamma1") interGasProperty->setGamma(node.attribute("value").toDouble());
        node = node.nextSiblingElement();
    }
    interGasProperty->setCs(fuelProperty->getCs());
    interGasProperty->setVisco(fuelProperty->getVisco());
    interGasProperty->setPhaseType(fuelProperty->getPhaseType());

    return true;
}

// 从xml获取airProperty. fuelProperty. interGasProperty的变量值
bool SphParameters::LoadFluid() {
    try {
        LoadAir();
        LoadFuel();
        LoadInterGas();
    } catch (...) {
        return false;
    }

    return true;
}

// "case.execution.special.inout.inoutzone"  inoutList
bool SphParameters::LoadInoutList() {
    inoutList = new InoutList();
    QDomElement node0 = sxml.GetNodeSimple("case.execution.special.inout.inoutzone");

    while(!node0.isNull()){

        QDomElement node = node0.firstChildElement();
        InoutZone *inoutZone = new InoutZone();

        while(!node.isNull()){
            inoutZone->setId(inoutList->getIdmax()+1);
            if(node.tagName() == "inoutphase") inoutZone->setInoutPhase(node.attribute("value").toInt());
            if(node.tagName() == "inputtreatment") inoutZone->setInputTreatment(node.attribute("value").toInt());
            if(node.tagName() == "layers") inoutZone->setLayers(node.attribute("value").toInt());
            if(node.tagName() == "zone3d") {
                QDomElement subnode = node.firstChildElement();
                Circle new_Circle;
                if(!subnode.isNull() && subnode.tagName() == "circle")
                    subnode = subnode.firstChildElement();
                while(!subnode.isNull()) {
                    if(subnode.tagName() == "point") {
                        new_Circle.point.x = subnode.attribute("x").toDouble();
                        new_Circle.point.y = subnode.attribute("y").toDouble();
                        new_Circle.point.z = subnode.attribute("z").toDouble();
                    }
                    if(subnode.tagName() == "radius") {
                        new_Circle.radius = subnode.attribute("v").toDouble();
                    }
                    if(subnode.tagName() == "direction") {
                        new_Circle.direction.x = subnode.attribute("x").toInt();
                        new_Circle.direction.y = subnode.attribute("y").toInt();
                        new_Circle.direction.z = subnode.attribute("z").toInt();
                    }
                    if(subnode.tagName() == "rotateaxis") {
                        new_Circle.rotateAxis.angle = subnode.attribute("angle").toInt();
                        QDomElement subnode_rotate = subnode.firstChildElement();
                        while(!subnode_rotate.isNull()){
                            if(subnode_rotate.tagName() == "point1") {
                                new_Circle.rotateAxis.point1.x = subnode_rotate.attribute("x").toDouble();
                                new_Circle.rotateAxis.point1.y = subnode_rotate.attribute("y").toDouble();
                                new_Circle.rotateAxis.point1.z = subnode_rotate.attribute("z").toDouble();
                            }
                            if(subnode_rotate.tagName() == "point2") {
                                new_Circle.rotateAxis.point2.x = subnode_rotate.attribute("x").toDouble();
                                new_Circle.rotateAxis.point2.y = subnode_rotate.attribute("y").toDouble();
                                new_Circle.rotateAxis.point2.z = subnode_rotate.attribute("z").toDouble();
                            }
                            subnode_rotate = subnode_rotate.nextSiblingElement();
                        }
                    }
                    subnode = subnode.nextSiblingElement();
                }
                inoutZone->setCircle(new_Circle);
            }

            if(node.tagName() == "imposevelocity"){
                ImposeVelocity new_iv;
                new_iv.mode = node.attribute("mode").toInt();
                QDomElement subnode = node.firstChildElement();
                if(!subnode.isNull()) {
                    switch(new_iv.mode) {
                        case 0:
                            new_iv.velocity = subnode.attribute("v").toDouble();
                            break;
                        case 1:
                            new_iv.filePath = subnode.attribute("file");
                            break;
                        default:break;
                    }
                }
                inoutZone->setImposeVelocity(new_iv);
            }
            node = node.nextSiblingElement();
        }

        node0 = node0.nextSiblingElement();

        inoutList->appendZone(inoutZone);
    }

    return true;
}

// "case.execution.parameters"  存储在simulationParam
bool SphParameters::LoadSimulationParam() {
    simulationParam = new SimulationParam();

    QDomElement node = sxml.GetNodeSimple("case.casedef.constantsdef");
    if(!node.isNull())  node = node.firstChildElement();
    else return false;
    while(!node.isNull()){
        if(node.tagName() == "gravity") {
            Double3 new_gravity;
            new_gravity.x = node.attribute("x").toDouble();
            new_gravity.y = node.attribute("y").toDouble();
            new_gravity.z = node.attribute("z").toDouble();
            simulationParam->setGravity(new_gravity);
        }
        node = node.nextSiblingElement();
    }

    node = sxml.GetNodeSimple("case.casedef.geometry.definition");
    if(!node.isNull())  {
        simulationParam->setDp(node.attribute("dp").toDouble());
        node = node.firstChildElement();
    } else return false;
    while(!node.isNull()){
        if(node.tagName() == "pointmin"){
            Double3 new_point;
            new_point.x = node.attribute("x").toDouble();
            new_point.y = node.attribute("y").toDouble();
            new_point.z = node.attribute("z").toDouble();
            simulationParam->setPointMin(new_point);
        }
        if(node.tagName() == "pointmax"){
            Double3 new_point;
            new_point.x = node.attribute("x").toDouble();
            new_point.y = node.attribute("y").toDouble();
            new_point.z = node.attribute("z").toDouble();
            simulationParam->setPointMax(new_point);
        }
        node = node.nextSiblingElement();
    }

    node = sxml.GetNodeSimple("case.execution.parameters");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.attribute("key") == "TimeMax") simulationParam->setTimeMax(node.attribute("value").toDouble());
        if(node.attribute("key") == "TimeOut") simulationParam->setTimeOut(node.attribute("value").toDouble());
        node = node.nextSiblingElement();
    }
    return true;
}

// 从xml获取值，存储在各个类的实例中
bool SphParameters::LoadXml(const QString &path){

    try {
        sxml.LoadXmlFile(path);

        LoadFluid();
        LoadInoutList();
        LoadSimulationParam();
    } catch (...) {
        qDebug() << "Load Xml failed!";
        return false;
    }

    qDebug() << "Load Xml successed...";
    return true;
}

// 将Air airProperty保存在XML中
bool SphParameters::SaveAir(){
    QDomElement node = sxml.GetNodeSimple("case.casedef.properties.property", "name", "air");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.tagName() == "rhop0") node.setAttribute("value", airProperty->getRhop());
        if(node.tagName() == "gamma") node.setAttribute("value", airProperty->getGamma());
        if(node.tagName() == "cs0") node.setAttribute("value", airProperty->getCs());
        if(node.tagName() == "visco") node.setAttribute("value", airProperty->getRhop());
        if(node.tagName() == "phasetype") node.setAttribute("value", airProperty->getPhaseType());
        node = node.nextSiblingElement();
    }
    return true;
}

// 将Fuel fuelProperty保存在XML中
bool SphParameters::SaveFuel(){
    QDomElement node = sxml.GetNodeSimple("case.casedef.properties.property", "name", "water");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.tagName() == "rhop0") node.setAttribute("value", fuelProperty->getRhop());
        if(node.tagName() == "gamma") node.setAttribute("value", fuelProperty->getGamma());
        if(node.tagName() == "cs0") node.setAttribute("value", fuelProperty->getCs());
        if(node.tagName() == "visco") node.setAttribute("value", fuelProperty->getRhop());
        if(node.tagName() == "phasetype") node.setAttribute("value", fuelProperty->getPhaseType());
        node = node.nextSiblingElement();
    }
    return true;
}

// 将InterGas interGasProperty保存在XML中
bool SphParameters::SaveInterGas(){
    QDomElement node = sxml.GetNodeSimple("case.casedef.constantsdef");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.tagName() == "rhop1") node.setAttribute("value", interGasProperty->getRhop());
        if(node.tagName() == "gamma1") node.setAttribute("value", interGasProperty->getGamma());
        node = node.nextSiblingElement();
    }
    return true;
}

// 将多相属性保存在XML中
bool SphParameters::SaveFluid(){
    if(SaveAir() && SaveFuel() && SaveInterGas())
        return true;
    return false;
}

// 将出入口信息保存在XML中
bool SphParameters::SaveInoutList(){
    QDomElement node_inout = sxml.GetNodeSimple("case.execution.special.inout");
    QDomElement subnode_inout;

    // 删除inout下的所有inoutzone
    if(!node_inout.isNull()) {
        subnode_inout = node_inout.firstChildElement();
        while(!subnode_inout.isNull()){
            QDomElement temp = subnode_inout.nextSiblingElement();
            if(subnode_inout.tagName() == "inoutzone")
                node_inout.removeChild(subnode_inout);
            subnode_inout = temp;
        }
    }

    // 将inoutList中的所有inoutzone创建为新的节点树，并添加到sxml.doc中
    foreach (InoutZone *inoutZone, inoutList->getList()) {
        QDomElement zoneNode = sxml.doc.createElement("inoutzone");

        QDomElement nnode = sxml.doc.createElement("refilling");
        nnode.setAttribute("value", 0);
        zoneNode.appendChild(nnode);

        nnode = sxml.doc.createElement("inoutphase");
        nnode.setAttribute("value", inoutZone->getInoutPhase());
        zoneNode.appendChild(nnode);

        nnode = sxml.doc.createElement("inputtreatment");
        nnode.setAttribute("value", inoutZone->getInputTreatment());
        zoneNode.appendChild(nnode);

        nnode = sxml.doc.createElement("layers");
        nnode.setAttribute("value", inoutZone->getLayers());
        zoneNode.appendChild(nnode);

        nnode = sxml.doc.createElement("zone3d");
        {
            QDomElement nnode1 = sxml.doc.createElement("circle");

            QDomElement nnode2 = sxml.doc.createElement("point");
            nnode2.setAttribute("x", inoutZone->getCircle().point.x);
            nnode2.setAttribute("y", inoutZone->getCircle().point.y);
            nnode2.setAttribute("z", inoutZone->getCircle().point.z);
            nnode1.appendChild(nnode2);

            nnode2 = sxml.doc.createElement("radius");
            nnode2.setAttribute("v", inoutZone->getCircle().radius);
            nnode1.appendChild(nnode2);

            nnode2 = sxml.doc.createElement("direction");
            nnode2.setAttribute("x", inoutZone->getCircle().direction.x);
            nnode2.setAttribute("y", inoutZone->getCircle().direction.y);
            nnode2.setAttribute("z", inoutZone->getCircle().direction.z);
            nnode1.appendChild(nnode2);


            nnode2 = sxml.doc.createElement("rotateaxis");
            nnode2.setAttribute("angle", inoutZone->getCircle().rotateAxis.angle);
            {
                QDomElement nnode3 = sxml.doc.createElement("point1");
                nnode3.setAttribute("x", inoutZone->getCircle().rotateAxis.point1.x);
                nnode3.setAttribute("y", inoutZone->getCircle().rotateAxis.point1.y);
                nnode3.setAttribute("z", inoutZone->getCircle().rotateAxis.point1.z);
                nnode2.appendChild(nnode3);

                nnode3 = sxml.doc.createElement("point2");
                nnode3.setAttribute("x", inoutZone->getCircle().rotateAxis.point2.x);
                nnode3.setAttribute("y", inoutZone->getCircle().rotateAxis.point2.y);
                nnode3.setAttribute("z", inoutZone->getCircle().rotateAxis.point2.z);
                nnode2.appendChild(nnode3);
            }
            nnode1.appendChild(nnode2);

            nnode.appendChild(nnode1);
        }
        zoneNode.appendChild(nnode);


        nnode = sxml.doc.createElement("imposevelocity");
        nnode.setAttribute("mode", inoutZone->getImposeVelocity().mode);
        if(inoutZone->getImposeVelocity().mode == 1) {
            QDomElement nnode1 = sxml.doc.createElement("velocityfile");
            nnode1.setAttribute("file", inoutZone->getImposeVelocity().filePath);
            nnode.appendChild(nnode1);
        } else {
            QDomElement nnode1 = sxml.doc.createElement("velocity");
            nnode1.setAttribute("v", inoutZone->getImposeVelocity().velocity);
            nnode.appendChild(nnode1);
        }
        zoneNode.appendChild(nnode);

        node_inout.appendChild(zoneNode);
    }
    return true;
}

// 将计算模拟参数保存在XML中
bool SphParameters::SaveSimulationParam(){
    QDomElement node = sxml.GetNodeSimple("case.casedef.constantsdef");
    if(!node.isNull())  node = node.firstChildElement();
    else return false;
    while(!node.isNull()){
        if(node.tagName() == "gravity") {
            node.setAttribute("x", simulationParam->getGravity().x);
            node.setAttribute("y", simulationParam->getGravity().y);
            node.setAttribute("z", simulationParam->getGravity().z);
        }
        node = node.nextSiblingElement();
    }

    node = sxml.GetNodeSimple("case.casedef.geometry.definition");
    if(!node.isNull())  {
        node.setAttribute("dp", simulationParam->getDp());
        node = node.firstChildElement();
    } else return false;
    while(!node.isNull()){
        if(node.tagName() == "pointmin"){
            node.setAttribute("x", simulationParam->getPointMin().x);
            node.setAttribute("y", simulationParam->getPointMin().y);
            node.setAttribute("z", simulationParam->getPointMin().z);
        }
        if(node.tagName() == "pointmax"){
            node.setAttribute("x", simulationParam->getPointMax().x);
            node.setAttribute("y", simulationParam->getPointMax().y);
            node.setAttribute("z", simulationParam->getPointMax().z);
        }
        node = node.nextSiblingElement();
    }

    node = sxml.GetNodeSimple("case.execution.parameters");

    if(!node.isNull())  node = node.firstChildElement();
    else return false;

    while(!node.isNull()){
        if(node.attribute("key") == "TimeMax") node.setAttribute("value", simulationParam->getTimeMax());
        if(node.attribute("key") == "TimeOut") node.setAttribute("value", simulationParam->getTimeOut());
        node = node.nextSiblingElement();
    }
    return true;
}

// 将所有变量信息保存在XML中
bool SphParameters::SaveXml(const QString &path){

    try {
        SaveFluid();
        SaveInoutList();
        SaveSimulationParam();

        sxml.SaveXmlFile(path);
    } catch (...) {
        qDebug() << "Save Xml failed!";
        return false;
    }

    qDebug() << "Save Xml successed...";
    return true;
}

// 输出多相属性
void SphParameters::VisualFluidProperties(){
    qDebug() << "\nMultiPhase info";
    qDebug() << "---------------------------------------------------";
    qDebug() << "Fuel Property";
    qDebug() << "Rhop:" << fuelProperty->getRhop();
    qDebug() << "Gamma:" << fuelProperty->getGamma();
    qDebug() << "Cs:" << fuelProperty->getCs();
    qDebug() << "Visco:" << fuelProperty->getVisco();
    qDebug() << "PhaseType:" << fuelProperty->getPhaseType();

    qDebug() << "\nAir Property";
    qDebug() << "Rhop:" << airProperty->getRhop();
    qDebug() << "Gamma:" << airProperty->getGamma();
    qDebug() << "Cs:" << airProperty->getCs();
    qDebug() << "Visco:" << airProperty->getVisco();
    qDebug() << "PhaseType:" << airProperty->getPhaseType();

    qDebug() << "\nInterGas Property";
    qDebug() << "Rhop:" << interGasProperty->getRhop();
    qDebug() << "Gamma:" << interGasProperty->getGamma();
    qDebug() << "Cs:" << interGasProperty->getCs();
    qDebug() << "Visco:" << interGasProperty->getVisco();
    qDebug() << "PhaseType:" << interGasProperty->getPhaseType();
}

// 输出出入口信息
void SphParameters::VisualInoutProperties(){
    qDebug() << "\nInout Info";
    qDebug() << "---------------------------------------------------";

    for(int i=0; i<inoutList->getList().size(); i++){
        qDebug() << "Inout " << i;
        qDebug() << "inoutPhase:" << inoutList->getList().at(i)->getInoutPhase() << "\t\t(0:water, 1.air)";
        qDebug() << "inputtreatment:" << inoutList->getList().at(i)->getInputTreatment() << "\t\t(0.2.inlet, 1.outlet)";
        qDebug() << "layers:" << inoutList->getList().at(i)->getLayers();
        qDebug() << "radius:" << inoutList->getList().at(i)->getCircle().radius;
        qDebug() << "position:" << "\tx:"<< inoutList->getList().at(i)->getCircle().point.x
        << "\ty:"<< inoutList->getList().at(i)->getCircle().point.y
        << "\tz:"<< inoutList->getList().at(i)->getCircle().point.z;
        qDebug() << "direction:" << "\tx:"<< inoutList->getList().at(i)->getCircle().direction.x
        << "\ty:"<< inoutList->getList().at(i)->getCircle().direction.y
        << "\tz:"<< inoutList->getList().at(i)->getCircle().direction.z;
        qDebug() << "angle:" << inoutList->getList().at(i)->getCircle().rotateAxis.angle;
        qDebug() << "point1:" << "\tx:"<< inoutList->getList().at(i)->getCircle().rotateAxis.point1.x
        << "\ty:"<< inoutList->getList().at(i)->getCircle().rotateAxis.point1.y
        << "\tz:"<< inoutList->getList().at(i)->getCircle().rotateAxis.point1.z;
        qDebug() << "point2:" << "\tx:"<< inoutList->getList().at(i)->getCircle().rotateAxis.point2.x
        << "\ty:"<< inoutList->getList().at(i)->getCircle().rotateAxis.point2.y
        << "\tz:"<< inoutList->getList().at(i)->getCircle().rotateAxis.point2.z;
        qDebug() << "";
    }
}

// 输出计算模拟参数
void SphParameters::VisualSimulationProperties(){
    qDebug() << "\nSimulationParam";
    qDebug() << "---------------------------------------------------";
    qDebug() << "Gravity" << "\tx:"<< simulationParam->getGravity().x << "\ty:"<< simulationParam->getGravity().y
    << "\tz:"<< simulationParam->getGravity().z;
    qDebug() << "Dp:" << simulationParam->getDp();
    qDebug() << "PointMin" << "\tx:"<< simulationParam->getPointMin().x << "\ty:"<< simulationParam->getPointMin().y
    << "\tz:"<< simulationParam->getPointMin().z;
    qDebug() << "PointMax" << "\tx:"<< simulationParam->getPointMax().x << "\ty:"<< simulationParam->getPointMax().y
    << "\tz:"<< simulationParam->getPointMax().z;
    qDebug() << "TimeMax:" << simulationParam->getTimeMax();
    qDebug() << "TimeOut:" << simulationParam->getTimeOut();
}

// 输出所有变量信息
void SphParameters::VisualALLProperties(){

    try {
        VisualFluidProperties();
        VisualInoutProperties();
        VisualSimulationProperties();

        qDebug() << "---------------------------------------------------";
        qDebug() << "Properties Visual Over...\n";
    } catch (...) {
        qDebug() << "Properties Visual failed!";
    }
}
