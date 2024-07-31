//
// Created by DOTAFS on 2024/7/25.
//

#ifndef WATERSIMULATION_H
#define WATERSIMULATION_H
#include "Module.h"
#include "Definition.h"
#include "WaterSimulation.h"
class WaterSimulation {
public:
    WaterSimulation();
    void SinusoidsWaveInit();
    Image* SinusoidsWave(int t);
    void update_viewPosition(float x, float z);




private:
    double calculate_wave_height_SW(double x, double z, double t, double y0, int Nw,
                                 double *A, double *kx, double *kz, double *omega);

    // command header
    Image *src;
    Matrix VTM;
    Matrix GTM;
    Module *cube;
    int rows = 720;
    int cols = 1280;

    Color White;
    Color Grey;
    Color DkGrey;
    Color Blue;
    DrawState *ds;
    View3D view;

    Lighting *light;


    // sinusoildsWave header
    double y0 = 0;  // 初始高度偏移量
    int Nw = 3;  // 波的数量
    double A[3] = {1.0, 0.5, 0.25};  // 每个波的振幅
    double kx[3] = {1.0, 2.0, 3.0};  // 每个波在 x 方向上的波数
    double kz[3] = {3, 2, 1};  // 每个波在 z 方向上的波数
    double omega[3] = {0.2, 0.2, 0.2}; // 每个波的角频率

    Point **points;
};


#endif //WATERSIMULATION_H
