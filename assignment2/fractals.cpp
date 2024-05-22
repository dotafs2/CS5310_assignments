// fractals.cpp
#include "fractals.h"
#include <complex>
#include <iostream>
using namespace std;


void mandelbrot(Image *src, float x0, float y0, float dx) {
    int rows = src->rows;
    int cols = src->cols;
    float dy = dx * rows / cols;
    float y1 = y0 + dy;
    float x1 = x0 + dx;

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = x0 + col * (x1 - x0) / cols;
            float y = y1 - r * (y1 - y0) / rows;
            std::complex<float> z(0, 0);
            std::complex<float> c(x, y);
            int k = 0;
            for (; k < 255; k++) {
                if (abs(z) > 2.0) break;
                z = z * z - c;
            }
            FPixel pixel = { { (float)k / 255, (float)k / 255, (float)k / 255 }, 1.0f, 1.0f };

          image_setf(src, r, col, pixel);
        }
    }
}
// basic julia
void julia(Image *src, float x0, float y0, float dx) {
    int rows = src->rows;
    int cols = src->cols;
    float dy = dx * rows / cols;
    float y1 = y0 + dy;
    float x1 = x0 + dx;

    complex<float> c(0.7, -0.27015);

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = x0 + col * (x1 - x0) / cols;
            float y = y0 + r * (y1 - y0) / rows;
            complex<float> z(x, y);
            int k = 0;
            for (; k < 255; k++) {
                if (abs(z) > 2.0) break;
                z = z * z - c;
            }
            float t = (float)k / 255;
            FPixel pixel = { { t, t, t }, 1.0f, 1.0f };
            image_setf(src, r, col, pixel);
        }
    }
}
// my julia
void myJulia(Image *src, float x0, float y0, float dx, float offset) {
    int rows = src->rows;
    int cols = src->cols;
    float dy = dx * rows / cols;
    float y1 = y0 + dy;
    float x1 = x0 + dx;

    complex<float> c(0.74 + offset, -0.290 - offset);

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = x0 + col * (x1 - x0) / cols;
            float y = y0 + r * (y1 - y0) / rows;


            complex<float> z(x, y);
            int k = 0;
            for (; k < 255; k++) {
                if (abs(z) > 2.0) break;
                z = z * z - c;
            }
            float t = (float)k / 255;
            FPixel pixel = { { t, t, t }, 1.0f, 1.0f };
            image_setf(src, r, col, pixel);
        }
    }
}



