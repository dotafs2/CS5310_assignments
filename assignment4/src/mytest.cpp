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
#include "ball.h"
/*
  Bruce A. Maxwell
  Fall 2014

  Test function 1 for the graphics primitives assignment
  Enterpoop
 */

// draw a few boxes, a circle, and some lines.
int main(int argc, char *argv[]) {
    Image *src;
    Color White;
    Point p;
    Circle circ;


    color_set( &White, 1.0, 1.0, 1.0 );

    src = image_create( 800, 800 );

    float scale = 200.0f;
    Color red = { 1.0, 0.0, 0.0 };

   point_set2D( &p, 200, 200);
   Point center;
    center.val[0] = 400;
    center.val[1] = 400;
    center.val[2] = 0;
    center.val[3] = 1;
    bool fill = false;
   draw_ball(src,20,20,center,scale,White,fill);
   //circle_set( &circ, p, 80 );
   // circle_draw( &circ, src, White );
   SSAA(src,8);
    image_write( src, "mytest2.ppm" );

    image_free( src);

    return(0);
}
