//
// Created by 曲辰熙 on 2024/5/21.
//

#ifndef FRACTALS_H
#define FRACTALS_H

#include "Image.h"

void mandelbrot(Image *src, float x0, float y0, float dx);
void julia(Image *src, float x0, float y0, float dx);

#endif // FRACTALS_H
