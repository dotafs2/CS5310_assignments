/*
  Bruce A. Maxwell
  Fall 2014

  Example code for drawing a single cube

  C Version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Module.h"
#include "Light.h"


void initPlane(Point **points) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            points[i][j].val[0] = i * 0.1;
            points[i][j].val[1] = 0; // height
            points[i][j].val[2] = j * 0.1;
            points[i][j].val[3] = 1.0;
        }
    }
}

double calculate_wave_height(double x, double z, double t, double y0, int Nw,
                             double *A, double *kx, double *kz, double *omega) {
	double height = -y0;
	for (int i = 0; i < Nw; i++) {
		height += A[i] * cos(kx[i] * x + kz[i] * z - omega[i] * t);
	}
	return height;
}



int main(int argc, char *argv[]) {
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

	color_set( &White, 1.0, 1.0, 1.0 );
	color_set( &Grey, 0.6, 0.62, 0.64 );
	color_set( &DkGrey, 0.1, 0.1, 0.1 );
	color_set(&Blue, 0.0, 0.5, 1.0);
  // initialize the image
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

	double y0 = 0;  // 初始高度偏移量
	int Nw = 3;  // 波的数量
	double A[] = {1.0, 0.5, 0.25};  // 每个波的振幅
	double kx[] = {1.0, 2.0, 3.0};  // 每个波在 x 方向上的波数
	double kz[] = {3, 2, 1};  // 每个波在 z 方向上的波数
	double omega[] = {0.1, 0.2, 0.3}; // 每个波的角频率

	double t = 1;

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
    cube = module_create();
	module_color( cube, &Blue );
	module_translate(cube,-3,0,-1);
	module_bodyColor( cube, &Blue );
	module_surfaceColor( cube, &Blue );

	Point **points = (Point **)malloc(HEIGHT * sizeof(Point *));
	for (int i = 0; i < HEIGHT; ++i) {
		points[i] = (Point *)malloc(WIDTH * sizeof(Point));
	}
	initPlane(points);

	// update height of all waves
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			double x = points[i][j].val[0];
			double z = points[i][j].val[2];
			double y = calculate_wave_height(x * 5, z * 5, t, y0, Nw, A, kx, kz, omega);
			if(y > 2) y = 2;
			if ( y < -1.5) y = -1.5;
			points[i][j].val[1] = y / 5; // Adjust this divisor as needed
		}
	}
	module_scale(cube,0.3,0.3,0.3);
	module_color( cube, &Blue );
	module_translate(cube,0,0,0);
	module_bodyColor( cube, &Blue );
	module_surfaceColor( cube, &Blue );
	module_plane( cube, points);
	// module_rotateZ(cube,0.0,1.0);
	light = lighting_create();
	lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );
	point_copy(&(ds->viewer), &(view.vrp));
	ds->shade = ShadeGouraud;
	//ds->shade = ShadeConstant;
	matrix_identity(&GTM);
	// module_parseLighting(cube,&GTM,light);
	module_draw(cube, &VTM, &GTM, ds, light, src);

  // write out the image
  image_write(src, "test0.ppm");

  // free stuff here
  module_delete( cube );
  image_free( src );


  return(0);
}
