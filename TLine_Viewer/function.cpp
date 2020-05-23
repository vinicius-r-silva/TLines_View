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
    int z;

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
    int K = nz/dz;
    int N = nt/dt;

    const double limit = K / 10.0;

    //const double C1 = (-2.0*dt) / (dt*dz*_R + 2*dz*L);
    //const double C2 = (2.0*L - dt*_R) / (2*L + dt*_R);
    //const double C3 = (-2.0*dt) / (dt*dz*_G + 2*dz*C);
    //const double C4 = (2.0*C - dt*_G) / (2*C + dt*_G);

    const double C1 = -dt / (dz*L);
    const double C2 = 1;
    const double C3 = -dt / (dz*C);
    const double C4 = 1;

    voltageFunction funcPrt;

    if(vol == CONTINUA)
        funcPrt = &continueFunction;
    else if(vol == DEGRAU)
        funcPrt = &stepFunction;

    double Zl = 0;

    if(res == ZERO)
        Zl = 0;
    else if(res == CEM)
        Zl = 100;
    else if(res == INFINITA)
        Zl = std::numeric_limits<double>::max();


    const double _REFLECTION_SOURCE = (double) (Rs - Z0)/(Rs + Z0);
    const double _REFLECTION_LOAD = (Zl - Z0)/(Zl + Z0);

    std::cout << "K: "<< K << std::endl;
    std::cout << "N: "<< N << std::endl;


    std::cout << "C: "<< C << std::endl;
    std::cout << "L: "<< L << std::endl;

    std::cout << "C1: "<< C1 << std::endl;
    std::cout << "C2: "<< C2 << std::endl;
    std::cout << "C3: "<< C3 << std::endl;
    std::cout << "C4: "<< C4 << std::endl;

    std::cout << "Vph: "<< Vph << std::endl;
    std::cout << "limit: "<< limit << std::endl;

    int t;
    int z;

    double **voltage = FData->voltage;
    double **current = FData->current;

    double minVoltage = 0;
    double maxVoltage = 0;

    double minCurrent = 0;
    double maxCurrent = 0;


    std::cout << "iniciando calculo" << std::endl;

    for (t = 0; t < N + 1; t++)
    {
        for (z = 0; z < K + 1; z++){
            current[t][z] = 0;
            voltage[t][z] = 0;
        }
    }
    
    int powerL = 0;
    int powerS = 0;

    voltage[0][0] = 0;
    current[0][0] = 0;
    
    const long double B1 = 2*dt/(Rs*C*dz);
    const long double B2 = 2*dt/(Zl*C*dz);
    const long double _r = dt*dt/(L*C*dz*dz);

    
    for (t = 1; t < N; t++)
    {
        voltage[t][0] = (1-B1)*voltage[t-1][0]/-C3 - 2*current[t-1][0] + 2.0*funcPrt(t, limit)/Rs;
        voltage[t][0] *= -C3;
    
        for (z = 1; z < K; z++)
        {
            voltage[t][z] = voltage[t-1][z]/-C3 - (current[t-1][z] - current[t-1][z-1]);
            voltage[t][z] *= -C3;

            if(voltage[t][z] > maxVoltage)
                maxVoltage = voltage[t][z];
            else if(voltage[t][z] < minVoltage)
                minVoltage = voltage[t][z];
        }        

        voltage[t][K] = (1-B2)*voltage[t-1][K]/-C3 + 2*current[t-1][K-1];
        voltage[t][K] *= -C3;

        if(res == ZERO){
            voltage[t][K] = 0;
        }
        

        for (z = 0; z < K; z++)
        {
            current[t][z] = current[t-1][z] - _r*(voltage[t][z+1]/-C3 - voltage[t][z]/-C3);
            
            if(current[t][z] > maxCurrent)
                maxCurrent = current[t][z];
            else if(current[t][z] < minCurrent)
                minCurrent = current[t][z];
        }
        
        
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
