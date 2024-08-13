/*
	Bruce A. Maxwell
	Fall 2014

	Test program for project 9
 */

#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include "Light.h"
#include "plyRead.h"
int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *starfury;
  Module *scene;

  // make it big and shrink the result
  int rows = 2000;
  int cols = 2000;
  int i;

  int nPolygons;
  Polygon *plist;
  Color *clist;

  Color AmbientColor;
  Color PointColor;
  Color PointColor2;
  Color SurfaceColor;

	Point pos;

  Lighting *light;
  DrawState *ds;

  View3D view;

  char buffer[256];

  double angle = 0.0;

	color_set(&AmbientColor, 0.1, 0.1, 0.1);
	color_set(&PointColor, 0.7, 0.6, 0.45);
	color_set(&PointColor2, 0.2, 0.3, 0.45);
	color_set(&SurfaceColor, 0.2, 0.2, 0.2);

    angle = 30;
    angle = angle * M_PI / 180.0;

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 0.0, 0.0, -15.0);
  vector_set(&(view.vpn), 0.0, 0.0, 1.0);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.4;
  view.dv = 1.4;
  view.f = 0.0;
  view.b = 100;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  readPLY("Bunny-LowPoly.ply", &nPolygons, &plist, &clist, 1);

  printf("Read %d polygons\n", nPolygons);

  starfury = module_create();
  module_surfaceColor(starfury, &SurfaceColor);
	module_scale(starfury,0.01,0.01,0.01);
  for(i=0;i < nPolygons;i++) {
	//  printf("%.2f %.2f %.2f\n", clist[i].c[0], clist[i].c[1], clist[i].c[2] );
    module_bodyColor(starfury, &clist[i]);
    module_polygon(starfury, &(plist[i]));
  }


  scene = module_create();
  module_rotateY(scene, cos(angle), sin(angle));
  module_translate(scene, -1.0, -2.0, 0.0);
  module_module(scene, starfury);
  module_translate(scene, 3, 3, 3);
  module_module(scene, starfury);

  light = lighting_create();

  // create one point light
	
	point_set3D( &pos, 0.0, 0.0, -50.0 );
	lighting_add( light, LightPoint, &PointColor, NULL, &pos, 0.0, 0.0 );

  // create another point light
	point_set3D( &pos, 50.0, -20.0, -50.0 );
	lighting_add( light, LightPoint, &PointColor2, NULL, &pos, 0.0, 0.0 );

  // create an ambient light
	lighting_add( light, LightAmbient, &AmbientColor, NULL, NULL, 0.0, 0.0 );

  matrix_identity(&GTM);

  ds = drawstate_create();
  point_copy( &(ds->viewer), &(view.vrp) );
  ds->shade = ShadeGouraud;

  printf("shading frame\n");
  module_draw(scene, &VTM, &GTM, ds, light, src);

  // write out the image
  printf("Writing out high resolution image\n");
  sprintf(buffer, "starfuries.ppm");
  image_write(src, buffer);

	printf("Converting to low resolution image\n");
	sprintf(buffer, "magick starfuries.ppm starfuries.png");
	system(buffer);


  printf("Deleting high-res image\n");
  //  system("rm starfuries.ppm");

  // ought to free stuff here
	image_free(src);

  return(0);
}
