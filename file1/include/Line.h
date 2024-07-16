
/**
 * @author DOTAFS on 2024/5/25.
 * @def Bresenham's line-drawing algorithm
 */

#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include "Image.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C" {
#endif

// Point structure
typedef struct {
    double val[4];  // Point3D
} Point;

typedef struct {
     double val[2]; // Point2D
} Point2;

// Line structure
typedef struct {
    int zBuffer;  // whether to use the z-buffer, should default to true (1)
    Point a;      // starting point
    Point b;      // ending point
} Line;

// Circle structure
typedef struct {
    double r;  // radius
    Point c;   // center
} Circle;

// Ellipse structure
typedef struct {
    double ra;  // major axis radius
    double rb;  // minor axis radius
    Point c;    // center
    double a;   // angle of major axis relative to the X-axis (optional)
} Ellipse;

// Polyline structure
typedef struct {
    int zBuffer;   // whether to use the z-buffer, should default to true (1)
    int numVertex; // number of vertices
    Point *vertex; // vertex information
} Polyline;

// Triangle functions
typedef struct {
    Point a, b, c;
    int zBuffer;
} Triangle;


// Point functions

/**
 * @brief Creates a point given x, y, z coordinates and a homogeneous coordinate w.
 *
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param z The z-coordinate.
 * @param w The homogeneous coordinate.
 * @return Point The created point.
 */
Point point_create(double x, double y, double z, double w);

/**
 * @brief Sets the values of a point to x, y, z, and h.
 *
 * @param p Pointer to the point to set.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param z The z-coordinate.
 * @param h The homogeneous coordinate.
 */
void point_set(Point *p, double x, double y, double z, double h);

/**
 * @brief Sets the first two values of the point to x and y, the third value to 0.0, and the fourth value to 1.0.
 *
 * @param p Pointer to the point to set.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 */
void point_set2D(Point *p, double x, double y);

/**
 * @brief Sets the values of the point to x, y, and z, and sets the homogeneous coordinate to 1.0.
 *
 * @param p Pointer to the point to set.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param z The z-coordinate.
 */
void point_set3D(Point *p, double x, double y, double z);

/**
 * @brief Normalizes the x, y, and z values of the point by its homogeneous coordinate.
 *
 * @param p Pointer to the point to normalize.
 */
void point_normalize(Point *p);

/**
 * @brief Copies the data from one point to another.
 *
 * @param to Pointer to the destination point.
 * @param from Pointer to the source point.
 */
void point_copy(Point *to, Point *from);

/**
 * @brief Draws the point into the image using the specified color.
 *
 * @param p Pointer to the point to draw.
 * @param src Pointer to the image.
 * @param c The color to use for drawing the point.
 */
void point_draw(Point *p, Image *src, Color c);

/**
 * @brief Draws the point into the image using the specified FPixel color.
 *
 * @param p Pointer to the point to draw.
 * @param src Pointer to the image.
 * @param c The FPixel color to use for drawing the point.
 */
void point_drawf(Point *p, Image *src, FPixel c);

/**
 * @brief Prints the contents of the point to the stream.
 *
 * @param p Pointer to the point to print.
 * @param fp FILE pointer to the stream.
 */
void point_print(Point *p, FILE *fp);

// Line functions

/**
 * @brief Creates a line given two points and a z-buffer flag.
 *
 * @param a Starting point of the line.
 * @param b Ending point of the line.
 * @param zBuffer Flag to indicate whether to use the z-buffer.
 * @return Line The created line.
 */
Line line_create(Point a, Point b, int zBuffer);

/**
 * @brief Sets a 2D line given the coordinates of the endpoints.
 *
 * @param l Pointer to the line to set.
 * @param x0 x-coordinate of the starting point.
 * @param y0 y-coordinate of the starting point.
 * @param x1 x-coordinate of the ending point.
 * @param y1 y-coordinate of the ending point.
 */
void line_set2D(Line *l, double x0, double y0, double x1, double y1);

/**
 * @brief Sets a line given two points.
 *
 * @param l Pointer to the line to set.
 * @param ta Starting point.
 * @param tb Ending point.
 */
void line_set(Line *l, Point ta, Point tb);

/**
 * @brief Sets the z-buffer flag for the line.
 *
 * @param l Pointer to the line.
 * @param flag The value to set for the z-buffer flag.
 */
void line_zBuffer(Line *l, int flag);

/**
 * @brief Normalizes the x and y values of the endpoints by their homogeneous coordinates.
 *
 * @param l Pointer to the line to normalize.
 */
void line_normalize(Line *l);

/**
 * @brief Copies the data from one line to another.
 *
 * @param to Pointer to the destination line.
 * @param from Pointer to the source line.
 */
void line_copy(Line *to, Line *from);

/**
 * @brief Draws the line into the image using the specified color and z-buffer if appropriate.
 *
 * @param l Pointer to the line to draw.
 * @param src Pointer to the image.
 * @param c The color to use for drawing the line.
 */
void line_draw(Line *l, Image *src, Color c);

// Circle functions

/**
 * @brief Initializes a circle with center `tc` and radius `tr`.
 *
 * @param c Pointer to the circle to initialize.
 * @param tc The center point of the circle.
 * @param tr The radius of the circle.
 */
void circle_set(Circle *c, Point tc, double tr);

/**
 * @brief Draws the circle into the image using the specified color.
 *
 * @param c Pointer to the circle to draw.
 * @param src Pointer to the image.
 * @param p The color to use for drawing the circle.
 */
void circle_draw(Circle *c, Image *src, Color p);

/**
 * @brief Draws a filled circle into the image using the specified color.
 *
 * @param c Pointer to the circle to draw.
 * @param src Pointer to the image.
 * @param p The color to use for drawing the filled circle.
 */
void circle_drawFill(Circle *c, Image *src, Color p);


// Ellipse functions

/**
 * @brief Initializes an ellipse with center `tc` and radii `ta` and `tb`.
 *
 * @param e Pointer to the ellipse to initialize.
 * @param tc The center point of the ellipse.
 * @param ta The major axis radius of the ellipse.
 * @param tb The minor axis radius of the ellipse.
 */
void ellipse_set(Ellipse *e, Point tc, double ta, double tb);

/**
 * @brief Draws the ellipse into the image using the specified color.
 *
 * @param e Pointer to the ellipse to draw.
 * @param src Pointer to the image.
 * @param p The color to use for drawing the ellipse.
 */
void ellipse_draw(Ellipse *e, Image *src, Color p);

/**
 * @brief Draws a filled ellipse into the image using the specified color.
 *
 * @param e Pointer to the ellipse to draw.
 * @param src Pointer to the image.
 * @param p The color to use for drawing the filled ellipse.
 */
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
// Polyline functions

/**
 * @brief Creates and returns an allocated Polyline pointer initialized to an empty Polyline.
 *
 * @return Polyline* A pointer to the allocated Polyline.
 */
Polyline* polyline_create();

/**
 * @brief Creates and returns an allocated Polyline pointer with the vertex list initialized to the points in vlist.
 *
 * @param numV Number of vertices.
 * @param vlist Pointer to the list of vertices.
 * @return Polyline* A pointer to the allocated Polyline.
 */
Polyline* polyline_createp(int numV, Point *vlist);

/**
 * @brief Frees the internal data and the Polyline pointer.
 *
 * @param p Pointer to the Polyline to be freed.
 */
void polyline_free(Polyline *p);

/**
 * @brief Initializes a pre-existing Polyline to an empty Polyline.
 *
 * @param p Pointer to the Polyline to be initialized.
 */
void polyline_init(Polyline *p);

/**
 * @brief Initializes the vertex list to the points in vlist.
 *
 * @param p Pointer to the Polyline to be initialized.
 * @param numV Number of vertices.
 * @param vlist Pointer to the list of vertices.
 */
void polyline_set(Polyline *p, int numV, Point *vlist);

/**
 * @brief Frees the internal data for a Polyline, if necessary, and sets numVertex to 0 and vertex to NULL.
 *
 * @param p Pointer to the Polyline to be cleared.
 */
void polyline_clear(Polyline *p);

/**
 * @brief Sets the z-buffer flag to the given value.
 *
 * @param p Pointer to the Polyline.
 * @param flag Value of the z-buffer flag.
 */
void polyline_zBuffer(Polyline *p, int flag);

/**
 * @brief Copies the vertex data from the source Polyline (from) to the destination (to).
 *
 * @param to Pointer to the destination Polyline.
 * @param from Pointer to the source Polyline.
 */
void polyline_copy(Polyline *to, Polyline *from);

/**
 * @brief Prints Polyline data to the stream designated by the FILE pointer.
 *
 * @param p Pointer to the Polyline.
 * @param fp Pointer to the FILE stream.
 */
void polyline_print(Polyline *p, FILE *fp);

/**
 * @brief Normalizes the x and y values of each vertex by the homogeneous coordinate.
 *
 * @param p Pointer to the Polyline to be normalized.
 */
void polyline_normalize(Polyline *p);

/**
 * @brief Draws the Polyline using color c and the z-buffer, if appropriate.
 *
 * @param p Pointer to the Polyline to be drawn.
 * @param src Pointer to the Image.
 * @param c Color to be used for drawing.
 */
void polyline_draw(Polyline *p, Image *src, Color c);

void print_line_coordinates(Point a, Point b);

// Triangle functions

/**
 * @brief Sets the vertices of the triangle to the given points.
 *
 * @param t Pointer to the triangle to set.
 * @param ta First vertex of the triangle.
 * @param tb Second vertex of the triangle.
 * @param tc Third vertex of the triangle.
 */
void triangle_set(Triangle *t, Point ta, Point tb, Point tc);

/**
 * @brief Sets the z-buffer flag for the triangle.
 *
 * @param t Pointer to the triangle.
 * @param flag The value to set for the z-buffer flag.
 */
void triangle_zBuffer(Triangle *t, int flag);

/**
 * @brief Normalizes the vertices of the triangle by their homogeneous coordinates.
 *
 * @param t Pointer to the triangle to normalize.
 */
void triangle_normalize(Triangle *t);

/**
 * @brief Copies the data from one triangle to another.
 *
 * @param to Pointer to the destination triangle.
 * @param from Pointer to the source triangle.
 */
void triangle_copy(Triangle *to, Triangle *from);

/**
 * @brief Draws the outline of the triangle into the image using the specified color.
 *
 * @param t Pointer to the triangle to draw.
 * @param src Pointer to the image.
 * @param c The color to use for drawing the triangle.
 */
void triangle_draw(Triangle *t, Image *src, Color c);

/**
 * @brief Fills the triangle into the image using the specified color.
 *
 * @param t Pointer to the triangle to fill.
 * @param src Pointer to the image.
 * @param c The color to use for filling the triangle.
 */
void triangle_fill(Triangle *t, Image *src, Color c);


#ifdef __cplusplus
}
#endif

#endif // LINE_H
