/*
  Assignment 4, required image 2

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

/*
  Program to test polygon functionality using barycentric coordinates
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 100;
  const int cols = 100;
  Polygon *p;
  Color Red;
  Color White;
  Color Blue;
  Point pt[100];

  color_set(&Red, 0.9, 0.2, 0.1 );
  color_set(&White, 1.0, 1.0, 1.0 );
  color_set(&Blue, 0.2, 0.1, 0.95 );
  
  src = image_create(rows, cols);

  // make a simple square to test proper areas and locations
  // the square ought to be 20x20, include pixel (30,30) and exclude pixel (50, 50)
  point_set2D(&(pt[0]), 30, 30);
  point_set2D(&(pt[1]), 50, 30);
  point_set2D(&(pt[2]), 50, 50);
  point_set2D(&(pt[3]), 30, 50);
  point_set2D(&(pt[4]), 30, 30);

  p = polygon_createp(3, pt);

  printf("drawing polygon 1\n");
  polygon_drawFillB(p, src, Blue);

  polygon_set(p, 3, &(pt[2]) );

  printf("drawing polygon 2\n");
  polygon_drawFillB(p, src, Red);

  point_set2D(&(pt[5]), 60, 20);
  point_set2D(&(pt[6]), 80, 85);
  point_set2D(&(pt[7]), 50, 70);
  polygon_set(p, 3, &(pt[5]));

  printf("drawing polygon 3\n");
  polygon_drawFillB(p, src, White);

  point_set2D(&(pt[8]), 5, 5);
  point_set2D(&(pt[9]), 25, 5);
  point_set2D(&(pt[10]), 25, 25);
  point_set2D(&(pt[11]), 5, 25);
  point_set2D(&(pt[12]), 5, 5);

  polygon_set(p, 3, &(pt[10]) );

  printf("drawing polygon 4\n");
  polygon_drawFillB(p, src, Red);

  polygon_set(p, 3, &(pt[8]));

  printf("drawing polygon 5\n");
  polygon_drawFillB(p, src, Blue);


  printf("writing output\n");
  image_write(src, "test4b.ppm");

  image_free(src);

  return(0);
}
