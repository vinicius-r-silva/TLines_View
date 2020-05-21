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

    //const double C1 = (-2.0*dt) / (dt*dz*_R + 2*dz*L);
    //const double C2 = (2.0*L - dt*_R) / (2*L + dt*_R);
    //const double C3 = (-2.0*dt) / (dt*dz*_G + 2*dz*C);
    //const double C4 = (2.0*C - dt*_G) / (2*C + dt*_G);

    const double C1 = -dt / (dz*L);
    const double C2 = 1;
    const double C3 = -dt / (dz*C);
    const double C4 = 1;


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
    std::cout << "_REFLECTION_LOAD: "<< _REFLECTION_LOAD << std::endl;
    std::cout << "_REFLECTION_SOURCE: "<< _REFLECTION_SOURCE << std::endl;

    int t;
    int z;

    double **voltage = FData->voltage;
    double **current = FData->current;

    double minVoltage = 0;
    double maxVoltage = 2;

    double minCurrent = 0;
    double maxCurrent = 2.0 / (Z0 + Zl + Rs);


    std::cout << "iniciando calculo" << std::endl;

    for (t = 0; t < N + 1; t++)
    {
        for (z = 0; z < K+1; z++){
            current[t][z] = 0;
            voltage[t][z] = 0;
        }
    }

    //inicial values
    voltage[0][0] = 2.0 * Z0/(Z0+Rs);
    current[0][0] = 0;//2.0 / (Z0 + Zl + Rs);

    
    for (z = 1; z < K + 1; z++)
    {
        voltage[0][z] = 0;
        current[0][z] = 0;
    }
    
    std::cout << "parte inicial completa" << std::endl;

    //  C1 = -0.01998
    //  C2 = 1;
    //  C3 = -49.95
    //  C4 = 1;
    int powerL = 0;
    int powerS = 0;

    for (t = 0; t < N; t++)
    {
        for (z = 0; z < K; z++){
            current[t + 1][z] = C1 * (voltage[t][z + 1] - voltage[t][z]) + C2 * current[t][z];
        }

        for (z = 0; z < K - 1; z++){
            voltage[t + 1][z + 1] = C3 * (current[t + 1][z + 1] - current[t + 1][z]) + C4 * voltage[t][z + 1];
        }
            
        // boundary conditions
        voltage[t + 1][K] = voltage[t][K-1];

        if(res == ZERO){
            voltage[t+1][K-1] = 0;
            voltage[t+1][0] = voltage[t][1];
        }else
            voltage[t + 1][0] = voltage[t][1] + (voltage[t+1][1]-voltage[t][0])*(Vph*dt-dz)/(Vph*dt+dz);

        //reflection part
        if(!((t+1) % K) && (t+1) % (2*K)){
            powerL++;
            voltage[t + 1][K-1] += voltage[t][K-2] * (powf64(_REFLECTION_LOAD, powerL) * powf64(_REFLECTION_SOURCE, powerS));
            voltage[t + 1][K] = voltage[t + 1][K-1];
            
        }
        
        if(!((t+1) % (2*K))){
            powerS++;
            voltage[t + 1][0] += voltage[t][1] * powf64(_REFLECTION_LOAD, powerL) * powf64(_REFLECTION_SOURCE, powerS);
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

    static double debugT = -1;
    static double debugZ = -1;

    int nt = t/dt;
    int nz = z/dz;
    
//     if(debugT != nt){
//         debugT = nt;

//        if(nz <= 50);
//            std::cout << "v " << "t: " << t << ", dt: " << dt << ", z: " << z << ", dz: "  << dz << ", nt: " << nt << ",  nz: " << nz << ", value: " << FData->voltage[nt][nz] << std::endl;

//     }

//     if(debugZ != nz){
//         debugZ = nz;

// //        if(nz <= 50);
// //            std::cout << "v " << "t: " << t << ", dt: " << dt << ", z: " << z << ", dz: "  << dz << ", nt: " << nt << ",  nz: " << nz << ", value: " << FData->voltage[nt][nz] << std::endl;

//     }
    return FData->voltage[nt][nz];
}

double getCurrent(functionData_t* FData, double t, double z, double dt, double dz){
    
    static double debugT = -1;
    static double debugZ = -1;

    int nt = t/dt;
    int nz = z/dz;
    
//     if(debugT != nt){
//         debugT = nt;

//         //if(nz <= 50)
//          //   std::cout << "c " << "t: " << t << ", dt: " << dt << ", z: " << z << ", dz: "  << dz << ", nt: " << nt << ",  nz: " << nz << ", value: " << FData->current[nt][nz] << std::endl;
//     }

//     if(debugZ != nz){
//         debugZ = nz;

// //        if(nz <= 50)
// //            std::cout << "c " << "t: " << t << ", dt: " << dt << ", z: " << z << ", dz: "  << dz << ", nt: " << nt << ",  nz: " << nz << ", value: " << FData->current[nt][nz] << std::endl;

//     }
    
    return FData->current[nt][nz];
}
