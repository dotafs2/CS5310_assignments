//
// Created by 曲辰熙 on 2024/5/18.
//
/*
 * All image-related code, including creating, allocating memory, and so on.
 */

#ifndef IMAGE_H
#define IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Definition.h"


// Image creation and memory management functions
Image *image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
int image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);
Image *image_scale(Image *src, int newRows, int newCols);
// Image file I/O functions
Image *image_read(char *filename);
int image_write(Image *src, char *filename);

// Image pixel access functions
FPixel image_getf(Image *src, int r, int c);
float image_getc(Image *src, int r, int c, int b);
float image_geta(Image *src, int r, int c);
float image_getz(Image *src, int r, int c);
void image_setf(Image *src, int r, int c, FPixel val);
void image_setc(Image *src, int r, int c, int b, float val);
void image_seta(Image *src, int r, int c, float val);
void image_setz(Image *src, int r, int c, float val);
void image_copy(Image *dest, const Image *src);

// Image manipulation functions
void image_reset(Image *src);
void image_fill(Image *src, FPixel val);
void image_fillrgb(Image *src, float r, float g, float b);
void image_filla(Image *src, float a);
void image_fillz(Image *src, float a);

// Color functions
void color_init(Color *c);
void color_copy(Color *to, Color *from);
void color_set(Color *to, float r, float g, float b);
void image_setColor(Image *src, int r, int c, Color val);
Color image_getColor(Image *src, int r, int c);
void swap_color(Color *c1, Color *c2);
void swap_texture(TextureCoord *t1, TextureCoord *t2);
 void swap_vector(Vector *v1, Vector *v2);
void convertTextureToAlpha(TextureMap *texture);

 // Texture reading function
TextureMap *texture_read(char *filename);




#ifdef __cplusplus
}
#endif

#endif // IMAGE_H
