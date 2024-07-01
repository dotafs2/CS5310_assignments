#include "fsMath.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void vector_set(Vector *v, double x, double y, double z) {
    v->val[0] = x;
    v->val[1] = y;
    v->val[2] = z;
    v->val[3] = 0.0;
}

void vector_print(Vector *v, FILE *fp) {
    fprintf(fp, "Vector: (%f, %f, %f, %f)\n", v->val[0], v->val[1], v->val[2], v->val[3]);
}

void vector_copy(Vector *dest, Vector *src) {
    memcpy(dest->val, src->val, 4 * sizeof(double));
}

double vector_length(Vector *v) {
    return sqrt(v->val[0] * v->val[0] + v->val[1] * v->val[1] + v->val[2] * v->val[2]);
}

void vector_normalize(Vector *v) {
    double length = vector_length(v);
    if (length > 0) {
        v->val[0] /= length;
        v->val[1] /= length;
        v->val[2] /= length;
    }
}

double vector_dot(Vector *a, Vector *b) {
    return a->val[0] * b->val[0] + a->val[1] * b->val[1] + a->val[2] * b->val[2];
}

void vector_cross(Vector *a, Vector *b, Vector *c) {
    c->val[0] = a->val[1] * b->val[2] - a->val[2] * b->val[1];
    c->val[1] = a->val[2] * b->val[0] - a->val[0] * b->val[2];
    c->val[2] = a->val[0] * b->val[1] - a->val[1] * b->val[0];
    c->val[3] = 0.0;
}

void matrix_print(Matrix *m, FILE *fp) {
    for (int i = 0; i < 4; i++) {
        fprintf(fp, "[ ");
        for (int j = 0; j < 4; j++) {
            fprintf(fp, "%f ", m->m[i][j]);
        }
        fprintf(fp, "]\n");
    }
    fprintf(fp, "\n");
}

void matrix_clear(Matrix *m) {
    memset(m->m, 0, 16 * sizeof(double));
}

void matrix_identity(Matrix *m) {
    matrix_clear(m);
    for (int i = 0; i < 4; i++) {
        m->m[i][i] = 1.0;
    }
}

double matrix_get(Matrix *m, int r, int c) {
    return m->m[r][c];
}

void matrix_set(Matrix *m, int r, int c, double v) {
    m->m[r][c] = v;
}

void matrix_copy(Matrix *dest, Matrix *src) {
    memcpy(dest->m, src->m, 16 * sizeof(double));
}

void matrix_transpose(Matrix *m) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            double temp = m->m[i][j];
            m->m[i][j] = m->m[j][i];
            m->m[j][i] = temp;
        }
    }
}

void matrix_multiply(Matrix *left, Matrix *right, Matrix *m) {
    Matrix result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += left->m[i][k] * right->m[k][j];
            }
        }
    }
    matrix_copy(m, &result);
}

void matrix_xformPoint(Matrix *m, Point *p, Point *q) {
    Point temp;
    for (int i = 0; i < 4; i++) {
        temp.val[i] = 0;
        for (int j = 0; j < 4; j++) {
            temp.val[i] += m->m[i][j] * p->val[j];
        }
    }
    for (int i = 0; i < 4; i++) {
        q->val[i] = temp.val[i];
    }
}


void matrix_xformVector(Matrix *m, Vector *p, Vector *q) {
    for (int i = 0; i < 4; i++) {
        q->val[i] = 0;
        for (int j = 0; j < 4; j++) {
            q->val[i] += m->m[i][j] * p->val[j];
        }
    }
}

void matrix_xformPolygon(Matrix *m, Polygon *p) {
    for (int i = 0; i < p->numVertex; i++) {
        printf("1");
        Point newPoint;
        matrix_xformPoint(m, &p->vertex[i], &newPoint);
        point_copy(&p->vertex[i], &newPoint);
    }
}

void matrix_xformPolyline(Matrix *m, Polyline *p) {
    for (int i = 0; i < p->numVertex; i++) {
        Point newPoint;
        matrix_xformPoint(m, &p->vertex[i], &newPoint);
        point_copy(&p->vertex[i], &newPoint);
    }
}

void matrix_xformLine(Matrix *m, Line *line) {
    Point newPointA, newPointB;
    matrix_xformPoint(m, &line->a, &newPointA);
    matrix_xformPoint(m, &line->b, &newPointB);
    point_copy(&line->a, &newPointA);
    point_copy(&line->b, &newPointB);
}

void matrix_scale2D(Matrix *m, double sx, double sy) {
    Matrix scale = {{{sx, 0, 0, 0},
                     {0, sy, 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};

    matrix_multiply(&scale,m, m);
}

void matrix_rotateZ(Matrix *m, double cth, double sth) {
    Matrix rotate = {{{cth, -sth, 0, 0},
                      {sth, cth, 0, 0},
                      {0, 0, 1, 0},
                      {0, 0, 0, 1}}};

    Matrix result;
    matrix_multiply(&rotate, m, &result);

    // Copy to make sure no bug
    *m = result;
}

void matrix_translate2D(Matrix *m, double tx, double ty) {
    Matrix translate = {{{1, 0, 0, tx},
                         {0, 1, 0, ty},
                         {0, 0, 1, 0},
                         {0, 0, 0, 1}}};

    // Multiply the translation matrix on the left of the input matrix
    Matrix result;
    matrix_multiply(&translate, m, &result);

    // Copy the result back to the input matrix
    *m = result;
}
void matrix_shear2D(Matrix *m, double shx, double shy) {
    Matrix shear = {{{1, shx, 0, 0},
                     {shy, 1, 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};
    Matrix result;
    matrix_multiply(&shear, m, &result);

    *m = result;
}

void matrix_translate(Matrix *m, double tx, double ty, double tz) {
    Matrix translate = {{{1, 0, 0, tx},
                         {0, 1, 0, ty},
                         {0, 0, 1, tz},
                         {0, 0, 0, 1}}};

    matrix_multiply(&translate, m, m);
}

void matrix_scale(Matrix *m, double sx, double sy, double sz) {
    Matrix scale = {{{sx, 0, 0, 0},
                     {0, sy, 0, 0},
                     {0, 0, sz, 0},
                     {0, 0, 0, 1}}};
    Matrix result;
    matrix_multiply(&scale, m, &result);

    *m = result;
}

void matrix_rotateX(Matrix *m, double cth, double sth) {
    Matrix rotate = {{{1, 0, 0, 0},
                      {0, cth, -sth, 0},
                      {0, sth, cth, 0},
                      {0, 0, 0, 1}}};

    matrix_multiply(&rotate, m, m);
}

void matrix_rotateY(Matrix *m, double cth, double sth) {
    Matrix rotate = {{{cth, 0, sth, 0},
                      {0, 1, 0, 0},
                      {-sth, 0, cth, 0},
                      {0, 0, 0, 1}}};

    matrix_multiply(&rotate, m, m);
}

void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w) {
    Matrix rotate = {{{u->val[0], u->val[1], u->val[2], 0},
                      {v->val[0], v->val[1], v->val[2], 0},
                      {w->val[0], w->val[1], w->val[2], 0},
                      {0, 0, 0, 1}}};

    matrix_multiply(&rotate, m, m);
}


// Matrix Functions

void matrix_shearZ(Matrix *m, double shx, double shy) {
    Matrix shear = {{{1, 0, shx, 0},
                     {0, 1, shy, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};

    matrix_multiply(&shear, m, m);
}

void matrix_perspective(Matrix *m, double d) {
    Matrix perspective = {{{1, 0, 0, 0},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 1/d, 1}}};

    matrix_multiply(&perspective, m, m);
}


// Function to set the VTM to the view specified by the View2D structure
void matrix_setView2D(Matrix *vtm, View2D *view) {

    // The height of the view rectangle in world coordinates
    double dy = view->dx * view->screeny / view->screenx;

    // Step 1: T(-V0x,-V0y)
    Matrix trans;
    matrix_identity(&trans);
    matrix_translate2D(&trans, -view->vrp.val[0], -view->vrp.val[1]);

    // Step 2: R(nx,-ny)
    Matrix rotate;
    matrix_identity(&rotate);
    matrix_rotateZ(&rotate, view->x.val[0], -view->x.val[1]);

    // Step 3: S(C/du,-R/dv)
    Matrix scale;
    matrix_identity(&scale);
    matrix_scale2D(&scale, view->screenx / view->dx, -view->screeny / dy);

    // Step 4: T(C/2,R/2)
    Matrix trans2;
    matrix_identity(&trans2);
    matrix_translate2D(&trans2, view->screenx / 2.0, view->screeny / 2.0);

    // combine
    matrix_multiply(&trans2, &scale, vtm);
    matrix_multiply(vtm, &rotate, vtm);
    matrix_multiply(vtm, &trans, vtm);
}

void matrix_setView3D(Matrix *vtm, View3D *view) {
    // Step 1: Initialize VTM to the identity matrix
    matrix_identity(vtm);

    // Step 2: Translate VRP to the origin
    Matrix translate;
    matrix_identity(&translate);
    matrix_translate(&translate, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
    matrix_multiply(&translate, vtm, vtm);

    // Step 3: Align VPN with the z-axis
    Vector u, v, n;
    vector_copy(&n, &view->vpn);
    vector_normalize(&n);
    vector_cross(&view->vup, &n, &u);
    vector_normalize(&u);
    vector_cross(&n, &u, &v);

    Matrix align;
    matrix_identity(&align);
    align.m[0][0] = u.val[0]; align.m[0][1] = u.val[1]; align.m[0][2] = u.val[2];
    align.m[1][0] = v.val[0]; align.m[1][1] = v.val[1]; align.m[1][2] = v.val[2];
    align.m[2][0] = n.val[0]; align.m[2][1] = n.val[1]; align.m[2][2] = n.val[2];
    matrix_multiply(&align, vtm, vtm);

    // Step 4: Apply perspective projection
    Matrix perspective;
    matrix_identity(&perspective);
    perspective.m[0][0] = 2.0 * view->d / view->du;
    perspective.m[1][1] = 2.0 * view->d / view->dv;
    perspective.m[2][2] = view->b / (view->b - view->f);
    perspective.m[2][3] = -view->b * view->f / (view->b - view->f);
    perspective.m[3][2] = 1.0 / view->d;
    perspective.m[3][3] = 0.0;
    matrix_multiply(&perspective, vtm, vtm);

    // Step 5: Scale to viewport
    Matrix scale;
    matrix_identity(&scale);
    scale.m[0][0] = view->screenx / 2.0;
    scale.m[1][1] = view->screeny / 2.0;
    scale.m[2][2] = 1.0;
    matrix_multiply(&scale, vtm, vtm);

    // Step 6: Translate to screen coordinates
    Matrix translate2;
    matrix_identity(&translate2);
    matrix_translate(&translate2, view->screenx / 2.0, view->screeny / 2.0, 0);
    matrix_multiply(&translate2, vtm, vtm);
}


#ifdef __cplusplus
}
#endif
