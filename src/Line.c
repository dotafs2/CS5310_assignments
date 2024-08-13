
/**
 * @author DOTAFS on 2024/5/25.
 * @def Bresenham's line-drawing algorithm
 * Circles and Ellipses from Hearn and Baker textbook.
 */

#include "Line.h"
#define ROUND(a) ((int)(a + 0.5))
#define PRINT_LINE_INFO 0

#define DEBUG_INPUT_CHECK 0

#if DEBUG_INPUT_CHECK
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
    #define INPUT_CHECK(cond, msg) \
    do { \
    if (!(cond)) { \
    fprintf(stderr, "Input check failed: %s, at %s:%d\n", msg, __FILE__, __LINE__); \
    return; \
    } \
    } while (0)
#else
    #define DEBUG_PRINT(...)
    #define INPUT_CHECK(cond, msg)
#endif

#ifdef __cplusplus
extern "C" {
#endif


void point_set2D(Point *p, double x, double y) {
    if (!p) return;
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = 1.0;
    p->val[3] = 1.0;
}

void point_set3D(Point *p, double x, double y, double z) {
    if (!p) return;
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = z;
    p->val[3] = 1.0;
}

void point_set(Point *p, double x, double y, double z, double h) {
    if (!p) return;
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = z;
    p->val[3] = h;
}

void point_normalize(Point *p) {
    if (!p || p->val[3] == 0.0) return;
    p->val[0] /= p->val[3];
    p->val[1] /= p->val[3];
    // p->val[2] /= p->val[3]; // don’t normalize the z-value (index 2).  You will need it for z-buffer rendering
    p->val[3] = 1.0;
}

void point_copy(Point *to, Point *from) {
    if ((to == NULL) || (from == NULL)) return;
    to->val[0] = from->val[0];
    to->val[1] = from->val[1];
    to->val[2] = from->val[2];
    to->val[3] = from->val[3];
}

void point_draw(Point *p, Image *src, Color c) {
    if (!p || !src) return;
    int x = (int)p->val[0];
    int y = (int)p->val[1];
    if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
        FPixel pixel = { {c.c[0], c.c[1], c.c[2]} };
        src->data[y * src->cols + x] = pixel;
    }
}

    void fog_draw(Fog *fog, Image *src, Color c) {
    if ((fog == NULL) || (src == NULL)) return;

    int x = (int)fog->position.val[0];
    int y = (int)fog->position.val[1];

    for (int i = -2; i <= 2; i+=2) {
        for (int j = -2; j <= 2; j+=2) {
            int newX = x + i;
            int newY = y + j;

            if (newX >= 0 && newX < src->cols && newY >= 0 && newY < src->rows) {
                // 计算基于边界的alpha调整值
                float distanceToEdgeX = fmin(newX, src->cols - 1 - newX);
                float distanceToEdgeY = fmin(newY, src->rows - 1 - newY);
                float edgeFactor = fmin(distanceToEdgeX / (src->cols / 2.0f), distanceToEdgeY / (src->rows / 2.0f));
                float adjustedAlpha = fog->alpha * edgeFactor;

                // 确保alpha值不小于一个较小的阈值，如0.01
                adjustedAlpha = fmax(adjustedAlpha, 0.01f);

                for (int k = 0; k < 3; ++k) {
                    src->data[newY * src->cols + newX].rgb[k] =
                        src->data[newY * src->cols + newX].rgb[k] * (1 - adjustedAlpha) + c.c[k] * adjustedAlpha;
                }
            }
        }
    }
}




void point_drawf(Point *p, Image *src, FPixel c) {
    if (!p || !src) return;
    int x = (int)p->val[0];
    int y = (int)p->val[1];
    if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
        src->data[y * src->cols + x] = c;
    }
}

void point_print(Point *p, FILE *fp) {
    if (!p || !fp) return;
    fprintf(fp, "Point: (%f, %f, %f, %f)\n", p->val[0], p->val[1], p->val[2], p->val[3]);
}

// Line functions

Line line_create(Point a, Point b, int zBuffer) {
    Line line;
    line.a = a;
    line.b = b;
    line.zBuffer = zBuffer;
    return line;
}


void line_set2D(Line *l, double x0, double y0, double x1, double y1) {
    if (!l) return;
    point_set2D(&l->a, x0, y0);
    point_set2D(&l->b, x1, y1);
    l->zBuffer = 1;  // Default z-buffer to true (1)
}


void line_set(Line *l, Point ta, Point tb) {
    if (!l) return;
    l->a = ta;
    l->b = tb;
    l->zBuffer = 1;  // Default z-buffer to true (1)
}


void line_zBuffer(Line *l, int flag) {
    if (!l) return;
    l->zBuffer = flag;
}

void line_normalize(Line *l) {
    if (!l) return;
    point_normalize(&l->a);
    point_normalize(&l->b);
}

void line_copy(Line *to, Line *from) {
    if (!to || !from) return;
    to->a = from->a;
    to->b = from->b;
    to->zBuffer = from->zBuffer;
}
// after mvp and ndc, our viewpoint is the origin point of the axis, x y is the position of the screen, and z is the
// depth , bec of ndc, z is between 0 and 1 , we use 1/z to check the depth , that means larger z is closer to our eyes.
void line_draw(Line *l, Image *src, Color c) {

    INPUT_CHECK(l == NULL, "line_draw Line* l is NULL");
    INPUT_CHECK(src == NULL, "line_draw Image *src is NULL");

    // print_line_coordinates(l->a, l->b);

    // start and end point of line's coord
    int x0 = (int)l->a.val[0];
    int y0 = (int)l->a.val[1];
    int x1 = (int)l->b.val[0];
    int y1 = (int)l->b.val[1];
    // depth of z0 and z1 in ndc
    float z0 = (float)l->a.val[2];
    float z1 = (float)l->b.val[2];

    // Bresenham's line algorithm
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;


    // starting z, because we need to change z in every pixels of each line, each point in line have different z value,
    // so thats why dynamic z.
    float deltaZ;

    // z-buffer check
    if(dx > dy)
        deltaZ = (1/z1 - 1/z0) / (float)dx;
    else
        deltaZ = (1/z1 - 1/z0) / (float)dy;

    // start from z0's z, because z0 is the start of drawing line. and change it to 1/z0 right now , which means if z
    // is larger, closer to our eyes.
    float z = 1/z0;

    // bresenham's line algorithm, should render the next pixel in the same y or go up or down.
    // Increase y coordinate or not.
    int err = (dx > dy ? dx : -dy) / 2, e2;

    // while all the pixel has been draw in one single line, otherwise do not stop loop.
    while(1) {

        INPUT_CHECK(!(x0 >= 0 && x0 < src->cols && y0 >= 0 && y0 < src->rows), "line_draw starting point x0,y0 is out of "
                                                                               "range of src->rows and src->cols");
        if(y0 < 0)
            return;
            FPixel* targetPixel = &src->data[y0 * src->cols + x0];
            int targetIndex = y0 * src->cols + x0;
            // z-buffer test
            if (l->zBuffer) {
                if (z > src->depth[targetIndex]) {
                    // updated global z-buffer val stored in Fpixel
                    src->depth[targetIndex] = z;
                    targetPixel->rgb[0] = c.c[0];
                    targetPixel->rgb[1] = c.c[1];
                    targetPixel->rgb[2] = c.c[2];
                }
            } else {
                targetPixel->rgb[0] = c.c[0];
                targetPixel->rgb[1] = c.c[1];
                targetPixel->rgb[2] = c.c[2];
            }
          //  print_line_coordinates(l->a,l->b);


        z += deltaZ;
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void circle_set(Circle *c, Point tc, double tr) {
    if (c == NULL) return;
    c->c = tc;
    c->r = tr;
}

void circleMidpoint(int xCenter, int yCenter, int radius, Image *src, Color p, int fill) {
    int x = 0;
    int y = radius;
    int pVal = 1 - radius;
    void circlePlotPoints(int, int, int, int, Image *, Color, int);

    // Plot first set of points
    circlePlotPoints(xCenter, yCenter, x, y, src, p, fill);

    while (x < y) {
        x++;
        if (pVal < 0) {
            pVal += 2 * x + 1;
        } else {
            y--;
            pVal += 2 * (x - y) + 1;
        }
        circlePlotPoints(xCenter, yCenter, x, y, src, p, fill);
    }
}

void circlePlotPoints(int xCenter, int yCenter, int x, int y, Image *src, Color p, int fill) {
    FPixel pixel = { {p.c[0], p.c[1], p.c[2]}};

    if (fill) {
        for (int i = -y; i <= y; i++) {
            if (xCenter + x >= 0 && xCenter + x < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter + x)] = pixel;
            if (xCenter - x >= 0 && xCenter - x < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter - x)] = pixel;
        }
        for (int i = -x; i <= x; i++) {
            if (xCenter + y >= 0 && xCenter + y < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter + y)] = pixel;
            if (xCenter - y >= 0 && xCenter - y < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter - y)] = pixel;
        }
    } else {
        if (xCenter + x >= 0 && xCenter + x < src->cols && yCenter + y >= 0 && yCenter + y < src->rows)
            src->data[(yCenter + y) * src->cols + (xCenter + x)] = pixel;
        if (xCenter - x >= 0 && xCenter - x < src->cols && yCenter + y >= 0 && yCenter + y < src->rows)
            src->data[(yCenter + y) * src->cols + (xCenter - x)] = pixel;
        if (xCenter + x >= 0 && xCenter + x < src->cols && yCenter - y >= 0 && yCenter - y < src->rows)
            src->data[(yCenter - y) * src->cols + (xCenter + x)] = pixel;
        if (xCenter - x >= 0 && xCenter - x < src->cols && yCenter - y >= 0 && yCenter - y < src->rows)
            src->data[(yCenter - y) * src->cols + (xCenter - x)] = pixel;
        if (xCenter + y >= 0 && xCenter + y < src->cols && yCenter + x >= 0 && yCenter + x < src->rows)
            src->data[(yCenter + x) * src->cols + (xCenter + y)] = pixel;
        if (xCenter - y >= 0 && xCenter - y < src->cols && yCenter + x >= 0 && yCenter + x < src->rows)
            src->data[(yCenter + x) * src->cols + (xCenter - y)] = pixel;
        if (xCenter + y >= 0 && xCenter + y < src->cols && yCenter - x >= 0 && yCenter - x < src->rows)
            src->data[(yCenter - x) * src->cols + (xCenter + y)] = pixel;
        if (xCenter - y >= 0 && xCenter - y < src->cols && yCenter - x >= 0 && yCenter - x < src->rows)
            src->data[(yCenter - x) * src->cols + (xCenter - y)] = pixel;
    }
}

void circle_draw(Circle *c, Image *src, Color p) {
    if (!c || !src) return;

    int xCenter = (int)c->c.val[0];
    int yCenter = (int)c->c.val[1];
    int radius = (int)c->r;

    circleMidpoint(xCenter, yCenter, radius, src, p, 0);
}

void circle_drawFill(Circle *c, Image *src, Color p) {
    if (!c || !src) return;

    int xCenter = (int)c->c.val[0];
    int yCenter = (int)c->c.val[1];
    int radius = (int)c->r;

    circleMidpoint(xCenter, yCenter, radius, src, p, 1);
}

// Ellipse functions
void ellipse_set(Ellipse *e, Point tc, double ta, double tb) {
    if (!e) return;
    e->c = tc;
    e->ra = ta;
    e->rb = tb;
}

// Midpoint ellipse drawing algorithm
void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry, Image *src, Color p, int fill) {
    int Rx2 = Rx * Rx;
    int Ry2 = Ry * Ry;
    int twoRx2 = 2 * Rx2;
    int twoRy2 = 2 * Ry2;
    int pVal;
    int x = 0;
    int y = Ry;
    int px = 0;
    int py = twoRx2 * y;
    void ellipsePlotPoints(int, int, int, int, Image *, Color, int);

    // Plot the first set of points
    ellipsePlotPoints(xCenter, yCenter, x, y, src, p, fill);

    // Region 1
    pVal = ROUND(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
    while (px < py) {
        x++;
        px += twoRy2;
        if (pVal < 0) {
            pVal += Ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            pVal += Ry2 + px - py;
        }
        ellipsePlotPoints(xCenter, yCenter, x, y, src, p, fill);
    }

    // Region 2
    pVal = ROUND(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (pVal > 0) {
            pVal += Rx2 - py;
        } else {
            x++;
            px += twoRy2;
            pVal += Rx2 - py + px;
        }
        ellipsePlotPoints(xCenter, yCenter, x, y, src, p, fill);
    }
}

// Plots the points of the ellipse
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y, Image *src, Color p, int fill) {
    FPixel pixel = { {p.c[0], p.c[1], p.c[2]}};

    if (fill) {
        for (int i = -y; i <= y; i++) {
            if (xCenter + x >= 0 && xCenter + x < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter + x)] = pixel;
            if (xCenter - x >= 0 && xCenter - x < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter - x)] = pixel;
        }
        for (int i = -x; i <= x; i++) {
            if (xCenter + y >= 0 && xCenter + y < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter + y)] = pixel;
            if (xCenter - y >= 0 && xCenter - y < src->cols && yCenter + i >= 0 && yCenter + i < src->rows)
                src->data[(yCenter + i) * src->cols + (xCenter - y)] = pixel;
        }
    } else {
        if (xCenter + x >= 0 && xCenter + x < src->cols && yCenter + y >= 0 && yCenter + y < src->rows)
            src->data[(yCenter + y) * src->cols + (xCenter + x)] = pixel;
        if (xCenter - x >= 0 && xCenter - x < src->cols && yCenter + y >= 0 && yCenter + y < src->rows)
            src->data[(yCenter + y) * src->cols + (xCenter - x)] = pixel;
        if (xCenter + x >= 0 && xCenter + x < src->cols && yCenter - y >= 0 && yCenter - y < src->rows)
            src->data[(yCenter - y) * src->cols + (xCenter + x)] = pixel;
        if (xCenter - x >= 0 && xCenter - x < src->cols && yCenter - y >= 0 && yCenter - y < src->rows)
            src->data[(yCenter - y) * src->cols + (xCenter - x)] = pixel;
        if (xCenter + y >= 0 && xCenter + y < src->cols && yCenter + x >= 0 && yCenter + x < src->rows)
            src->data[(yCenter + x) * src->cols + (xCenter + y)] = pixel;
        if (xCenter - y >= 0 && xCenter - y < src->cols && yCenter + x >= 0 && yCenter + x < src->rows)
            src->data[(yCenter + x) * src->cols + (xCenter - y)] = pixel;
        if (xCenter + y >= 0 && xCenter + y < src->cols && yCenter - x >= 0 && yCenter - x < src->rows)
            src->data[(yCenter - x) * src->cols + (xCenter + y)] = pixel;
        if (xCenter - y >= 0 && xCenter - y < src->cols && yCenter - x >= 0 && yCenter - x < src->rows)
            src->data[(yCenter - x) * src->cols + (xCenter - y)] = pixel;
    }
}

void ellipse_draw(Ellipse *e, Image *src, Color p) {
    if (!e || !src) return;

    int xCenter = (int)e->c.val[0];
    int yCenter = (int)e->c.val[1];
    int ra = (int)e->ra;
    int rb = (int)e->rb;

    ellipseMidpoint(xCenter, yCenter, ra, rb, src, p, 0);
}

void ellipse_drawFill(Ellipse *e, Image *src, Color p) {
    if (!e || !src) return;

    int xCenter = (int)e->c.val[0];
    int yCenter = (int)e->c.val[1];
    int ra = (int)e->ra;
    int rb = (int)e->rb;

    ellipseMidpoint(xCenter, yCenter, ra, rb, src, p, 1);
}

Polyline* polyline_create() {
    Polyline *p = (Polyline *)malloc(sizeof(Polyline));
    if (!p) return NULL;
    p->numVertex = 0;
    p->vertex = NULL;
    p->zBuffer = 1;
    return p;
}

Polyline* polyline_createp(int numV, Point *vlist) {
    Polyline *p = polyline_create();
    if (!p) return NULL;
    polyline_set(p, numV, vlist);
    return p;
}

void polyline_free(Polyline *p) {
    if (p == NULL) return;
    if((&p->vertex) != NULL)
        free(p->vertex);
    free(p);
}

void polyline_init(Polyline *p) {
    if (!p) return;
    p->numVertex = 0;
    p->vertex = NULL;
    p->zBuffer = 1;
}

void polyline_set(Polyline *p, int numV, Point *vlist) {
    if (!p) return;
    if (p->vertex) {
        free(p->vertex);
    }
    p->vertex = (Point *)malloc(numV * sizeof(Point));
    if (!p->vertex) {
        p->numVertex = 0;
        return;
    }
    p->numVertex = numV;
    for (int i = 0; i < numV; i++) {
        p->vertex[i] = vlist[i];
    }
}

void polyline_clear(Polyline *p) {
    if (!p) return;
    if (p->vertex) {
        free(p->vertex);
        p->vertex = NULL;
    }
    p->numVertex = 0;
}

void polyline_zBuffer(Polyline *p, int flag) {
    if (!p) return;
    p->zBuffer = flag;
}

void polyline_copy(Polyline *to, Polyline *from) {
    if (!to || !from) return;
    polyline_clear(to);
    polyline_set(to, from->numVertex, from->vertex);
    to->zBuffer = from->zBuffer;
}

void polyline_print(Polyline *p, FILE *fp) {
    if (!p || !fp) return;
    fprintf(fp, "Polyline with %d vertices:\n", p->numVertex);
    for (int i = 0; i < p->numVertex; i++) {
        fprintf(fp, "  Vertex %d: (%f, %f, %f, %f)\n", i, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
    }
}

void print_line_coordinates(Point a, Point b) {
    printf("drawing line (%.2f %.2f) to (%.2f %.2f)\n", a.val[0], a.val[1], b.val[0], b.val[1]);
}

void polyline_normalize(Polyline *p) {
    if (p == NULL) return;
    for(int i=0;i<p->numVertex;i++) {
        p->vertex[i].val[0] /= p->vertex[i].val[3];
        p->vertex[i].val[1] /= p->vertex[i].val[3];
        p->vertex[i].val[3] = 1.0;
    }
}



void polyline_draw(Polyline *p, Image *src, Color c) {
    if (!p || !src) return;
    for (int i = 0; i < p->numVertex - 1; i++) {
        Line l;
        line_set(&l, p->vertex[i], p->vertex[i+1]);
        line_zBuffer(&l, p->zBuffer);
        line_draw(&l, src, c);
    }
}

void triangle_set(Triangle *t, Point ta, Point tb, Point tc) {
    if (!t) return;
    t->a = ta;
    t->b = tb;
    t->c = tc;
    t->zBuffer = 1;
}

void triangle_zBuffer(Triangle *t, int flag) {
    if (!t) return;
    t->zBuffer = flag;
}

void triangle_normalize(Triangle *t) {
    if (!t) return;
    point_normalize(&t->a);
    point_normalize(&t->b);
    point_normalize(&t->c);
}

void triangle_copy(Triangle *to, Triangle *from) {
    if (!to || !from) return;
    to->a = from->a;
    to->b = from->b;
    to->c = from->c;
    to->zBuffer = from->zBuffer;
}

void triangle_draw(Triangle *t, Image *src, Color c) {
    if (!t || !src) return;

    Line l1, l2, l3;
    line_set(&l1, t->a, t->b);
    line_zBuffer(&l1, t->zBuffer);
    line_draw(&l1, src, c);

    line_set(&l2, t->b, t->c);
    line_zBuffer(&l2, t->zBuffer);
    line_draw(&l2, src, c);

    line_set(&l3, t->c, t->a);
    line_zBuffer(&l3, t->zBuffer);
    line_draw(&l3, src, c);
}


// Swap two points
void swap_points(Point *a, Point *b) {
    Point temp = *a;
    *a = *b;
    *b = temp;
}

// Function to draw a filled triangle
void triangle_fill(Triangle *t, Image *src, Color c) {
    if (!t || !src) return;

    Point v0 = t->a;
    Point v1 = t->b;
    Point v2 = t->c;

    // Sort the vertices by y-coordinate ascending (v0, v1, v2)
    if (v0.val[1] > v1.val[1]) swap_points(&v0, &v1);
    if (v0.val[1] > v2.val[1]) swap_points(&v0, &v2);
    if (v1.val[1] > v2.val[1]) swap_points(&v1, &v2);

    // Compute the slopes of the triangle edges
    double dx01 = (v1.val[0] - v0.val[0]) / (v1.val[1] - v0.val[1]);
    double dx02 = (v2.val[0] - v0.val[0]) / (v2.val[1] - v0.val[1]);
    double dx12 = (v2.val[0] - v1.val[0]) / (v2.val[1] - v1.val[1]);

    double x0 = v0.val[0];
    double x1 = v0.val[0];

    // Scanline from y0 to y1
    for (int y = ROUND(v0.val[1]); y <= ROUND(v1.val[1]); y++) {
        if (y >= 0 && y < src->rows) {
            int xStart = (int)(x0 < x1 ? x0 : x1);
            int xEnd = (int)(x0 > x1 ? x0 : x1);
            if (xStart < 0) xStart = 0;
            if (xEnd >= src->cols) xEnd = src->cols - 1;
            for (int x = xStart; x <= xEnd; x++) {
                FPixel pixel = { {c.c[0], c.c[1], c.c[2]} };
                src->data[y * src->cols + x] = pixel;
            }
        }
        x0 += dx01;
        x1 += dx02;
    }

    x0 = v1.val[0];
    x1 = v0.val[0] + dx02 * (v1.val[1] - v0.val[1]);

    // Scanline from y1 to y2
    for (int y = ROUND(v1.val[1]); y <= ROUND(v2.val[1]); y++) {
        if (y >= 0 && y < src->rows) {
            int xStart = (int)(x0 < x1 ? x0 : x1);
            int xEnd = (int)(x0 > x1 ? x0 : x1);
            if (xStart < 0) xStart = 0;
            if (xEnd >= src->cols) xEnd = src->cols - 1;
            for (int x = xStart; x <= xEnd; x++) {
                FPixel pixel = { {c.c[0], c.c[1], c.c[2]}};
                src->data[y * src->cols + x] = pixel;
            }
        }
        x0 += dx12;
        x1 += dx02;
    }
}


#ifdef __cplusplus
}
#endif
