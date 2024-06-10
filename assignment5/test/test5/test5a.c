/*
	Bruce A. Maxwell
	Fall 2014
  Assignment 5, matrix test program

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

/*
  Program to test matrix library functionality
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 600;
  const int cols = 800;
  const int Resolution = 50;
  Color Grey;
  Color dkGrey;
  Color Red;
  Color Blue;
  Point unitCircle[Resolution];
  Point unitSquare[4];
  Point pt[Resolution];
  Point ptt[Resolution];
  int i, index = 0;
  Matrix VTM, GTM, LTM;
  Polygon *ship[50];
  Color shipColor[50];
  double theta = 0.0;
  double phaserAngle = 0.0;
  int firePhase = 0;

	color_set(&Grey, 180/255.0, 180/255.0, 183/255.0);
	color_set(&dkGrey, 140/255.0, 140/255.0, 143/255.0);
	color_set(&Red, 250/255.0, 40/255.0, 40/255.0);
	color_set(&Blue, 30/255.0, 20/255.0, 250/255.0);

  if(argc > 1) {
    theta = atoi(argv[1]);
  }
  printf("Drawing ship with orientation %.2f degrees\n", theta);

  if(argc > 2) {
    phaserAngle = atoi(argv[2]);
    firePhase = 1;

    printf("Drawing phaser with angle %.2f degrees\n", phaserAngle);
  }

  srand(42);

  src = image_create(rows, cols);

  // initialize the three matrices
  matrix_identity(&VTM);
  matrix_identity(&GTM);
  matrix_identity(&LTM);

  // Fix world coordinates as normal (x, y) 

  // give the view window an origin at -180m, -150m
  // size is a 4x3 ratio
  // VTM = T(0, rows-1)S(cols/vx, rows/vy)T(180, 150)
  matrix_translate2D(&VTM, 120, 100);
  matrix_scale2D(&VTM, cols/(4*60), -rows/(3*60));
  matrix_translate2D(&VTM, 0, rows-1);
  printf("VTM\n");
  matrix_print(&VTM, stdout);

  // make a space ship oriented along the positive X axis
  // use the LTM to move simple primitives into place
  // use the GTM to rotate the ship
  // use the VTM to change the view

  // make a list of points that form the unit circle
  for(i=0;i<Resolution;i++) {
    point_set2D(&(unitCircle[i]), 
		cos( i * 2.0 * M_PI / (float)Resolution), 
		sin( i * 2.0 * M_PI / (float)Resolution));
  }
  // set up the unit square
  point_set2D(&(unitSquare[0]), 0, 0);
  point_set2D(&(unitSquare[1]), 1, 0);
  point_set2D(&(unitSquare[2]), 1, 1);
  point_set2D(&(unitSquare[3]), 0, 1);

  // build a set of polygons that form the ship in model space
  // put the origin of the model between the engines

  // outline for the main disk
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 31, 31);
  // move it 20m along the X-axis
  matrix_translate2D(&LTM, 60, 0);
  // transform the circle points using LTM
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(unitCircle[i]), &(pt[i]));
  }

  // add the polygon
  matrix_print(&LTM, stdout);
  ship[index] = polygon_createp(Resolution, pt);
  shipColor[index++] = Red;

  printf("Post-LTM\n");
  polygon_print(ship[0], stdout);

  // main disk
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 30, 30);
  // move it 20m along the X-axis
  matrix_translate2D(&LTM, 60, 0);
  // transform the circle points using LTM
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(unitCircle[i]), &(pt[i]));
  }

  // add the polygon
  matrix_print(&LTM, stdout);
  ship[index] = polygon_createp(Resolution, pt);
  shipColor[index++] = Grey;

  // central bridge disk
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 10, 10);
  // move it 20m along the X-axis
  matrix_translate2D(&LTM, 60, 0);
  // transform the circle points using LTM
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(unitCircle[i]), &(pt[i]));
  }

  // add the polygon
  matrix_print(&LTM, stdout);
  ship[index] = polygon_createp(Resolution, pt);
  shipColor[index++] = dkGrey;

  // make the body disk elongated along the X axis
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 30, 12);
  matrix_translate2D(&LTM, 2.5, 0);

  // transform the circle points using LTM
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(unitCircle[i]), &(pt[i]));
  }

  // add the polygon
  matrix_print(&LTM, stdout);
  ship[index] = polygon_createp(Resolution, pt);
  shipColor[index++] = Grey;
  
  // make a trapezoidal strut out of the unit square
  matrix_identity(&LTM);
  matrix_translate2D(&LTM, -0.5, 0.0);
  matrix_scale2D(&LTM, 10, 10);
  matrix_shear2D(&LTM, .2, 0.0);

  for(i=0;i<4;i++) {
    matrix_xformPoint(&LTM, &(unitSquare[i]), &(pt[i]));
  }

  // move the strut out from the origin along the Y axis
  matrix_identity(&LTM);
  matrix_translate2D(&LTM, 0, 12);

  for(i=0;i<4;i++) {
    matrix_xformPoint(&LTM, &(pt[i]), &(ptt[i]));
  }

  // add the polygon
  matrix_print(&LTM, stdout);
  ship[index] = polygon_createp(4, ptt);
  shipColor[index++] = Grey;

  // place the second strut
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 1, -1);
  matrix_translate2D(&LTM, 0, -12);

  for(i=0;i<4;i++) {
    matrix_xformPoint(&LTM, &(pt[i]), &(ptt[i]));
  }

  // add the polygon
  ship[index] = polygon_createp(4, ptt);
  shipColor[index++] = Grey;

  // create an engine outline from the unit circle
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 31, 6);

  // make the engine
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(unitCircle[i]), &(pt[i]));
  }

  // send one engine to the right location
  matrix_identity(&LTM);
  matrix_translate2D(&LTM, -5, 27);

  // move the engine
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(pt[i]), &(ptt[i]));
  }

  // add the polygon
  ship[index] = polygon_createp(Resolution, ptt);
  shipColor[index++] = Blue;

  // send the other engine to the right location
  matrix_identity(&LTM);
  matrix_translate2D(&LTM, -5, -27);

  // move the engine
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(pt[i]), &(ptt[i]));
  }

  // add the polygon
  ship[index] = polygon_createp(Resolution, ptt);
  shipColor[index++] = Blue;

  // create an engine
  matrix_identity(&LTM);
  matrix_scale2D(&LTM, 30, 5);

  // make the engine
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(unitCircle[i]), &(pt[i]));
  }

  // send one engine to the right location
  matrix_identity(&LTM);
  matrix_translate2D(&LTM, -5, 27);

  // move the engine
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(pt[i]), &(ptt[i]));
  }

  // add the polygon
  ship[index] = polygon_createp(Resolution, ptt);
  shipColor[index++] = Grey;

  // send the other engine to the right location
  matrix_identity(&LTM);
  matrix_translate2D(&LTM, -5, -27);

  // move the engine
  for(i=0;i<Resolution;i++) {
    matrix_xformPoint(&LTM, &(pt[i]), &(ptt[i]));
  }

  // add the polygon
  ship[index] = polygon_createp(Resolution, ptt);
  shipColor[index++] = Grey;

  // set up the phaser
  if(firePhase) {
    matrix_identity(&LTM);

    matrix_scale2D(&LTM, 100, 2);

    // orient the phaser
    matrix_rotateZ(&LTM, cos(phaserAngle*M_PI/180.0), sin(phaserAngle*M_PI/180.0));
    
    // translate it to the center of the disk and out
    matrix_translate2D(&LTM, 
		       60 + 30 * cos(phaserAngle*M_PI/180.0), 
		       30 * sin(phaserAngle*M_PI/180.0) );

    // use the unit square
    for(i=0;i<4;i++) {
      matrix_xformPoint(&LTM, &(unitSquare[i]), &(pt[i]));
    }
    
    // add the polygon
    ship[index] = polygon_createp(4, pt);
    shipColor[index++] = Red;
  }

  matrix_rotateZ(&GTM, cos(theta*M_PI/180.0), sin(theta*M_PI/180.0));

  printf("GTM:\n");
  matrix_print(&GTM, stdout);

  printf("Pre-GTM/VTM\n");
  polygon_print(ship[0], stdout);

  for(i=0;i<index;i++) {

    // multiply the polygon by the global transform matrix
    matrix_xformPolygon(&GTM, ship[i]);
    if(i==0) {
      printf("Pre-VTM\n");
      polygon_print(ship[i], stdout);
    }

    // multiply the polygon by the view transformation matrix
    matrix_xformPolygon(&VTM, ship[i]);

    if(i==0) {
      printf("Pre-draw\n");
      polygon_print(ship[i], stdout);
    }

    // draw the polygon
    polygon_drawFill(ship[i], src, shipColor[i]);
  }

  image_write(src, "test5a.ppm");


  image_free(src);

  return(0);
}
