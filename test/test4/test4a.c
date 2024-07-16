/*
  Assignment 4, required image 1

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fsMath.h"
#include "Polygon.h"
#include "Line.h"

/*
  Program to test polygon functionality
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 300;
  const int cols = 400;
  Polygon *p;
  Color Red;
  Color Orange;
  Color White;
  Color Blue;
  Point pt[100];
  int i;
  
  srand(42);

  color_set(&Red, 0.9, 0.2, 0.1 );
  color_set(&Orange, 0.95, 0.7, 0.3 );
  color_set(&White, 1.0, 1.0, 1.0 );
  color_set(&Blue, 0.2, 0.1, 0.95 );

  src = image_create(rows, cols);

  // make a simple square to test proper areas and locations
  // the square ought to be 20x20, include pixel (30,30) and exclude pixel (50, 50)
  point_set2D(&(pt[0]), 30, 30);
  point_set2D(&(pt[1]), 50, 30);
  point_set2D(&(pt[2]), 50, 50);
  point_set2D(&(pt[3]), 30, 50);

  p = polygon_createp(4, pt);
  printf("drawing a square\n");
 //  polygon_drawFill(p, src, Blue);

  // something more interesting
  for(i=0;i<50;i++) {
    float dr = rand() % 20;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(70 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(70 + dr));
    point_print(&pt[i],stdout);
  }
  polygon_set(p, 50, pt);

  printf("drawing first big polygon\n");
  polygon_drawFill(p, src, Red);

  for(i=0;i<50;i++) {
    float dr = rand() % 15;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(50 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(50 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing second big polygon\n");
 // polygon_drawFill(p, src, Orange);

  for(i=0;i<50;i++) {
    float dr = rand() % 10;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(30 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(30 + dr));
  }
 // polygon_set(p, 50, pt);

  printf("drawing third big polygon\n");
 // polygon_drawFill(p, src, White);

  printf("writing output\n");
  image_write(src, "test4a.ppm");

  image_free(src);

  return(0);
}
