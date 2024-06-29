/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.

  Bruce A. Maxwell updated 2021-09-05
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i, j;
  int min;

  if(argc < 3) {
    printf("Usage: ppmtest <input file> <output file>\n");
    exit(-1);
  }

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* mess with the image here  */
  for(i=0;i<imagesize;i++) {

    // this little piece of code thresholds out very red pixels
    // the resulting image will be very red where there is red stuff
    // and greyscale elsewhere
    j = (int)image[i].r - ((int)image[i].g + (int)image[i].b) / 2;
    min = image[i].g < image[i].b ? image[i].g : image[i].b;
    min = image[i].r < min ? image[i].r : min;
    min = min < 128 ? min : min/2;
    if(j > 10) {
      image[i].r = image[i].r < 128 ? image[i].r*2 : image[i].r;
    }
    else {
      image[i].r = min;
    }
    image[i].g = image[i].b = min;
  }

  /* write out the resulting image */
  writePPM(image, rows, cols, colors /* s/b 255 */, argv[2]);

  /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image);
#endif

  return(0);
}
