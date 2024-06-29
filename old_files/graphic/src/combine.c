#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include <math.h>

#define USECPP 0

static Pixel* resizeImage(Pixel* image, int oldRows, int oldCols, int newRows, int newCols) {
    Pixel* newImage = (Pixel*)malloc(newRows * newCols * sizeof(Pixel));
    if (!newImage) {
        fprintf(stderr, "Unable to allocate memory for resized image\n");
        exit(-1);
    }

    float rowRatio = (float)oldRows / newRows;
    float colRatio = (float)oldCols / newCols;
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            int old_i = (int)(i * rowRatio);
            int old_j = (int)(j * colRatio);
            newImage[i * newCols + j] = image[old_i * oldCols + old_j];
        }
    }
    return newImage;
}

static void boxBlur(Pixel* image, int rows, int cols) {
    Pixel* tempImage = (Pixel*)malloc(rows * cols * sizeof(Pixel));
    if (!tempImage) {
        fprintf(stderr, "Unable to allocate memory for temporary image\n");
        exit(-1);
    }

    int kernelSize = 10;
    int halfKernel = kernelSize / 2;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int rSum = 0, gSum = 0, bSum = 0;
            int count = 0;

            for (int ki = -halfKernel; ki <= halfKernel; ++ki) {
                for (int kj = -halfKernel; kj <= halfKernel; ++kj) {
                    int ni = i + ki;
                    int nj = j + kj;

                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        Pixel p = image[ni * cols + nj];
                        rSum += p.r;
                        gSum += p.g;
                        bSum += p.b;
                        ++count;
                    }
                }
            }

            Pixel* p = &tempImage[i * cols + j];
            p->r = rSum / count;
            p->g = gSum / count;
            p->b = bSum / count;
        }
    }

    for (int i = 0; i < rows * cols; ++i) {
        image[i] = tempImage[i];
    }

    free(tempImage);
}

int main(int argc, char *argv[]) {
    Pixel *image1, *image2, *mask;
    int rows1, cols1, colors1;
    int rows2, cols2, colors2;
    int rows_mask, cols_mask, colors_mask;
    int newRows, newCols;
    long i, j;
    int dx, dy;

    if(argc < 8) {
        printf("Usage: ppmmerge <input image> <background image> <mask image> <dx> <dy> <scale factor> <output file>\n");
        exit(-1);
    }

    dx = atoi(argv[4]);
    dy = atoi(argv[5]);
    float scaleFactor = atof(argv[6]);

    /* read in the images */
    image1 = readPPM(&rows1, &cols1, &colors1, argv[1]);
    if(!image1) {
        fprintf(stderr, "Unable to read %s\n", argv[1]);
        exit(-1);
    }

    image2 = readPPM(&rows2, &cols2, &colors2, argv[2]);
    if(!image2) {
        fprintf(stderr, "Unable to read %s\n", argv[2]);
        exit(-1);
    }

    mask = readPPM(&rows_mask, &cols_mask, &colors_mask, argv[3]);
    if(!mask) {
        fprintf(stderr, "Unable to read %s\n", argv[3]);
        exit(-1);
    }

    /* calculate new dimensions for the input image and mask */
    newRows = (int)(rows1 * scaleFactor);
    newCols = (int)(cols1 * scaleFactor);

    /* resize the input image and mask */
    Pixel* resizedImage1 = resizeImage(image1, rows1, cols1, newRows, newCols);
    Pixel* resizedMask = resizeImage(mask, rows_mask, cols_mask, newRows, newCols);

    /* blur the background image */
    boxBlur(image2, rows2, cols2);

    /* make sure the images and resized mask have the same size */
    if(rows1 != rows_mask || cols1 != cols_mask) {
        fprintf(stderr, "Resized input image and mask must have the same dimensions\n");
        free(image1);
        free(image2);
        free(mask);
        free(resizedImage1);
        free(resizedMask);
        exit(-1);
    }

    /* merge the images using the resized mask */
    for(i=0; i<newRows; i++) {
        for(j=0; j<newCols; j++) {
            int idx = i * newCols + j;
            int idx_bg = (i + dy) * cols2 + (j + dx);

            if(i + dy < 0 || i + dy >= rows2 || j + dx < 0 || j + dx >= cols2)
                continue;

            float alpha = resizedMask[idx].r / 255.0;
            image2[idx_bg].r = alpha * resizedImage1[idx].r + (1 - alpha) * image2[idx_bg].r;
            image2[idx_bg].g = alpha * resizedImage1[idx].g + (1 - alpha) * image2[idx_bg].g;
            image2[idx_bg].b = alpha * resizedImage1[idx].b + (1 - alpha) * image2[idx_bg].b;
        }
    }

    /* write out the resulting image */
    writePPM(image2, rows2, cols2, colors2 /* s/b 255 */, argv[7]);

    /* free the image memory */
#if USECPP
    delete[] image1;
    delete[] image2;
    delete[] mask;
    delete[] resizedImage1;
    delete[] resizedMask;
#else
    free(image1);
    free(image2);
    free(mask);
    free(resizedImage1);
    free(resizedMask);
#endif

    return 0;
}
