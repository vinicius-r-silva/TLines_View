#include "function.h"
#include <iostream>

functionData_t calculateAllValues(float vol, float cur, float dt, int nt, float dz, int nz){
    double C1 = (-2*dt) / (dt*dz*R + 2*dz*L);
    double C2 = (2*L - dt*R) / (2*L + dt*R);
    double C3 = (-2*dt) / (dt*dz*G + 2*dz*C);
    double C4 = (2*C - dt*G) / (2*C + dt*G);

    int t;
    int z;

    double **voltage;
    double **current;

    voltage = new double *[nt + 1];
    current = new double *[nt + 1];

    std::cout << "iniciando alocacao" << std::endl;
    for (t = 0; t < nt + 1; t++)
    {
        voltage[t] = new double[nz + 1];
        current[t] = new double[nz + 1];

        if (voltage[t] == nullptr || current[t] == nullptr)
        {
            std::cout << "sem memoria" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "iniciando calculo" << std::endl;

    //inicial values
    for (t = 0; t < nt + 1; t++)
    {
        voltage[t][0] = 2;
        current[t][0] = 2 / (Z0 + Zl + Rs);
    }

    for (z = 1; z < nz; z++)
    {
        voltage[0][z + 1] = 0;
        current[0][z + 1] = 0;
    }

    std::cout << "parte inicial completa" << std::endl;

    for (t = 1; t < nt; t++)
    {
        for (z = 1; z < nz; z++)
            current[t + 1][z] = C1 * (voltage[t][z + 1] - voltage[t][z - 1]) + C2 * current[t - 1][z];

        for (z = 1; z < nz; z++)
            voltage[t + 1][z + 1] = C3 * (current[t + 1][z + 1] - current[t + 1][z]) + C4 * current[t][z + 1];

        std::cout << "t: " << t << std::endl;
    }

    std::cout << "calculo principal terminado" << std::endl;

    //curto circuito
    for (t = 0; t < nt; t++)
    {
        voltage[t + 1][nz] = 0;
    }

    std::cout << "calculo terminado" << std::endl;

    return {voltage, current};
}

/*
int main(){

    calculateAllValues();

    return 0;
}
*/
