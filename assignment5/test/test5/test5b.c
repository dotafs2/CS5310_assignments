/*
  Bruce Maxwell
  Fall 2014

  2D View test function
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

void setWhite( Image *src );
void setWhite( Image *src ) {
  int i, j;
  Color White;

  color_set(&White, 1.0, 1.0, 1.0);

  for(i=0;i<src->rows;i++) {
    for(j=0;j<src->cols;j++) {
      image_setColor( src, i, j, White );
    }
  }
}

int main(int argc, char *argv[]) {
  const int nLines = 50;
  const int nFrames = 50;
  const int rows = 180;
  const int cols = 320;
  View2D view;
  Matrix vtm;
  Matrix ltm;
  Line line[nLines];
  Line tline;
  float orient[nLines];
  float freq[nLines];
  Color color[nLines];
  int i, t;
  char filename[256];
  Image *src;

  src = image_create( rows, cols );

  srand48(0x01234ABCD);

  // make a bunch of random lines in the [0,1] square
  for(i=0;i<nLines;i++) {
    line_set2D( &(line[i]), drand48(), drand48(), drand48(), drand48() );
    orient[i] = drand48()*M_PI*2;
    freq[i] = 0.25 * drand48();
    color_set( &(color[i]), drand48(), drand48(), drand48() );
  }

  // set up a view centered on (1.5, 1.5) with x pointing right
  point_set2D( &(view.vrp), 1.8, 1.8 );
  view.dx = 1.0;
  vector_set( &(view.x), 1.0, 0.0, 0.0 );
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView2D( &vtm, &view );
  printf("vtm:\n");
  matrix_print(&vtm, stdout);

  // loop for some number of frames
  for(t=0;t<nFrames;t++) {
    
    setWhite( src );

    // rotate the lines about one end
    for(i=0;i<nLines;i++) {
      float angle = orient[i] + freq[i]*2*M_PI*t/nFrames;
      tline = line[i];
      printf("line (%.2f %.2f) (%.2f %.2f)\n", line[i].a.val[0], line[i].a.val[1], 
             line[i].b.val[0], line[i].b.val[1] );

      matrix_identity( &ltm );
      matrix_translate2D( &ltm, -tline.a.val[0], -tline.a.val[1] );
      matrix_rotateZ( &ltm, cos(angle), sin(angle) );
      matrix_translate2D( &ltm, tline.a.val[0], tline.a.val[1] );
      matrix_print(&ltm, stdout);

      matrix_xformLine( &ltm, &tline );
      printf("line (%.2f %.2f) (%.2f %.2f)\n", tline.a.val[0], tline.a.val[1], 
             tline.b.val[0], tline.b.val[1] );
      matrix_xformLine( &vtm, &tline );
      printf("line (%.2f %.2f) (%.2f %.2f)\n", tline.a.val[0], tline.a.val[1], 
             tline.b.val[0], tline.b.val[1] );
      
      line_draw( &tline, src, color[i] );
    }

    printf("writing file\n");
    sprintf(filename, "frame-%04d.ppm", t );
    image_write( src, filename );

    // translate the view across the scene
    point_set2D( &(view.vrp), 1.8 - 2.4*(t+1)/nFrames, 1.8 - 2.4*(t+1)/nFrames );
    matrix_setView2D( &vtm, &view );
    matrix_print( &vtm, stdout );
  }

  // cleanup
  image_free( src );

  return(0);
}
