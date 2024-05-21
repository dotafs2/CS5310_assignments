//
// Created by 曲辰熙 on 2024/5/21.
//
#include <iostream>
extern "C" {
#include "Image.h"
}
#include "fractals.h"

int main(int argc, char *argv[]) {
    Image *src = image_create(750, 1000);

    mandelbrot(src, -1.5, -1.5, 4);
   image_write(src, "mandelbrot-full.ppm");
    Image *sre = image_create(750, 1000);
    mandelbrot(sre, 1.755, -0.02, 0.02);
    image_write(sre, "mandelbrot.ppm");

    Image *srd = image_create(750, 1000);
    julia(srd, -1.8, -1.8 * 0.75, 3.6);
   image_write(srd, "julia.ppm");

    image_free(src);
    image_free(sre);
    image_free(srd);
    return 0;
}
