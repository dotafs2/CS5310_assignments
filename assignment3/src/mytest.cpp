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

    src = image_create( 400, 400 );

    for ( int y = 0 ; y <= 100; y ++){
        for(int x = 0; x <= 100; x++){
            Point sb;
            float xSegment = (float) x/ (float) 100;
            float ySegment = (float) y/ (float) 100;
            float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            float yPos = cos(ySegment * M_PI);
            float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            sb.val[0] = xPos*100 + 200;
            sb.val[1] = yPos*100 + 200;
            sb.val[2] = zPos*100 + 200;
            sb.val[3] = 1;
            point_draw(&sb,src,White);
        }
    }
   point_set2D( &p, 200, 200);
   // circle_set( &circ, p, 80 );
   // circle_draw( &circ, src, White );
   // SSAA(src,4);
    image_write( src, "mytest2.ppm" );

    image_free( src);

    return(0);
}
