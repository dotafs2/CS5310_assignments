//
// Created by DOTAFS on 2024/6/10. This file include basic math functions, MVP matrix , etc..
//

#ifndef FSMATH_H
#define FSMATH_H

#include "Line.h"
#include "stdlib.h"
#include "Polygon.h"
#include "string.h"




typedef struct {
    double m[4][4];
} Matrix;



typedef struct {
    Point V0;       // Center of the view rectangle in world coordinates
    double du;      // Width of the view rectangle in world coordinates
    double nx, ny;  // Normalized orientation vector (cos(theta_v), sin(theta_v))
    int C;          // Number of columns in the output image
    int R;          // Number of rows in the output image
} View2D;


typedef struct {
    Point vrp;        // View Reference Point
    Vector vpn;       // View Plane Normal
    Vector vup;       // View Up Vector
    double d;         // Projection Distance
    double du;        // View Window Extent (width)
    double dv;        // View Window Extent (height)
    double f;         // Front Clip Plane
    double b;         // Back Clip Plane
    int screenx;      // Screen Size in X direction (pixels)
    int screeny;      // Screen Size in Y direction (pixels)
} View3D;



void vector_set(Vector *v, double x, double y, double z);

void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);

void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);

void vector_cross(Vector *a, Vector *b, Vector *c);

void matrix_print(Matrix *m, FILE *fp);

void matrix_clear(Matrix *m);

void matrix_identity(Matrix *m);

double matrix_get(Matrix *m, int r, int c) ;
void matrix_set(Matrix *m, int r, int c, double v);

void matrix_copy(Matrix *dest, Matrix *src);
void matrix_transpose(Matrix *m);

void matrix_multiply(Matrix *left, Matrix *right, Matrix *m) ;

void matrix_xformPoint(Matrix *m, Point *p, Point *q) ;
void matrix_xformVector(Matrix *m, Vector *p, Vector *q) ;
void matrix_xformPolygon(Matrix *m, Polygon *p) ;

void matrix_xformPolyline(Matrix *m, Polyline *p) ;
void matrix_xformLine(Matrix *m, Line *line);

void matrix_scale2D(Matrix *m, double sx, double sy) ;
void matrix_rotateZ(Matrix *m, double cth, double sth) ;
void matrix_translate2D(Matrix *m, double tx, double ty) ;
void matrix_shear2D(Matrix *m, double shx, double shy);

void matrix_translate(Matrix *m, double tx, double ty, double tz);
void matrix_scale(Matrix *m, double sx, double sy, double sz);

void matrix_rotateX(Matrix *m, double cth, double sth);

void matrix_rotateY(Matrix *m, double cth, double sth);

void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w);

void matrix_shearZ(Matrix *m, double shx, double shy);
void matrix_perspective(Matrix *m, double d);

void matrix_setView2D(Matrix *vtm, View2D *view);
void matrix_setView3D(Matrix *vtm, View3D *view);

#endif //FSMATH_H
