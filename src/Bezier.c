//
// Created by DOTAFS on 2024/6/29.
//
#include "Bezier.h"


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
    int i, j, k, l;
    int num_points = 4;
    int new_points = num_points * (1 << divisions);
    Point temp[new_points][new_points];

    // Copy initial control points
    for (i = 0; i < num_points; i++) {
        for (j = 0; j < num_points; j++) {
            temp[i][j] = b->p2[i][j];
        }
    }

    // Subdivision using de Casteljau's algorithm
    for (int d = 0; d < divisions; d++) {
        int old_points = num_points * (1 << d);
        int new_points = old_points * 2 - 1;
        Point temp2[new_points][new_points];

        // Subdivide each row
        for (i = 0; i < old_points; i++) {
            for (j = 0; j < old_points - 1; j++) {
                Point mid;
                deCasteljau(temp[i] + j, 2, 0.5, &mid);
                temp2[i * 2][j * 2] = temp[i][j];
                temp2[i * 2][j * 2 + 1] = mid;
            }
            temp2[i * 2][old_points * 2 - 2] = temp[i][old_points - 1];
        }

        // Subdivide each column
        for (j = 0; j < new_points; j++) {
            for (i = 0; i < old_points - 1; i++) {
                Point mid;
                deCasteljau(temp2 + i * 2 + j, 2, 0.5, &mid);
                temp2[i * 2][j] = temp2[i * 2][j];
                temp2[i * 2 + 1][j] = mid;
            }
            temp2[old_points * 2 - 2][j] = temp[old_points - 1][j];
        }

        // Copy the new points back to temp
        for (i = 0; i < new_points; i++) {
            for (j = 0; j < new_points; j++) {
                temp[i][j] = temp2[i][j];
            }
        }
    }

    // Draw the surface
    if (solid) {
        // Draw triangles
        for (i = 0; i < new_points - 1; i++) {
            for (j = 0; j < new_points - 1; j++) {
                // Triangle 1
                Line line;
                line.a = temp[i][j];
                line.b = temp[i + 1][j];
                line_draw(&line, src, c);

                line.a = temp[i + 1][j];
                line.b = temp[i + 1][j + 1];
                line_draw(&line, src, c);

                line.a = temp[i + 1][j + 1];
                line.b = temp[i][j];
                line_draw(&line, src, c);

                // Triangle 2
                line.a = temp[i][j];
                line.b = temp[i][j + 1];
                line_draw(&line, src, c);

                line.a = temp[i][j + 1];
                line.b = temp[i + 1][j + 1];
                line_draw(&line, src, c);

                line.a = temp[i + 1][j + 1];
                line.b = temp[i][j];
                line_draw(&line, src, c);
            }
        }
    } else {
        // Draw lines connecting control points
        for (i = 0; i < new_points; i++) {
            for (j = 0; j < new_points - 1; j++) {
                Line line;
                line.a = temp[i][j];
                line.b = temp[i][j + 1];
                line_draw(&line, src, c);
            }
        }

        for (j = 0; j < new_points; j++) {
            for (i = 0; i < new_points - 1; i++) {
                Line line;
                line.a = temp[i][j];
                line.b = temp[i + 1][j];
                line_draw(&line, src, c);
            }
        }
    }
}


