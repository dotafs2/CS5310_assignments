
/**
 * @author DOTAFS on 2024/5/25.
 * @def Bresenham's line-drawing algorithm
 */

#include "Line.h"
#define ROUND(a) ((int)(a + 0.5))
void point_set2D(Point *p, double x, double y) {
    if (!p) return;
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = 0.0;
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
    p->val[2] /= p->val[3];
    p->val[3] = 1.0;
}

void point_copy(Point *to, Point *from) {
    if (!to || !from) return;
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
        FPixel pixel = { {c.c[0], c.c[1], c.c[2]}, 1.0, 1.0 };
        src->data[y * src->cols + x] = pixel;
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

void line_draw(Line *l, Image *src, Color c) {
    if (!l || !src) return;

    int x0 = (int)l->a.val[0];
    int y0 = (int)l->a.val[1];
    int x1 = (int)l->b.val[0];
    int y1 = (int)l->b.val[1];

    // Bresenham's line algorithm
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    // Increase y coordinate or not
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while(1) {
        if (x0 >= 0 && x0 < src->cols && y0 >= 0 && y0 < src->rows) {
            FPixel pixel = { {c.c[0], c.c[1], c.c[2]}, 1.0f, 1.0f };
            src->data[y0 * src->cols + x0] = pixel;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void circle_set(Circle *c, Point tc, double tr) {
    if (!c) return;
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
    FPixel pixel = { {p.c[0], p.c[1], p.c[2]}, 1.0, 1.0 };

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
    FPixel pixel = { {p.c[0], p.c[1], p.c[2]}, 1.0, 1.0 };

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

// Draws the ellipse into the image using the specified color
void ellipse_draw(Ellipse *e, Image *src, Color p) {
    if (!e || !src) return;

    int xCenter = (int)e->c.val[0];
    int yCenter = (int)e->c.val[1];
    int ra = (int)e->ra;
    int rb = (int)e->rb;

    ellipseMidpoint(xCenter, yCenter, ra, rb, src, p, 0);
}

// Draws a filled ellipse into the image using the specified color
void ellipse_drawFill(Ellipse *e, Image *src, Color p) {
    if (!e || !src) return;

    int xCenter = (int)e->c.val[0];
    int yCenter = (int)e->c.val[1];
    int ra = (int)e->ra;
    int rb = (int)e->rb;

    ellipseMidpoint(xCenter, yCenter, ra, rb, src, p, 1);
}