//
// Created by 曲辰熙 on 2024/5/18.
//

#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
    float rgb[3];
    float a;
    float z;
} FPixel;

typedef struct {
    FPixel *data;
    int rows;
    int cols;
    float* depth;
    float* alpha;
    int max_val;
    char filename[256]; // bec imagetest.c do not have filename parameters in image_alloc, so can not use pointer.
} Image; // defined as 1D

typedef struct {
    float c[3];
} Color;

Image *image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
int image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);
Image* image_read(char *filename);
int image_write(Image *src, char *filename);
FPixel image_getf(Image *src, int r, int c);
float image_getc(Image *src, int r, int c, int b);
float image_geta(Image *src, int r, int c);
float image_getz(Image *src, int r, int c);
void image_setf(Image *src, int r, int c, FPixel val);
void image_setc(Image *src, int r, int c, int b, float val);
void image_seta(Image *src, int r, int c, float val);
void image_setz(Image *src, int r, int c, float val);
void image_reset(Image *src);
void image_fill(Image *src, FPixel val);
void image_fillrgb(Image *src, float r, float g, float b);
void image_filla(Image *src, float a);
void image_fillz(Image *src, float a);
void color_copy(Color *to, Color *from);
void color_set(Color *to, float r, float g, float b);
void image_setColor(Image *src, int r, int c, Color val);
Color image_getColor(Image *src, int r, int c);

#endif // IMAGE_H