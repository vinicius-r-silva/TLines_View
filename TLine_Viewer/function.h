#pragma once
#include <cmath>
//v(t) = 2u(t)
//v(t) = u(t) - u(t-l*/(10*uf)
//uf -> 0.9c

#define l 100        //meters
#define MAX_TIME 10    //seconds
#define c 300000000     //light speed

#define DT 0.001
#define DZ 0.01

//sem perdas
#define R 0 //resistance
#define G 0 //conductance

#define Z0 50
#define Rs 75
//#define Zl infinity
#define Zl 0
//#define Zl 100

const int K = l/DZ;
const int N = MAX_TIME/DT;

const double C = 7.40741 * powf64x(10, -11);
const double L = 1.85185 * powf64x(10, -7);

const double C1 = (-2*DT) / (DT*DZ*R + 2*DZ*L);
const double C2 = (2*L - DT*R) / (2*L + DT*R);
const double C3 = (-2*DT) / (DT*DZ*G + 2*DZ*C);
const double C4 = (2*C - DT*G) / (2*C + DT*G);


typedef struct functionData_t{

    double **voltage;
    double **current;

}functionData_t;

double initialFunction(double t);
functionData_t calculateAllValues();