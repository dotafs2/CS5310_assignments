/*
	Bruce A. Maxwell
	Fall 2014
	CS351

	Test function for each function in the vector and matrix library.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"


int main(int argc, char *argv[]) {
	Point p;
	Point q;
	Vector u;
	Vector v;
	Vector w;
	Vector x;
	Vector y;
	Vector z;
	Matrix m;
	Matrix n;
	Matrix o;
	int i, j, k;

	point_set(&p, 1.0, 1.0, 1.0, 1.0);
	point_set(&q, -1.0, -1.0, -1.0, 1.0);

	vector_set(&v, 1.0, 1.0, 1.0);
	vector_set(&x, 1.0, 0.0, 0.0);
	vector_set(&y, 0.0, 1.0, 0.0);
	vector_set(&z, 0.0, 0.0, 1.0);
	
	vector_print(&v, stdout);
	printf("vector_length: %.3f (should be 1.732)\n", vector_length(&v));
	
	vector_normalize( &v );
	printf("vector_normalize: should be (0.577, 0.577, 0.577, 0.0)\n");
	vector_print(&v, stdout);

	vector_set(&v, 1.0, 2.0, -3.0);
	vector_set(&w, 2.0, 3.0, 1.0);
	printf("vector_dot: %.3f (should be 5.0)\n", vector_dot(&v, &w));

	vector_cross(&v, &w, &u);
	printf("vector_cross: should be (11.0, -7.0, -1.0, 0.0)\n");
	vector_print(&u, stdout);

	vector_cross(&w, &v, &u);
	printf("vector_cross: should be (-11.0, 7.0, 1.0, 0.0)\n");
	vector_print(&u, stdout);
	
	printf("\n---------------\nThis should/could be garbage\n");
	matrix_print(&m, stdout);

	matrix_clear(&m);
	printf("matrix_clear: zero matrix\n");
	matrix_print(&m, stdout);

	matrix_identity(&m);
	printf("matrix_identity: identity matrix w/ones on the diagonals\n");
	matrix_print(&m, stdout);

	for(i=0,k=0;i<4;i++) {
		for(j=0;j<4;j++,k++) {
			matrix_set(&m, i, j, (double)k);
		}
	}
	printf("matrix_set: should set to 0-15 in row major order\n");
	matrix_print(&m, stdout);

	printf("matrix_get: should print out 0-15\n");
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			printf("%.2f ", matrix_get(&m, i, j));
		}
	}
	printf("\n");

	matrix_copy(&n, &m);
	
	matrix_transpose(&m);
	printf("matrix_transpose: should transpose the prior matrix so numbers are column major\n");
	matrix_print(&m, stdout);

	printf("matrix_copy: should print the original matrix\n");
	matrix_print(&n, stdout);

	matrix_multiply(&m, &n, &o);
	printf("Result should be: \n");
	printf("[[224, 248, 272, 296]\n[248 276, 304, 332]\n[272, 304, 336, 368]\n[296, 332, 368, 404]]\n");
	matrix_print(&o, stdout);

	matrix_transpose(&n);
	matrix_multiply(&m, &n, &o);
	printf("matrix_multiply: result should be\n");
	printf("[[56, 152, 248, 344]\n[62, 174, 286, 398]\n[68, 196, 324, 452]\n[74, 218, 362, 506]]\n");
	matrix_print(&o, stdout);

	matrix_multiply(&m, &n, &m);
	printf("matrix_multiply: result should not change from above\n");
	matrix_print(&m, stdout);

	matrix_copy(&m, &n);
	matrix_multiply(&m, &n, &n);
	printf("matrix_multiply: result should not change from above\n");
	matrix_print(&n, stdout);

	matrix_multiply(&m, &m, &m);
	printf("matrix_multiply: result should not change from above\n");
	matrix_print(&m, stdout);
	
	matrix_identity(&m);
	matrix_identity(&n);
	matrix_identity(&o);

	matrix_scale2D(&m, 2.0, 3.0);
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_scale2D/xformPoint: result should be (2.0, 3.0, 1.0, 1.0)\n");
	point_print(&q, stdout);

	matrix_rotateZ(&n, cos(M_PI/6.0), sin(M_PI/6.0) );
	matrix_xformPoint(&n, &p, &q);
	printf("matrix_rotateZ/xformPoint: result should be ( 0.366, 1.366, 1.000, 1.000 )\n");
	point_print(&q, stdout);

	matrix_translate2D(&o, -3, -5);
	matrix_xformPoint(&o, &p, &q);
	printf("matrix_translate2D/xformPoint: result should be ( -2.0, -4.0, 1.0, 1.0 )\n");
	point_print(&q, stdout);

	matrix_identity(&m);
	matrix_shear2D(&m, 1.5, -1.5);
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_shear2D/xformPoint: result should be (2.5, -0.5, 1.0, 1.0)\n");
	point_print(&q, stdout);

	matrix_identity(&m);
	matrix_translate(&m, 4, -3, 2);
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_translate/xformPoint: result should be (5.000, -2.000, 3.000, 1.000)\n");
	point_print(&q, stdout);

	matrix_identity(&m);
	matrix_scale(&m, 2, -1, 3);
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_scale/xformPoint: result should be (2.000, -1.000, 3.000, 1.000)\n");
	point_print(&q, stdout);

	matrix_identity(&m);
	matrix_rotateX(&m, cos(M_PI/6.0), sin(M_PI/6.0) );
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_rotateX/xformPoint: result should be (1.000, 0.366, 1.366, 1.000)\n");
	point_print(&q, stdout);

	matrix_identity(&m);
	matrix_rotateY(&m, cos(M_PI/6.0), sin(M_PI/6.0) );
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_rotateY/xformPoint: result should be (1.366, 1.000, 0.366, 1.000)\n");
	point_print(&q, stdout);

	// do some cross products to make a coordinate system, normalize, then call Rxyz
	vector_set(&x, 3, 4, 5);
	vector_set(&y, 0, 1, 0);
	vector_cross(&x, &y, &z);
	vector_cross(&z, &x, &y);
	vector_normalize(&x);
	vector_normalize(&y);
	vector_normalize(&z);
	matrix_identity(&m);
	matrix_rotateXYZ(&m, &x, &y, &z);
	matrix_xformPoint(&m, &p, &q);
	printf("matrix_rotateXYZ: result should be (1.697, 0.049, -0.343, 1.000)\n");
	point_print(&q, stdout);

	return(0);
}
