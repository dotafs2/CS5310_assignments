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

int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *cube;
  int rows = 360;
  int cols = 640;

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
  point_set3D(&(view.vrp), 5, 5, -7.0);
  vector_set(&(view.vpn), -5, -5, 7);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = .9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

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

  module_plane( cube, points);


  light = lighting_create();
  lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );

  ds = drawstate_create();

  point_copy(&(ds->viewer), &(view.vrp));
	//ds->shade = ShadeGouraud;
		ds->shade = ShadeConstant;

  matrix_identity(&GTM);
  module_draw(cube, &VTM, &GTM, ds, light, src);

  // write out the image
  image_write(src, "test0.ppm");

  // free stuff here
  module_delete( cube );
  image_free( src );


  return(0);
}
