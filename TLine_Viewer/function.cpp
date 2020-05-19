#include "function.h"
#include <iostream>

void freeMemory(functionData_t* functionData){
    int t;

    double **voltage = functionData->voltage;
    double **current = functionData->current;

    for (t = 0; voltage[t] != nullptr; t++)
    {
        delete[] voltage[t];
        delete[] current[t];
    }

    delete[] voltage;
    delete[] current;

    delete functionData;

    std::cout << "ALL CLEAR\n";
}


functionData_t* allocMemory(int vol, int res, double dt, double nt, double dz, double nz){

    int K = nz/dz;
    int N = nt/dt;

    int t;
    int z;

    double **voltage;
    double **current;

    functionData_t* functionData = new functionData_t;

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

    functionData->voltage = voltage;
    functionData->current = current;

    return calculateAllValues(functionData, vol, res, dt, nt, dz, nz);

}


functionData_t* calculateAllValues(functionData_t* functionData, int vol, int res, double dt, double nt, double dz, double nz){
    int K = nz/dz;
    int N = nt/dt;

    const double C1 = (-2.0*dt) / (dt*dz*_R + 2*dz*L);
    const double C2 = (2.0*L - dt*_R) / (2*L + dt*_R);
    const double C3 = (-2.0*dt) / (dt*dz*_G + 2*dz*C);
    const double C4 = (2.0*C - dt*_G) / (2*C + dt*_G);


    std::cout << "K: "<< K << std::endl;
    std::cout << "N: "<< N << std::endl;


    std::cout << "C: "<< C << std::endl;
    std::cout << "L: "<< L << std::endl;

    std::cout << "C1: "<< C1 << std::endl;
    std::cout << "C2: "<< C2 << std::endl;
    std::cout << "C3: "<< C3 << std::endl;
    std::cout << "C4: "<< C4 << std::endl;

    int t;
    int z;

    double **voltage = functionData->voltage;
    double **current = functionData->current;

    double minVoltage = 0;
    double maxVoltage = 2;

    double minCurrent = 0;
    double maxCurrent = 2.0 / (Z0 + Zl + Rs);


    std::cout << "iniciando calculo" << std::endl;

    //inicial values
    voltage[0][0] = 2;
    current[0][0] = 2.0 / (Z0 + Zl + Rs);

    /*
    for (t = 0; t < N + 1; t++)
    {
        voltage[t][0] = 2;
        current[t][0] = 0;
    }
    */
    
    for (z = 1; z < K + 1; z++)
    {
        voltage[0][z] = 0;
        current[0][z] = 0;
    }
    
    std::cout << "parte inicial completa" << std::endl;


    for (t = 0; t < N; t++)
    {
        for (z = 0; z < K; z++){
            // double cur = C1 * (voltage[t][z + 1] - voltage[t][z - 1]) + C2 * current[t - 1][z];
            current[t + 1][z] = C1 * (voltage[t][z + 1] - voltage[t][z]) + C2 * current[t][z];
            /* 
            if(!std::isinf(current[t + 1][z]) && current[t + 1][z] > maxCurrent)
                maxCurrent = current[t + 1][z];
            else if(!std::isinf(current[t + 1][z]) && current[t + 1][z] < minCurrent)
                minCurrent = current[t + 1][z];
            */
        }

        for (z = 0; z < K; z++){
            // double cur = C3 * (current[t + 1][z + 1] - current[t + 1][z]) + C4 * current[t][z + 1];
            voltage[t + 1][z + 1] = C3 * (current[t + 1][z + 1] - current[t + 1][z]) + C4 * voltage[t][z + 1];

            /*
            if(!std::isinf(voltage[t + 1][z + 1]) && voltage[t + 1][z + 1] > maxVoltage)
                maxVoltage = voltage[t + 1][z + 1];
            else if(!std::isinf(voltage[t + 1][z + 1]) && voltage[t + 1][z + 1] < minVoltage)
                minVoltage = voltage[t + 1][z + 1];
            */
        }

        // std::cout << "t: " << t << std::endl;
    }

    functionData->minVoltage = minVoltage;
    functionData->maxVoltage = maxVoltage;

    functionData->minCurrent = minCurrent;
    functionData->maxCurrent = maxCurrent;


    std::cout << "calculo principal terminado" << std::endl;

    //curto circuito
    for (t = 0; t < N; t++)
    {
        voltage[t][K] = 0;
    }

    std::cout << "calculo terminado" << std::endl;

    return functionData;
}

double getVoltage(functionData_t* functionData,double t, double z, double dt, double dz){

    int nt = t/dt;
    int nz = z/dz;
    
    if(nz <= 50)
        std::cout << "v " << "t: " << t << ", dt: " << dt << ", z: " << z << ", dz: "  << dz << ", nt: " << nt << ",  nz: " << nz << ", value: " << functionData->voltage[nt][nz] << std::endl;
    
    return functionData->voltage[nt][nz];
}

double getCurrent(functionData_t* functionData,double t, double z, double dt, double dz){

    int nt = t/dt;
    int nz = z/dz;
    
    if(nz <= 50)
        std::cout << "c " << "t: " << t << ", dt: " << dt << ", z: " << z << ", dz: "  << dz << ", nt: " << nt << ",  nz: " << nz << ", value: " << functionData->current[nt][nz] << std::endl;
    
    return functionData->current[nt][nz];
}
