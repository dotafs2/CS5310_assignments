//
// Created by DOTAFS on 2024/7/25.
//

#include "WaterSimulation.h"

WaterSimulation::WaterSimulation() {
    points = (Point **)malloc(HEIGHT * sizeof(Point *));
    for (int i = 0; i < HEIGHT; ++i) {
        points[i] = (Point *)malloc(WIDTH * sizeof(Point));
    }
}

double WaterSimulation::calculate_wave_height_SW(double x, double z, double t, double y0, int Nw,
                             double *A, double *kx, double *kz, double *omega) {
    double height = -y0;
    //  double distance = sqrt( x * x  + z * z);  // normal of vector which used in the center of water
    double centerX = WIDTH * 0.5 / 2;
    double centerZ = HEIGHT * 0.5 / 2;
    double distance = sqrt((x - centerX) * (x - centerX) + (z - centerZ) * (z - centerZ));

    for (int i = 0; i < Nw; i++) {
        if(realtime) {
            height += A[i] * cos(kx[i] * x + kz[i] * z - omega[i] * t); // normal simulation
        }
        else {
            height += A[i] * cos(kx[i] * distance - omega[i] * t); // center water simulation
        }
    }
    return height;
}


void WaterSimulation::SinusoidsWaveInit(){
    // initialize the image
    color_set( &White, 1.0, 1.0, 1.0 );
    color_set( &Grey, 0.6, 0.62, 0.64 );
    color_set( &DkGrey, 0.1, 0.1, 0.1 );
   // color_set(&Blue, 0.0, 0.5, 1.0);
    color_set(&Blue, 0.1,0.25,0.35);
    src = image_create(rows, cols);

    // initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);

    // set the View parameters
    point_set3D(&(view.vrp), 3, 4, -5.0);
    vector_set(&(view.vpn), -3, -4, 5);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 1.6;
    view.dv = .9;
    view.f = 0.0;
    view.b = 15;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if(realtime) {
                    points[i][j].val[0] = i * 0.1;
                    points[i][j].val[1] = 0; // height
                    points[i][j].val[2] = j;
                    points[i][j].val[3] = 1.0;
                }
                else {
                    points[i][j].val[0] = i * 0.01;
                    points[i][j].val[1] = 0; // height
                    points[i][j].val[2] = j *0.01;
                    points[i][j].val[3] = 1.0;
                }
            }
        }
    // create drawstate and module
    ds = drawstate_create();
    cube = module_create();
}

Image* WaterSimulation::SinusoidsWave(int t) {
    // reset image pixels and module
    image_reset(src);
    module_clear(cube);
    // update height of all waves
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            double x = points[i][j].val[0];
            double z = points[i][j].val[2];
            double y = 0; // prevent bug
            if(realtime)
                y = calculate_wave_height_SW(x * 5, z * 5, t, y0, Nw, A, kx, kz, omega);
            else
                y = calculate_wave_height_SW(x * 50, z * 50, t, y0, Nw, A, kx, kz, omega);

            points[i][j].val[1] = y/5; // Adjust this divisor as need
        }
    }
    module_scale(cube,1.2,1.2,1.2);
    module_color( cube, &Blue );
    module_translate(cube,-5,-1.5,0);
    module_bodyColor( cube, &Blue );
    module_surfaceColor( cube, &Blue );
    module_plane( cube, points, HEIGHT, WIDTH);
   // module_rotateZ(cube,0.0,1.0);
    light = lighting_create();
    lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );
    point_copy(&(ds->viewer), &(view.vrp));
    ds->shade = ShadeGouraud;
    //ds->shade = ShadeConstant;
    matrix_identity(&GTM);
    // module_parseLighting(cube,&GTM,light);
    module_draw(cube, &VTM, &GTM, ds, light, src);

    //char filename[50];
   // snprintf(filename, sizeof(filename), "water_%d.ppm", t);
   // image_write(src, filename);
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return src;
}


void WaterSimulation::update_viewPosition(float x, float z) {
    view.vrp.val[0] += x;
    view.vrp.val[2] += z;
    matrix_setView3D(&VTM, &view);
}