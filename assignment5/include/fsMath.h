//
// Created by DOTAFS on 2024/6/10.
//

#ifndef FSMATH_H
#define FSMATH_H

#include "Line.h"

typedef struct {
    double m[4][4];
} Matrix;

typedef struct {

} Vector;

/**
 * Set the Vector to (x, y, z, 0.0).
 */
void vector_set(Vector *v, double x, double y, double z);
/**
 *  Print out the Vector to stream fp in a pretty form.
 */
void vector_print(Vector *v, FILE *fp);
/**
 * Copy the src Vector into the dest Vector
 */
void vector_copy(Vector *dest, Vector *src);


#endif //FSMATH_H
