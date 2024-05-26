/*
  Bruce A. Maxwell
  Fall 2014

  A program to test the polyline function
*/
#include <stdio.h>
#include <stdlib.h>
#include "Line.h"
#include "Image.h"
#include "anti-alias.h"

// draw some random lines, then two boxes
int main(int argc, char *argv[]) {
  const int nPoints = 1000;

  Polyline thing1;
  Polyline *thing2;
  Point p[nPoints];
  int i;
  int rows = 500;
  int cols = 500;
  Image *src;
  Color Blue;
  Color Red;
  Color Green;

	srand48(0);

  color_set( &Blue, 0.1, 0.15, 0.7 );
  color_set( &Red, 0.8, 0.2, 0.1 );
  color_set( &Green, 0.2, 0.6, 0.2 );
  
  printf("Startup\n");
  for(i=0;i<nPoints;i++) {
    point_set2D( &(p[i]), drand48()*cols/2, drand48()*rows/2 );
  }

  // init is necessary, because otherwise the fields are full of garbage
  printf("Polyline init and set\n");
  polyline_init( &thing1 );
  polyline_set( &thing1, 20, &( p[42] ) );

  printf("Polyline create\n");
	thing2 = polyline_createp( 10, &(p[105] ) );
	//  thing2 = polyline_createp( 2, &(p[105] ) );
	//	polyline_print(thing2, stdout);
  
  printf("Creating image\n");
  src = image_create( rows, cols );

  printf("Drawing poly-lines\n");
  // draw some random lines
  polyline_draw( &thing1, src, Blue );
  polyline_draw( thing2, src, Red );

  // draw a box going counter-clockwise, should be ok
  point_set2D( &(p[500]), 50, 300 );
  point_set2D( &(p[501]), 50, 350 );
  point_set2D( &(p[502]), 100, 350 );
  point_set2D( &(p[503]), 100, 300 );
  point_set2D( &(p[504]), 50, 300 );

  printf("Counter-clockwise box\n");
  polyline_set( thing2, 5, &(p[500]) );
  polyline_draw( thing2, src, Green );

  // draw a box going clockwise, should not be ok
  point_set2D( &(p[500]), 350, 300 );
  point_set2D( &(p[501]), 400, 300 );
  point_set2D( &(p[502]), 400, 350 );
  point_set2D( &(p[503]), 350, 350 );
  point_set2D( &(p[504]), 350, 300 );

  printf("Clockwise box\n");
  polyline_set( thing2, 5, &(p[500]) );
  polyline_draw( thing2, src, Green );
  SSAA(src,4);
  image_write( src, "test3c.ppm");

  printf("Cleanup\n");
  polyline_clear( &thing1 );
  polyline_free( thing2 );
  image_free( src );

  return(0);
}


