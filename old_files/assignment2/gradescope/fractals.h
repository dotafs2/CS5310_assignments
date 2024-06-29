//
// Created by 曲辰熙 on 2024/5/21.
//

#ifndef FRACTALS_H
#define FRACTALS_H

#include "Image.h"

/*
 * simple implementation of mandelbrot , same as required in lab.
 */
void mandelbrot(Image *src, float x0, float y0, float dx);

/*
Simple implementation of julia, same as required in lab, input current point, and dx, find next point.
 */
void julia(Image *src, float x0, float y0, float dx);
/*
 * My altered julia allows julia to generate moving pictures.
 */
void myJulia(Image *src, float x0, float y0, float dx, float offset);
#endif // FRACTALS_H
