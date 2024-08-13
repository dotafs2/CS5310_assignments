/*
	Bruce A. Maxwell
	Fall 2014

  A quick and dirty visualization of a sphere using phong shading

  Tests the Lighting_shading function over a lot of orientations.

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Module.h"
#include "Light.h"

int main(int argc, char *argv[]) {
  Image *src;
  int i, j;
  float x, y, z;
  Vector N;
  Lighting *l;
  Point p;
  Point V;
  Point lp;
  Color BlueGrey;
  Color Sun;
  Color Cb;
  Color Cs;
  Color c;
  int rows = 500, cols = 500;

	color_set(&BlueGrey, 0.2, 0.25, 0.3);
	color_set(&Sun, 0.9, 0.85, 0.8);
	color_set(&Cb, 0.7, 0.2, 0.1);
	color_set(&Cs, 0.3, 0.3, 0.3);

	point_set(&V, 0.0, 4.0, 0.0, 1.0);
	point_set(&lp, 1.0, 5.0, 1.0, 1.0);

  src = image_create(rows, cols);

  // add an ambient light and a point light, slightly below and to the right of the viewer
  l = lighting_create();
 // lighting_add( l, LightAmbient, &BlueGrey, NULL, NULL, 0.0, 0.0 );
  lighting_add( l, LightPoint, &Sun, NULL, &lp, 0.0, 0.0 );

  // for each pixel in the image
  for(i=0;i<rows;i++) {
	  // the z value is defined by the pixel (faux parallel projections)
	  z = -1 + i * (2.0/(rows-1));
	  for(j=0;j<cols;j++) {
		  // the x value is defined by the pixel (faux parallel projection)
		  x = -1 + j * (2.0/(cols-1));
		  y = 1.0 - x*x - z*z;
		  if( y <= 0.0 )
			  continue;

		  y = sqrt(y);

		  // the surface point and normal vector are the same since the sphere is centered on 0
		  p.val[0] = x;
		  p.val[1] = y;
		  p.val[2] = z;
		  vector_set( &N, x, y, z );
		  lighting_shading( l, &N, &V, &p, &Cb, &Cs, 32, 1, &c);
		  image_setColor( src, i, j, c );
	  }
  }
  image_write( src, "test9d.ppm");

  return(0);
}
