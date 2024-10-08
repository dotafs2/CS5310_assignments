/**
 * @author FS Qu
 * @def Polygon handling and rendering functions
 */

#ifndef POLYGON_H
#define POLYGON_H

#include <stdio.h>
#include "Image.h"
#include "stdlib.h"
#include "Line.h"
#include "Definition.h"
#include "Light.h"
#ifdef __cplusplus
extern "C" {
#endif



#define MAX_TEXTURES 3

 typedef enum{
  ShadeFrame,
  ShadeConstant,
  ShadeFlat,
  ShadeGouraud,
  ShadePhong,
  ShadeDepth
} ShadeMethod;

 typedef struct DrawState {
  Color color;
  Color flatColor;
  Color body;
  Color surface;
  float surfaceCoeff;
  ShadeMethod shade;
  int zBufferFlag;
//  TextureMap* textureMap;
  TextureMap textureMaps[MAX_TEXTURES];
  Point viewer;
 } DrawState;

/**
 * @brief Creates and returns an allocated Polygon pointer initialized to an empty Polygon.
 *
 * @return Polygon* A pointer to the allocated Polygon.
 */
Polygon *polygon_create();

/**
 * @brief Creates and returns an allocated Polygon pointer with the vertex list initialized to the points in vlist.
 *
 * @param numV Number of vertices.
 * @param vlist Pointer to the list of vertices.
 * @return Polygon* A pointer to the allocated Polygon.
 */
Polygon *polygon_createp(int numV, Point *vlist);

/**
 * @brief Frees the internal data and the Polygon pointer.
 *
 * @param p Pointer to the Polygon to be freed.
 */
void polygon_free(Polygon *p);

/**
 * @brief Initializes a pre-existing Polygon to an empty Polygon.
 *
 * @param p Pointer to the Polygon to be initialized.
 */
void polygon_init(Polygon *p);

/**
 * @brief Initializes the vertex list to the points in vlist.
 *
 * @param p Pointer to the Polygon to be initialized.
 * @param numV Number of vertices.
 * @param vlist Pointer to the list of vertices.
 */
void polygon_set(Polygon *p, int numV, Point *vlist);

/**
 * @brief Frees the internal data for a Polygon, if necessary, and sets numVertex to 0 and vertex to NULL.
 *
 * @param p Pointer to the Polygon to be cleared.
 */
void polygon_clear(Polygon *p);

/**
 * @brief Sets the oneSided field to the given value.
 *
 * @param p Pointer to the Polygon.
 * @param oneSided Value of the oneSided field.
 */
void polygon_setSided(Polygon *p, int oneSided);

/**
 * @brief Initializes the color array to the colors in clist.
 *
 * @param p Pointer to the Polygon.
 * @param numV Number of vertices.
 * @param clist Pointer to the list of colors.
 */
void polygon_setColors(Polygon *p, int numV, Color *clist);

/**
 * @brief Initializes the normal array to the vectors in nlist.
 *
 * @param p Pointer to the Polygon.
 * @param numV Number of vertices.
 * @param nlist Pointer to the list of normals.
 */
void polygon_setNormals(Polygon *p, int numV, Vector *nlist);

/**
 * @brief Initializes the vertex list to the points in vlist, the colors to the colors in clist,
 * the normals to the vectors in nlist, and the zBuffer and oneSided flags to their respective values.
 *
 * @param p Pointer to the Polygon.
 * @param numV Number of vertices.
 * @param vlist Pointer to the list of vertices.
 * @param clist Pointer to the list of colors.
 * @param nlist Pointer to the list of normals.
 * @param zBuffer Value of the z-buffer flag.
 * @param oneSided Value of the oneSided flag.
 */
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided, TextureCoord *tlist);
/**
 * @brief Sets the z-buffer flag to the given value.
 *
 * @param p Pointer to the Polygon.
 * @param flag Value of the z-buffer flag.
 */
void polygon_zBuffer(Polygon *p, int flag);

/**
 * @brief Copies the vertex and color data from one Polygon to another.
 *
 * @param to Pointer to the destination Polygon.
 * @param from Pointer to the source Polygon.
 */
void polygon_copy(Polygon *to, Polygon *from);

/**
 * @brief Prints Polygon data to the stream designated by the FILE pointer.
 *
 * @param p Pointer to the Polygon.
 * @param fp Pointer to the FILE stream.
 */
void polygon_print(Polygon *p, FILE *fp);

/**
 * @brief Normalizes the x and y values of each vertex by the homogeneous coordinate.
 *
 * @param p Pointer to the Polygon to be normalized.
 */
void polygon_normalize(Polygon *p);

/**
 * @brief Draws the outline of the Polygon using color c.
 *
 * @param p Pointer to the Polygon to be drawn.
 * @param src Pointer to the Image.
 * @param c Color to be used for drawing.
 */
void polygon_draw(Polygon *p, Image *src, Color c);

/**
 * @brief Draws the filled Polygon using color c with the scanline z-buffer rendering algorithm.
 *
 * @param p Pointer to the Polygon to be drawn.
 * @param src Pointer to the Image.
 * @param c Color to be used for drawing.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c);

/**
 * @brief Draws the filled Polygon using color c with the Barycentric coordinates algorithm.
 *
 * @param p Pointer to the Polygon to be drawn.
 * @param src Pointer to the Image.
 * @param c Color to be used for drawing.
 */
void polygon_drawFillB(Polygon *p, Image *src, Color c);
 void polygon_shade( Polygon *p, Lighting *l, DrawState *ds );
/**
 * @brief Helper function to compute Barycentric coordinates.
 *
 * @param vlist List of vertices.
 * @param px x-coordinate of the point.
 * @param py y-coordinate of the point.
 * @param alpha Pointer to store alpha value.
 * @param beta Pointer to store beta value.
 * @param gamma Pointer to store gamma value.
 * @return int 1 if the point is inside the triangle, 0 otherwise.
 */
int barycentric(Point *vlist, int px, int py, float *alpha, float *beta, float *gamma);
 void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light);
 void polygon_setTexture(Polygon *p, TextureCoord *stlist);
#ifdef __cplusplus
}
#endif

#endif // POLYGON_H
