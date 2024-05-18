//
// Created by 曲辰熙 on 2024/5/18.
//

#include <stdlib.h>
#include <string.h>
#include "Image.h"



Image* image_create(int rows, int cols){
    Image* img = (Image*) malloc(sizeof(Image));
    if(img == NULL) return NULL;
    img -> rows = rows;
    img -> cols = cols;
    img -> data = (FPixel*) malloc(rows * cols * sizeof(FPixel));
    if(img->data == NULL) {
        free(img);
        return NULL;
    }
    img->depth = (float*) malloc(rows * cols * sizeof(float));
    if(img->depth == NULL) {
        free(img);
        return NULL;
    }
    img->alpha = (float*) malloc(rows * cols * sizeof(float));
    if(img->alpha == NULL) {
        free(img);
        return NULL;
    }
    img->max_val = 255;
    memset(img->filename, 0, sizeof(img->filename));
    return img;
}

void image_free(Image *src){
    for(int i = 0 ; i < src->rows * src->cols ; i ++){
        free(&src->data[i]);
    }
    free(src->data);
    free(src->depth);
    free(src->alpha);
    free(src->filename);
    free(src);
}

void image_init(Image *src){
    src->rows = 0;
    src->cols = 0;
    src->data = NULL;
    src->depth = NULL;
    src->alpha = NULL;
    src->max_val = 0;
}

int image_alloc(Image *src, int rows, int cols){
    if (src == NULL) return 1;
    for (int i = 0; i < rows*cols; i++) {
        src->data[i].rgb[0] = 0.f;
        src->data[i].rgb[1] = 0.f;
        src->data[i].rgb[2] = 0.f;
        *src->depth = 1.f;
        *src->alpha = 1.f;
    }
    return 0;
}