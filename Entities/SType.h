//
// Created by Kang on 2023/2/9.
//

#ifndef DEMO2_STYPE_H
#define DEMO2_STYPE_H

typedef struct{
    double x;
    double y;
    double z;
}Double3;

typedef struct{
    float x;
    float y;
    float z;
}Float3;

typedef struct{
    Double3 axis;
    double angle;
}AxisAngleD;

typedef struct{
    Float3 axis;
    float angle;
}AxisAngleF;

typedef struct{
    int x;
    int y;
    int z;
}Int3;
#endif //DEMO2_STYPE_H
