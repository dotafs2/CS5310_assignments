/*
  Bruce Maxwell
  Fall 2014

  Test function for 3D matrix viewing
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fsMath.h"
#include "Polygon.h"
#include "Line.h"

int main(int argc, char *argv[]) {
    const int rows = 180;
    const int cols = 320;
    const int numFrames = 50; // Number of frames for the animation
    char filename[50];

    View3D view;
    Matrix vtm;
    Polygon side[6];
    Polygon tpoly;
    Point  tv[4];
    Point  v[8];
    Color  color[6];
    Image *src;
    int i, frame;

    // set some colors
    color_set( &color[0], 0, 0, 1 );
    color_set( &color[1], 0, 1, 0 );
    color_set( &color[2], 1, 0, 0 );
    color_set( &color[3], 1, 0, 1 );
    color_set( &color[4], 0, 1, 1 );
    color_set( &color[5], 1, 1, 0 );

    // initialize polygons
    for(i=0;i<6;i++) {
        polygon_init( &side[i] );
    }

    // corners of a cube, centered at (0, 0, 0)
    point_set( &v[0], -1, -1, -1, 1 );
    point_set( &v[1],  1, -1, -1, 1 );
    point_set( &v[2],  1,  1, -1, 1 );
    point_set( &v[3], -1,  1, -1, 1 );
    point_set( &v[4], -1, -1,  1, 1 );
    point_set( &v[5],  1, -1,  1, 1 );
    point_set( &v[6],  1,  1,  1, 1 );
    point_set( &v[7], -1,  1,  1, 1 );

    // front side
    polygon_set( &side[0], 4, &(v[0]) );

    // back side
    polygon_set( &side[1], 4, &(v[4]) );

    // top side
    point_copy( &tv[0], &v[2] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[6] );

    polygon_set( &side[2], 4, tv );

    // bottom side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[1] );
    point_copy( &tv[2], &v[5] );
    point_copy( &tv[3], &v[4] );

    polygon_set( &side[3], 4, tv );

    // left side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[4] );

    polygon_set( &side[4], 4, tv );

    // right side
    point_copy( &tv[0], &v[1] );
    point_copy( &tv[1], &v[2] );
    point_copy( &tv[2], &v[6] );
    point_copy( &tv[3], &v[5] );

    polygon_set( &side[5], 4, tv );

    for(frame = 0; frame < numFrames; frame++) {
        // Create image
        src = image_create(rows, cols);

        // Set the viewpoint based on the current frame
        float alpha = (float)frame / (numFrames - 1);
        point_set( &(view.vrp), 3*cos(alpha * 2 * M_PI), 2, 3*sin(alpha * 2 * M_PI), 1.0 );
        vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );
        vector_set( &(view.vup), 0, 1, 0 );

        view.d = 1;  // focal length
        view.du = 2;
        view.dv = view.du * (float)rows / cols;
        view.f = 0; // front clip plane
        view.b = 4; // back clip plane
        view.screenx = cols;
        view.screeny = rows;

        matrix_setView3D( &vtm, &view );

        // use a temporary polygon to transform stuff
        polygon_init( &tpoly );

        printf("Drawing Polygons for frame %d\n", frame);
        for(i=0;i<6;i++) {
            polygon_copy( &tpoly, &side[i] );
            matrix_xformPolygon( &vtm, &tpoly );

            // normalize by homogeneous coordinate before drawing
            polygon_normalize( &tpoly );
            polygon_draw( &tpoly, src, color[i] );
            polygon_print( &tpoly, stdout );
        }

        // Write the image to a file
        snprintf(filename, sizeof(filename), "frame-%04d.ppm", frame);
        printf("Writing image %s\n", filename);
        image_write(src, filename);

        // Free the image
        image_free(src);
    }

    return(0);
}
