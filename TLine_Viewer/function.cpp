#include "function.h"
#include <iostream>

functionData_t* allocMemory(int vol, int res, float dt, int nt, float dz, int nz){

    int K = nz/dz;
    int N = nt/dt;

    int t;
    int z;

    double **voltage;
    double **current;

    functionData_t* functionData = new functionData_t;

    voltage = new double *[N + 1];
    current = new double *[N + 1];

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

    functionData->voltage = voltage;
    functionData->current = current;

    return calculateAllValues(functionData, vol, res, dt, nt, dz, nz);

}


functionData_t* calculateAllValues(functionData_t* functionData, int vol, int res, float dt, int nt, float dz, int nz){
    int K = nz/dz;
    int N = nt/dt;

    double C1 = (-2*dt) / (dt*dz*_R + 2*dz*L);
    double C2 = (2*L - dt*_R) / (2*L + dt*_R);
    double C3 = (-2*dt) / (dt*dz*_G + 2*dz*C);
    double C4 = (2*C - dt*_G) / (2*C + dt*_G);

    int t;
    int z;

    double **voltage = functionData->voltage;
    double **current = functionData->current;

    double minVoltage = 0;
    double maxVoltage = 2;

    double minCurrent = 0;
    double maxCurrent = 2 / (Z0 + Zl + Rs);


    std::cout << "iniciando calculo" << std::endl;

    //inicial values
    for (t = 0; t < N + 1; t++)
    {
        voltage[t][0] = 2;
        current[t][0] = 2 / (Z0 + Zl + Rs);
    }

    for (z = 1; z < K; z++)
    {
        voltage[0][z + 1] = 0;
        current[0][z + 1] = 0;
    }

    std::cout << "parte inicial completa" << std::endl;

    for (t = 1; t < N; t++)
    {
        for (z = 1; z < K; z++){
            current[t + 1][z] = C1 * (voltage[t][z + 1] - voltage[t][z - 1]) + C2 * current[t - 1][z];
            
            if(current[t + 1][z] > maxCurrent)
                maxCurrent = current[t + 1][z];
            else if(current[t + 1][z] < minCurrent)
                minCurrent = current[t + 1][z];

        }

        for (z = 1; z < K; z++){
            voltage[t + 1][z + 1] = C3 * (current[t + 1][z + 1] - current[t + 1][z]) + C4 * current[t][z + 1];

            if(voltage[t + 1][z + 1] > maxVoltage)
                maxVoltage = voltage[t + 1][z + 1];
            else if(voltage[t + 1][z + 1] < minVoltage)
                minVoltage = voltage[t + 1][z + 1];
        }

        //std::cout << "t: " << t << std::endl;
    }

    functionData->minVoltage = minVoltage;
    functionData->maxVoltage = maxVoltage;

    functionData->minCurrent = minCurrent;
    functionData->maxCurrent = maxCurrent;


    std::cout << "calculo principal terminado" << std::endl;

    //curto circuito
    for (t = 0; t < N; t++)
    {
        voltage[t + 1][K] = 0;
    }

    std::cout << "calculo terminado" << std::endl;

    return functionData;
}

double getVoltage(functionData_t* functionData,double t, double z, float dt, float dz){

    int nt = t/dt;
    int nz = z/dz;
    
    return functionData->voltage[nt][nz];
}

double getCurrent(functionData_t* functionData,double t, double z, float dt, float dz){

    int nt = t/dt;
    int nz = z/dz;

    return functionData->current[nt][nz];
}

/*
int main(){

    calculateAllValues();

    return 0;
}
*/
