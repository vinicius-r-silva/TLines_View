#pragma once
#include <cmath>
//v(t) = 2u(t)
//v(t) = u(t) - u(t-l*/(10*uf)
//uf -> 0.9c

//#define l 100        //meters
#define MAX_TIME 10    //seconds
//#define _c 300000000     //light speed

#define DT 0.001
#define DZ 0.01

//sem perdas
#define _R 0 //resistance
#define _G 0 //conductance

#define Z0 50
#define Rs 75
//#define Zl infinity
#define Zl 0
//#define Zl 100

#define DEGRAU 0
#define CONTINUA 1

#define ZERO 0
#define CEM 1
#define INFINITA 2

const double C = 7.40741 * powf64x(10, -11);
const double L = 1.85185 * powf64x(10, -7);

typedef struct functionData_t{

    double **voltage;
    double **current;

    double minCurrent;
    double maxCurrent;

    double minVoltage;
    double maxVoltage;

}functionData_t;

void freeMemory(functionData_t* functionData);
functionData_t* allocMemory(int vol, int res, float dt, int nt, float dz, int nz);
double initialFunction(double t);
functionData_t* calculateAllValues(functionData_t* functionData, int vol, int res, float dt, int nt, float dz, int nz);

double getCurrent(functionData_t* functionData, double t, double z, float dt, float dz);
double getVoltage(functionData_t* functionData, double t, double z, float dt, float dz);
