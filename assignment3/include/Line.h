
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
    double val[4];  // four element vector of doubles
} Point;

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



#endif // LINE_H
