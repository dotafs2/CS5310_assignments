#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <math.h>
#include "ppmIO.h"

/* put your include file here */
#include "line.h"
#include "image.h"

#define NUMLINES 200
#define TIMING 5
#define ROWS 200
#define COLS 200

/*
  This C program draw lines for 5 seconds (set above in TIMING
  constant).  It executes this by repeatedly drawing NUMLINES (set
  above) lines which are randomly selected (both color and position).

  This final image is written out to the file lines.ppm.  */

int main(int argc, char *argv[]) {
  Point start[NUMLINES], end[NUMLINES];
  Color color[NUMLINES];
  Image *src;
  Line line;
  long i,j;
  Color white;
  double tstart, tend;
  long numLines;
  double sum, dx, dy;
  struct timeb tp;

	color_set( &white, 1.0, 1.0, 1.0);

  // allocate an image ROWS by COLS
  src = image_create(ROWS, COLS);
  if(!src) {
    fprintf(stderr, "unable to allocate memory\n");
    exit(0);
  }

  // Initialize the image to all white
  for(i=0;i<src->rows;i++) {
    for(j=0;j<src->cols;j++) {
      image_setColor(src, i, j, white );
    }
  }

  // Pre-calculate the line endpoints and colors so we don't have to
  // call the random() function inside the main drawing loop.
  sum = 0.0;
  for(i=0;i<NUMLINES;i++) {
    int tsx, tsy, tex, tey;
    tsx = (int)(drand48() * COLS);
    tsy = (int)(drand48() * ROWS);
    tex = (int)(drand48() * COLS);
    tey = (int)(drand48() * ROWS);
    point_set2D(&(start[i]), tsx, tsy );
    point_set2D(&(end[i]), tex, tey );
    color[i].c[0] = drand48();
    color[i].c[1] = drand48();
    color[i].c[2] = drand48();

    dx = tsx - tex;
    dy = tsy - tey;
    sum += sqrt(dx * dx + dy * dy);
  }
  sum /= NUMLINES;
  printf("Average line length = %.1lf\n", sum);

  // Start drawing lines
  ftime( &tp );
  tstart = tp.time + tp.millitm / 1000.0;

  for(i=0,numLines=0; numLines < 5000000;numLines++, i++) {
    i = i % NUMLINES;
    line_set(&line, start[i], end[i]);
    line_draw(&line, src, color[i]);
  }

  ftime( &tp );
  tend = tp.time + tp.millitm / 1000.0;

  // print out the result
  printf("%.2lf lines per second\n", numLines / (tend - tstart) );

  // write out the image
  image_write(src, "lines.ppm");

  // free the image data
  image_free(src);

  return(0);
}

