/*
  Bruce A. Maxwell
  Fall 2013

  test program for benchmarking the two polygon drawing algorithms:
  scanline fill and barycentric fill.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "graphics.h"

int main(int argc, char *argv[])  {
  const int NPoints = 1002;
  const int NTriangles = 1000;
  const int NPasses = 100;
  const int Rows = 500;
  const int Cols = 500;
  
  // create a bunch of random triangles
  Image *src;
  Point biglist[NPoints];
  Polygon plist[NTriangles];
  Color color[NTriangles];
  int i, t;
  struct timeb tp;
  double start, end;

  for(i=0;i<NPoints;i++) {
    point_set2D( &(biglist[i]), rand() % (Cols-20) + 10, rand() % (Rows-20) + 10 );
  }
  
  for(i=0;i<NTriangles;i++) {
    polygon_init( &(plist[i]) );
    polygon_set( &(plist[i]), 3, &(biglist[i]) );
    color_set( &(color[i]), drand48(), drand48(), drand48() );
  }

  src = image_create( Rows, Cols );

  printf("Starting fillscan\n");
  ftime( &tp );
  start = tp.time + tp.millitm/1000.0;

  for(t=0;t<NPasses;t++) {
    for(i=0;i<NTriangles;i++) {
      polygon_drawFill( &(plist[i]), src, color[i] );
    }
  }

  ftime( &tp );
  end = tp.time + tp.millitm/1000.0;

  printf("Fillscan polygons per second: %.2lf\n", (NPasses*NTriangles) / (end - start) );

  image_write( src, "polygons-fs.ppm");

  printf("Starting barycentric\n");
  ftime( &tp );
  start = tp.time + tp.millitm/1000.0;

  for(t=0;t<NPasses;t++) {
    for(i=0;i<NTriangles;i++) {
      polygon_drawFillB( &(plist[i]), src, color[i] );
    }
  }

  ftime( &tp );
  end = tp.time + tp.millitm/1000.0;

  image_write( src, "polygons-bc.ppm");

  printf("Barycentric polygons per second: %.2lf\n", (NPasses*NTriangles) / (end - start) );

  image_free(src);

  return(0);
}
