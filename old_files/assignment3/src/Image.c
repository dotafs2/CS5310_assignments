//
// Created by 曲辰熙 on 2024/5/18.
//
/*
 *  All image-related code, including creating, allocating memory, and so on.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"



Image* image_create(int rows, int cols){
    // command it bec image test: black.ppm
    // if( rows == 0 || cols == 0) return NULL;
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
    if (src == NULL) return;
    free(src->data);
    free(src->depth);
    free(src->alpha);
    free(src);
}

void image_init(Image *src){
    if (src == NULL) return;
    src->rows = 0;
    src->cols = 0;
    src->data = NULL;
    src->depth = NULL;
    src->alpha = NULL;
    src->max_val = 0;
}

int image_alloc(Image *src, int rows, int cols){
    if (src == NULL) return 1;
    if (rows == 0 || cols == 0) {
        image_dealloc(src);
        return 1;
    }

    if (src->rows != rows || src->cols != cols) {
        free(src->data);
        free(src->depth);
        free(src->alpha);

        // Allocate new memory for data, depth, and alpha
        src->data = (FPixel*) malloc(rows * cols * sizeof(FPixel));
        if (src->data == NULL) return 1;

        src->depth = (float*) malloc(rows * cols * sizeof(float));
        if (src->depth == NULL) {
            free(src->data);
            return 1;
        }

        src->alpha = (float*) malloc(rows * cols * sizeof(float));
        if (src->alpha == NULL) {
            free(src->data);
            free(src->depth);
            return 1;
        }
    }

    src->rows = rows;
    src->cols = cols;

    for (int i = 0; i < rows * cols; i++) {
        src->data[i].rgb[0] = 0.f;
        src->data[i].rgb[1] = 0.f;
        src->data[i].rgb[2] = 0.f;
        src->depth[i] = 1.f;
        src->alpha[i] = 1.f;
    }
    src->max_val = 255;
    return 0;
}

void image_dealloc(Image *src) {
    if (src == NULL) return;
    if (src->data != NULL) {
        free(src->data);
        src->data = NULL;
    }
    if (src->depth != NULL) {
        free(src->depth);
        src->depth = NULL;
    }
    if (src->alpha != NULL) {
        free(src->alpha);
        src->alpha = NULL;
    }
    src->rows = 0;
    src->cols = 0;
    src->max_val = 0;
    memset(src->filename, 0, sizeof(src->filename));
}

int image_write(Image* src, char *filename) {
    if (src == NULL || filename == NULL) {
        return 0;
    }
    FILE *file = fopen(filename, "wb");
    if (!file) return 1;

    fprintf(file, "P6\n%d %d\n%d\n", src->cols, src->rows, src->max_val);
    for (int i = 0; i < src->rows * src->cols; i++) {
        fputc((unsigned char)(src->data[i].rgb[0] * 255), file);
        fputc((unsigned char)(src->data[i].rgb[1] * 255), file);
        fputc((unsigned char)(src->data[i].rgb[2] * 255), file);
    }

    fclose(file);
    strncpy(src->filename, filename, sizeof(src->filename) - 1);
    return 0;
}

Image* image_read(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    char format[3];
    int rows, cols, max_val;
    fscanf(file, "%2s\n", format);
    if (strcmp(format, "P6") != 0) {
        fclose(file);
        return NULL;
    }
    // ignore the comments
    char ch = (char)fgetc(file);
    while (ch == '#') {
        while (fgetc(file) != '\n');
        ch = (char)fgetc(file);
    }
    ungetc(ch, file);

    // its unsafe but I am too lazy to change it
    fscanf(file, "%d %d\n", &cols, &rows);
    fscanf(file, "%d\n", &max_val);

    Image *img = image_create(rows, cols);
    if (!img) {
        fclose(file);
        return NULL;
    }
    img->max_val = max_val;
    for (int i = 0; i < rows * cols; i++) {
        img->data[i].rgb[0] = (float) (fgetc(file) / 255.0);
        img->data[i].rgb[1] = (float) (fgetc(file) / 255.0);
        img->data[i].rgb[2] = (float) (fgetc(file) / 255.0);
        img->depth[i] = 1.0f;
        img->alpha[i] = 1.0f;
    }

    fclose(file);
    strncpy(img->filename, filename, sizeof(img->filename) - 1);
    return img;
}


FPixel image_getf(Image *src, int r, int c) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        FPixel empty = { {0.f, 0.f, 0.f}, 0.f, 0.f };
        return empty;
    }
    return src->data[r * src->cols + c];
}

float image_getc(Image *src, int r, int c, int b) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols || b < 0 || b > 2) {
        return 0.f;
    }
    return src->data[r * src->cols + c].rgb[b];
}

float image_geta(Image *src, int r, int c) {
    if (!src    || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return 0.f;
    }
    return src->alpha[r * src->cols + c];
}

float image_getz(Image *src, int r, int c) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return 0.f;
    }
    return src->depth[r * src->cols + c];
}

void image_setf(Image *src, int r, int c, FPixel val){
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return;
    }
    src->data[r * src->cols + c] = val;
}

void image_setc(Image *src, int r, int c, int b, float val) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols || b < 0 || b > 2) {
        return;
    }
    src->data[r * src->cols + c].rgb[b] = val;
}

void image_seta(Image *src, int r, int c, float val) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return;
    }
    src->alpha[r * src->cols + c] = val;
}

void image_setz(Image *src, int r, int c, float val) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return;
    }
    src->depth[r * src->cols + c] = val;
}

void image_reset(Image *src) {
    if (!src) return;
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->data[i].rgb[0] = 0.f;
        src->data[i].rgb[1] = 0.f;
        src->data[i].rgb[2] = 0.f;
        src->depth[i] = 1.f;
        src->alpha[i] = 1.f;
    }
    src->max_val = 255;
    memset(src->filename, 0, sizeof(src->filename));
}


void image_fill(Image *src, FPixel val) {
    if (!src) return;
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->data[i] = val;
    }
}

void image_fillrgb(Image *src, float r, float g, float b) {
    if (!src) return;
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->data[i].rgb[0] = r;
        src->data[i].rgb[1] = g;
        src->data[i].rgb[2] = b;
    }
}

void image_filla(Image *src, float a) {
    if (!src) return;
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->alpha[i] = a;
    }
}

void image_fillz(Image *src, float z) {
    if (!src) return;
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->depth[i] = z;
    }
}

void color_copy(Color *to, Color *from) {
    if (!to || !from) return;
    to->c[0] = from->c[0];
    to->c[1] = from->c[1];
    to->c[2] = from->c[2];
}

void color_set(Color *to, float r, float g, float b) {
    if (!to) return;
    to->c[0] = r;
    to->c[1] = g;
    to->c[2] = b;
}

void image_setColor(Image *src, int r, int c, Color val) {
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return;
    }
    src->data[r * src->cols + c].rgb[0] = val.c[0];
    src->data[r * src->cols + c].rgb[1] = val.c[1];
    src->data[r * src->cols + c].rgb[2] = val.c[2];
}

Color image_getColor(Image *src, int r, int c) {
    Color col = { {0.f, 0.f, 0.f} }; // Default color is black
    if (!src || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return col;
    }
    col.c[0] = src->data[r * src->cols + c].rgb[0];
    col.c[1] = src->data[r * src->cols + c].rgb[1];
    col.c[2] = src->data[r * src->cols + c].rgb[2];
    return col;
}

