//
// Created by DOTAFS on 2024/7/25.
//

#ifndef WATERSIMULATION_H
#define WATERSIMULATION_H
#include "Module.h"
#include "Definition.h"
#include "WaterSimulation.h"
#include "Particle.h"
#include <ctime>
#include <vector>
#include "perlinNoise.h"
#define realtime 0
#if realtime
#define HEIGHT 50
#define WIDTH 50
#else
#define HEIGHT 500
#define WIDTH 500
#endif



class WaterSimulation {
public:
    WaterSimulation();
    void SinusoidsWaveInit();
    Image* SinusoidsWave(int t);
    void update_viewPosition(float x, float z);
    void PBFInit();
    Image* PBF(float t);
    Image* TexturePlayGround(float t);
    void TexturePlayGroundInit();


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
    Color Red;
    Color WaterBlue;
    DrawState *ds;
    View3D view;
    Point **points;
    Point **checkboardpoints;
    std::vector<Point> rainPoints;
    Lighting *light;

#if realtime
        // sinusoildsWave header
        double y0 = 0;  // 初始高度偏移量
        int Nw = 3;  // 波的数量
        double A[3] = {1.0, 0.5, 0.25};  // 每个波的振幅
        double kx[3] = {1.0, 2.0, 3.0};  // 每个波在 x 方向上的波数
        double kz[3] = {3, 2, 1};  // 每个波在 z 方向上的波数
        double omega[3] = {0.2, 0.2, 0.2}; // 每个波的角频率
#else

    // sinusoildsWave header
    double y0 = 0;  // 初始高度偏移量
    int Nw = 3;  // 波的数量
    double A[4] = {0.5, 0.5, 0.5,0.7};  // 每个波的振幅
    double kx[4] = {0.2, 0.2, 0.2,0.2};  // 每个波在 x 方向上的波数
    double kz[4] = {0.1, 0.1,0.1,0.1};  // 每个波在 z 方向上的波数
    double omega[4] = {0.1, 0.1, 0.1,0.1}; // 每个波的角频率
#endif


    int particleCount = PARTICLECOUNT;
    Particle *particles;
    Particle *singleParticle;
    Bin ***grid;
    std::vector<Fog> fogs;

};


#endif //WATERSIMULATION_H
