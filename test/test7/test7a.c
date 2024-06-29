/*
	Bruce A Maxwell
	Fall 2014
	CS 351

	Test function for 2D Bezier curves in screen space
*/
#include <stdio.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	Color blue;
	Point p[4];
	BezierCurve bc;
	Image *src = image_create(300, 400);

	// set up four points
	point_set2D(&p[0], 50, 200);
	point_set2D(&p[1], 150, 40);
	point_set2D(&p[2], 250, 80);
	point_set2D(&p[3], 350, 250);

	color_set(&blue, .1, .2, .8);

	// set and draw the curve
	printf("Setting bezier curve\n");
	bezierCurve_set(&bc, p);

	printf("Drawing bezier curve\n");
	bezierCurve_draw( &bc, src, blue );

	// write the image and clean up
	image_write(src, "bez2d.ppm");

	printf("Writing bez2d.ppm\n");
	image_free( src );

	return(0);
}
