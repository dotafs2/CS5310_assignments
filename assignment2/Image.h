//
// Created by 曲辰熙 on 2024/5/18.
//

#ifndef ASSIGNMENT2_IMAGE_H
#define ASSIGNMENT2_IMAGE_H

#endif //ASSIGNMENT2_IMAGE_H


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

Image *image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
int image_alloc(Image *src, int rows, int cols);
Image* image_read(char *filename);
int image_write(Image *src, char *filename);