#include "function.h"
#include <iostream>

void freeMemory(functionData_t* FData){
    int t;

    double **voltage = FData->voltage;
    double **current = FData->current;

    for (t = 0; voltage[t] != nullptr; t++)
    {
        delete[] voltage[t];
        delete[] current[t];
    }

    delete[] voltage;
    delete[] current;

    delete FData;

    std::cout << "ALL CLEAR\n";
}

double continueFunction(int nt, double limit){
    return 2;
}

double stepFunction(int nt, double limit){
    return (nt >= limit) ? 0 : 1;
}

functionData_t* allocMemory(int vol, int res, double dt, double nt, double dz, double nz){

    int K = nz/dz;
    int N = nt/dt;

    int t;

    double **voltage;
    double **current;


    functionData_t* FData = new functionData_t;

    voltage = new double *[N + 2];
    current = new double *[N + 2];

    std::cout << "iniciando alocacao" << std::endl;
    for (t = 0; t < N + 1; t++)
    {
        voltage[t] = new double[K + 1];
        current[t] = new double[K + 1];

        if (voltage[t] == nullptr || current[t] == nullptr)
        {
            std::cout << "sem memoria" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    voltage[N + 1] = nullptr;
    current[N + 1] = nullptr;

    FData->voltage = voltage;
    FData->current = current;

    return calculateAllValues(FData, vol, res, dt, nt, dz, nz);

}


functionData_t* calculateAllValues(functionData_t* FData, int vol, int res, double dt, double nt, double dz, double nz){
    int Nz = nz/dz;
    int Nt = nt/dt;

    const double limit = Nz / 10.0;

    //const double C1 = (-2.0*dt) / (dt*dz*_R + 2*dz*L);
    //const double C2 = (2.0*L - dt*_R) / (2*L + dt*_R);
    //const double C3 = (-2.0*dt) / (dt*dz*_G + 2*dz*C);
    //const double C4 = (2.0*C - dt*_G) / (2*C + dt*_G);

    // const double C1 = -dt / (dz*L);
    // const double C2 = 1;
    // const double C3 = -dt / (dz*C);
    // const double C4 = 1;

    const double C1 = -dt / (dz*C);
    const double C2 = -dt / (dz*L);


    voltageFunction Vg;
    Vg = &continueFunction;

    if(vol == CONTINUA)
        Vg = &continueFunction;
    else if(vol == DEGRAU)
        Vg = &stepFunction;

    double Rl = 0;

    if(res == ZERO)
        Rl = 0;
    else if(res == CEM)
        Rl = 100;
    else if(res == INFINITA)
        Rl = std::numeric_limits<double>::max();


    // const double _REFLECTION_SOURCE = (double) (Rs - Z0)/(Rs + Z0);
    // const double _REFLECTION_LOAD = (Rl - Z0)/(Rl + Z0);

    std::cout << "K: "<< Nz << std::endl;
    std::cout << "N: "<< Nt << std::endl;


    std::cout << "C: "<< C << std::endl;
    std::cout << "L: "<< L << std::endl;

    // std::cout << "C1: "<< C1 << std::endl;
    // std::cout << "C2: "<< C2 << std::endl;
    // std::cout << "C3: "<< C3 << std::endl;
    // std::cout << "C4: "<< C4 << std::endl;

    std::cout << "Vph: "<< Vph << std::endl;
    std::cout << "limit: "<< limit << std::endl;

    int t;
    int z;

    double **v = FData->voltage;
    double **i = FData->current;

    double minVoltage = 0;
    double maxVoltage = 0;

    double minCurrent = 0;
    double maxCurrent = 0;


    std::cout << "iniciando calculo" << std::endl;

    for (t = 0; t < Nt + 1; t++)
    {
        for (z = 0; z < Nz + 1; z++){
            i[t][z] = 0;
            v[t][z] = 0;
        }
    }
    
    v[0][0] = 0;
    i[0][0] = 0;
    
    const long double B1 = 2*dt/(Rs*C*dz);
    const long double B2 = 2*dt/(Rl*C*dz);
    
    for (t = 1; t < Nt; t++)
    {
        v[t][0] = (1 - B1)*v[t-1][0] + 2*C1*i[t-1][0] + B1*Vg(t-1,limit);

        for (z = 1; z < Nz; z++)
            v[t][z] = C1*(i[t-1][z] - i[t-1][z-1]) + v[t-1][z]; 

        v[t][Nz] = (1 - B2) * v[t-1][Nz] - 2*C1*i[t-1][Nz-1];

        if(res == ZERO){
            v[t][Nz] = 0;
        }else if(res == INFINITA)
            v[t][Nz] = v[t][Nz-1];

        for (z = 0; z < Nz; z++)
            i[t][z] = C2*(v[t][z+1] - v[t][z]) + i[t-1][z];
    }

    
    
    FData->minVoltage = minVoltage;
    FData->maxVoltage = maxVoltage;

    FData->minCurrent = minCurrent;
    FData->maxCurrent = maxCurrent;    

    std::cout << "calculo terminado" << std::endl;

    return FData;
}

double getVoltage(functionData_t* FData, double t, double z, double dt, double dz){

    int nt = t/dt;
    int nz = z/dz;
    
    return FData->voltage[nt][nz];
}

double getCurrent(functionData_t* FData, double t, double z, double dt, double dz){
    
    int nt = t/dt;
    int nz = z/dz;
    
    return FData->current[nt][nz];
}
