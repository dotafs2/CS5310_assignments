/*
	Bruce A Maxwell
	Fall 2014
	CS 351

	Creates a pair of spinning Bezier surfaces

	Accepts one command line parameter, which is the number of
	subdivisions to execute.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int frame;
	Color blue, green, purple, red, white;
	Point p[16];
	BezierSurface bc;
	DrawState ds;
	Module *curve;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}
	printf("Creating Bezier curves with %d subdivisions\n", divisions);

	color_set(&white, 1.0, 1.0, 1.0 );
	color_set(&blue, .1, .2, .8);
	color_set(&green, .2, 0.7, 0.3 );
	color_set(&purple, 0.6, 0.1, 0.7 );
	color_set(&red, 0.75, 0.3, 0.3 );

	curve = module_create();

	// create a flat plane
	point_set3D(&p[0], 0.0, -0.2, 0.0); // first row, constant x, even spacing in z
	point_set3D(&p[1], 0.0, -0.2, 0.33);
	point_set3D(&p[2], 0.0, -0.2, 0.66);
	point_set3D(&p[3], 0.0, -0.2, 1.0);
	point_set3D(&p[4], 0.33, -0.2, 0.0); // second row
	point_set3D(&p[5], 0.33, -0.2, 0.33);
	point_set3D(&p[6], 0.33, -0.2, 0.66);
	point_set3D(&p[7], 0.33, -0.2, 1.0);
	point_set3D(&p[8], 0.66, -0.2, 0.0); // third row
	point_set3D(&p[9], 0.66, -0.2, 0.33);
	point_set3D(&p[10], 0.66, -0.2, 0.66);
	point_set3D(&p[11], 0.66, -0.2, 1.0);
	point_set3D(&p[12], 1.0, -0.2, 0.0); // fourth row
	point_set3D(&p[13], 1.0, -0.2, 0.33);
	point_set3D(&p[14], 1.0, -0.2, 0.66);
	point_set3D(&p[15], 1.0, -0.2, 1.0);
	bezierSurface_set(&bc, p);

	// put the curve into a module
	module_color(curve, &red);
	module_bezierSurface(curve, &bc, divisions, 0);

	// create a curved surface sitting above the plane
	point_set3D(&p[0], 0.0, 0.0, 0.0); // first row, constant x, even spacing in z
	point_set3D(&p[1], 0.0, 0.2, 0.33);
	point_set3D(&p[2], 0.0, 0.5, 0.66);
	point_set3D(&p[3], 0.0, 0.1, 1.0);
	point_set3D(&p[4], 0.33, 0.8, 0.0); // second row
	point_set3D(&p[5], 0.33, -0.1, 0.33);
	point_set3D(&p[6], 0.33, 0.0, 0.66);
	point_set3D(&p[7], 0.33, 0.3, 1.0);
	point_set3D(&p[8], 0.66, 0.3, 0.0); // third row
	point_set3D(&p[9], 0.66, 0.8, 0.33);
	point_set3D(&p[10], 0.66, 0.9, 0.66);
	point_set3D(&p[11], 0.66, 0.5, 1.0);
	point_set3D(&p[12], 1.0, 0.4, 0.0); // fourth row
	point_set3D(&p[13], 1.0, 0.2, 0.33);
	point_set3D(&p[14], 1.0, 0.5, 0.66);
	point_set3D(&p[15], 1.0, 1.0, 1.0);
	bezierSurface_set(&bc, p);

	// put the curve into a module
	module_color(curve, &green);
	module_bezierSurface(curve, &bc, divisions, 0);

	// set up the drawstate
	drawstate_setColor(&ds, white);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 1.2, -3.0 );
	vector_set( &(view.vpn), 0.0, -0.8, 2.5 );
	vector_set( &(view.vup), 0.0, 1.0, 0.0 );
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );

	// Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( curve, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "bezSurf-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( curve );

	return(0);
}
