//
// Created by DOTAFS on 2024/6/29.
//
#include "Bezier.h"

#include "Polygon.h"


void bezierCurve_init(BezierCurve *b) {
    for (int i = 0; i < 4; i++) {
        b->p[i].val[0] = i / 3.0;
        b->p[i].val[1] = 0.0;
        b->p[i].val[2] = 0.0;
    }
    b->zbufferFlag = 1;
}


void bezierSurface_init(BezierSurface *b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b->p2[i][j].val[0] = i / 3.0;
            b->p2[i][j].val[1] = 0.0;
            b->p2[i][j].val[2] = j / 3.0;
        }
    }
    b->zbufferFlag = 1;
}


void bezierCurve_set(BezierCurve *b, Point *vlist) {
    for (int i = 0; i < 4; i++) {
        b->p[i].val[0] = vlist[i].val[0];
        b->p[i].val[1] = vlist[i].val[1];
        b->p[i].val[2] = vlist[i].val[2];
        b->p[i].val[3] = vlist[i].val[3];
    }
}


void bezierSurface_set(BezierSurface *b, Point *vlist) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b->p2[i][j].val[0] = vlist[i * 4 + j].val[0];
            b->p2[i][j].val[1] = vlist[i * 4 + j].val[1];
            b->p2[i][j].val[2] = vlist[i * 4 + j].val[2];
            b->p2[i][j].val[3] = vlist[i * 4 + j].val[3];
        }
    }
}

void bezierCurve_copy(BezierCurve *to, const BezierCurve *from) {
    for (int i = 0; i < 4; i++) {
        to->p[i].val[0] = from->p[i].val[0];
        to->p[i].val[1] = from->p[i].val[1];
        to->p[i].val[2] = from->p[i].val[2];
    }
    to->zbufferFlag = from->zbufferFlag;
}

void bezierCurve_zBuffer(BezierCurve *b, int flag) {
    b->zbufferFlag = flag;
}

void bezierSurface_zBuffer(BezierSurface *b, int flag) {
    b->zbufferFlag = flag;
}

double point_distance(Point a, Point b) {
    double dx = a.val[0] - b.val[0];
    double dy = a.val[1] - b.val[1];
    double dz = a.val[2] - b.val[2];
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// explicit formula computation, I will add de Casteljau algorithm latter
Point bezierCurve_explicit(BezierCurve *b, double t) {
    Point result;
    double u = 1 - t;
    double tt = t * t;
    double uu = u * u;
    double uuu = uu * u;
    double ttt = tt * t;

    // Bernstein polynomials
    result.val[0] = uuu * b->p[0].val[0]; // (1-t)^3 * P0
    result.val[0] += 3 * uu * t * b->p[1].val[0]; // 3 * (1-t)^2 * t * P1
    result.val[0] += 3 * u * tt * b->p[2].val[0]; // 3 * (1-t) * t^2 * P2
    result.val[0] += ttt * b->p[3].val[0]; // t^3 * P3

    result.val[1] = uuu * b->p[0].val[1];
    result.val[1] += 3 * uu * t * b->p[1].val[1];
    result.val[1] += 3 * u * tt * b->p[2].val[1];
    result.val[1] += ttt * b->p[3].val[1];

    result.val[2] = uuu * b->p[0].val[2];
    result.val[2] += 3 * uu * t * b->p[1].val[2];
    result.val[2] += 3 * u * tt * b->p[2].val[2];
    result.val[2] += ttt * b->p[3].val[2];

    return result;
}


Point lerp(Point a, Point b, double t) {
    Point result;
    result.val[0] = (1 - t) * a.val[0] + t * b.val[0];
    result.val[1] = (1 - t) * a.val[1] + t * b.val[1];
    result.val[2] = (1 - t) * a.val[2] + t * b.val[2];
    return result;
}


void deCasteljau(Point *control_points, int count, double t, Point *result) {
    if (count == 1) {
        *result = control_points[0];
        return;
    }


    Point *new_points = (Point *)malloc((count - 1) * sizeof(Point));
    for (int i = 0; i < count - 1; i++) {
        new_points[i] = lerp(control_points[i], control_points[i + 1], t);
    }
    deCasteljau(new_points, count - 1, t, result);
    free(new_points);
}



void bezierCurve_draw(BezierCurve *b, Image *src, Color c, BezierMethod flag) {
    int segments = 100; // Number of line segments
    double step = 1.0 / segments;
    if(flag == BezierExplicit) {
        Point prev = bezierCurve_explicit(b, 0.0);

        for (int i = 1; i <= segments; i++) {
            double t = i * step;
            Point curr = bezierCurve_explicit(b, t);

            Line line;
            line.a = prev;
            line.b = curr;
            line_draw(&line, src, c);

            prev = curr;
        }
    }
    if(flag == BezierDeCasteljau) {
        Point prev, curr;
        deCasteljau(b->p, 4, 0.0, &prev);

        for (int i = 1; i <= segments; i++) {
            double t = i * step;
            deCasteljau(b->p, 4, t, &curr);

            Line line;
            line.a = prev;
            line.b = curr;
            line_draw(&line, src, c);

            prev = curr;
        }
    }
}


void bezierSurface_draw(BezierSurface *b, Image *src, Color c, int divisions, int solid) {
    if (divisions < 1) {
        divisions = 1; // Ensure we have at least one division
    }

    double step = 1.0 / divisions;
    Point grid[divisions + 1][divisions + 1];

    // Compute the grid of points on the surface using de Casteljau's algorithm or explicit formula
    for (int i = 0; i <= divisions; i++) {
        for (int j = 0; j <= divisions; j++) {
            double u = i * step;
            double v = j * step;

            Point p, u_curve[4], v_curve[4];
            // Generate intermediate points for de Casteljau's algorithm in both directions

            for (int k = 0; k < 4; k++) {
                deCasteljau(b->p2[k], 4, u, &u_curve[k]);
            }
            deCasteljau(u_curve, 4, v, &p);
            grid[i][j] = p;
        }
    }

    // Draw the grid as filled polygons
    Polygon poly;
    polygon_init(&poly);
    Point vlist[4];

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            // Each cell in the grid is a quadrilateral
            vlist[0] = grid[i][j];
            vlist[1] = grid[i+1][j];
            vlist[2] = grid[i+1][j+1];
            vlist[3] = grid[i][j+1];

            polygon_set(&poly, 4, vlist);

            if (solid) {
                polygon_drawFill(&poly, src, c);
            } else {
                polygon_draw(&poly, src, c);
            }
        }
    }

    polygon_clear(&poly);
}


