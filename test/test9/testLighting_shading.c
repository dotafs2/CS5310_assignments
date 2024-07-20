/*
	Bruce A. Maxwell
	Fall 2014
	CS 351 

	Lighting test function. This function should print out:

	c: 0.33 0.42 0.50

	If your lighting 
 */

#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include "Light.h"

int main(int argc, char *argv[]) {

	Vector N;
	Vector V;
	Point p;
	Color Cb = {{0.4, 0.5, 0.6}};
	Color Cs = {{0.2, 0.2, 0.2}};
	float s = 10;
	Lighting *l;
	Color c;
	Color amb = {{0.3, 0.3, 0.3}};
	Color pt = {{1.0, 1.0, 1.0}};
	Point loc;

	point_set3D(&p, 0, 0, 0); // origin
	point_set3D(&N, 0, 1, 0); // up
	vector_set(&V, 1, 2, 3);
	

	l = lighting_create();
	lighting_add( l, LightAmbient, &amb, NULL, NULL, 0, 0);

	point_set3D( &loc, 3, 2, 1 );
	lighting_add( l, LightPoint, &pt, NULL, &loc, 0, 0 );

	printf("Testing ambient + point light with point light source on same side as normal\n");
	lighting_shading( l, &N, &V, &p, &Cb, &Cs, s, 1, &c );

	printf("c: %.2f %.2f %.2f\n", c.c[0], c.c[1], c.c[2] );


	printf("Testing ambient + point light with point light source opposite normal and 2-sided\n");
	// invert the normal
	point_set3D( &N, 0, -1, 0 );
	lighting_shading( l, &N, &V, &p, &Cb, &Cs, s, 0, &c );

	printf("c: %.2f %.2f %.2f\n", c.c[0], c.c[1], c.c[2] );


	printf("Testing ambient + point light with point light source opposite normal and 1-sided\n");
	point_set3D( &N, 0, -1, 0 );
	lighting_shading( l, &N, &V, &p, &Cb, &Cs, s, 1, &c );

	printf("c: %.2f %.2f %.2f\n", c.c[0], c.c[1], c.c[2] );


	printf("Testing ambient + point with the light and view on opposite sides (2-sided)\n");
	// put back the normal
	point_set3D( &N, 0, 1, 0 );
	// invert the view vector
	vector_set(&V, -3, -2, -1);
	lighting_shading( l, &N, &V, &p, &Cb, &Cs, s, 0, &c );

	printf("c: %.2f %.2f %.2f\n", c.c[0], c.c[1], c.c[2] );

	return(0);
}
