#include "fsMath.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PRINT_MVP 0

void vector_init(Vector *v) {
        if (v == NULL) return;
        v->val[0] = 0;
        v->val[1] = 0;
        v->val[2] = 0;
        v->val[3] = 0;
}
void vector_set(Vector *v, double x, double y, double z) {
    v->val[0] = x;
    v->val[1] = y;
    v->val[2] = z;
    v->val[3] = 0.0;
}

void vector_print(Vector *v, FILE *fp) {
    fprintf(fp, "Vector: (%f, %f, %f, %f)\n", v->val[0], v->val[1], v->val[2], v->val[3]);
}

    void vector_copy(Vector *dest,Vector *src) {
    if (dest == NULL || src == NULL) {
        fprintf(stderr, "Error: Null pointer passed to vector_copy.\n");
        return;
    }
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

void vector_add(Vector *a, Vector *b, Vector *result) {
    result->val[0] = a->val[0] + b->val[0];
    result->val[1] = a->val[1] + b->val[1];
    result->val[2] = a->val[2] + b->val[2];
    result->val[3] = 0.0;
}
void vector_divide_num(Vector *a, float b) {
    a->val[0]/=b;
    a->val[1]/=b;
    a->val[2]/=b;
}

void vector_subtract(Vector *a, Vector *b, Vector *result) {
    result->val[0] = a->val[0] - b->val[0];
    result->val[1] = a->val[1] - b->val[1];
    result->val[2] = a->val[2] - b->val[2];
    result->val[3] = 0.0;
}
void vector_negate(Vector *a) {
    a->val[0]*=-1;
    a->val[1]*=-1;
    a->val[2]*=-1;
}

void vector_scale(Vector *v, double s, Vector *result) {
    result->val[0] = v->val[0] * s;
    result->val[1] = v->val[1] * s;
    result->val[2] = v->val[2] * s;
    result->val[3] = 0.0;
}
void normal_calculation(Point *p1, Point *p2, Point *p3, Vector *normal) {
    Vector u, v;
    // printf("\np1:");
    // point_print(p1,stdout);
    // printf("\np2:");
    // point_print(p2,stdout);
    // printf("\np3:");
    // point_print(p3,stdout);
    // Calculate vectors u and v
    u.val[0] = p2->val[0] - p1->val[0];
    u.val[1] = p2->val[1] - p1->val[1];
    u.val[2] = p2->val[2] - p1->val[2];

    v.val[0] = p3->val[0] - p1->val[0];
    v.val[1] = p3->val[1] - p1->val[1];
    v.val[2] = p3->val[2] - p1->val[2];
    // printf("\nu:");
    // vector_print(&u,stdout);
    // printf("\nv:");
    // vector_print(&v,stdout);
    // Calculate the cross product u x v
    vector_cross(&v,&u,normal);
    // printf("\n normal :");
    // vector_print(normal,stdout);

    // Normalize the normal vector
    double length = sqrt(normal->val[0] * normal->val[0] +
                         normal->val[1] * normal->val[1] +
                         normal->val[2] * normal->val[2]);
    // printf("\n length : %lf", length);
    if (length > 0.0) {
        normal->val[0] /= length;
        normal->val[1] /= length;
        normal->val[2] /= length;
    }
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
    Vector temp;
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


    void matrix_xformNormal(Matrix *m, Vector *p, Vector *q) {
    Vector temp;
    // Compute the normal transformation using the transpose of the upper-left 3x3 part of m
    for (int i = 0; i < 3; i++) {
        temp.val[i] = 0;
        for (int j = 0; j < 3; j++) {
            temp.val[i] += m->m[i][j] * p->val[j];
        }
    }
    // Copy the result back to q
    for (int i = 0; i < 3; i++) {
        q->val[i] = temp.val[i];
    }
    q->val[3] = 0;  // Ensure the fourth component is 0 for normal vectors

    // Normalize the resulting vector to ensure it's a unit vector
    float length = sqrt(q->val[0] * q->val[0] + q->val[1] * q->val[1] + q->val[2] * q->val[2]);
    if (length > 0) {
        for (int i = 0; i < 3; i++) {
            q->val[i] /= length;
        }
    }
}


    void matrix_xformPolygon(Matrix *m, Polygon *p) {
    for (int i = 0; i < p->numVertex; i++) {
        // Transform the vertex
        Point newPoint;
        matrix_xformPoint(m, &p->vertex[i], &newPoint);
        point_copy(&p->vertex[i], &newPoint);

        // Transform the normal if it exists
        if (p->normal) {
            Vector newNormal;
            matrix_xformNormal(m, &p->normal[i], &newNormal);
            vector_copy(&p->normal[i], &newNormal);
        }
    }
}
// verify
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
// verify
void matrix_rotateZ(Matrix *m, double cth, double sth) {
    Matrix rotate = {{{cth, -sth, 0, 0},
                      {sth, cth, 0, 0},
                      {0, 0, 1, 0},
                      {0, 0, 0, 1}}};

    Matrix result;
    matrix_multiply(&rotate, m, &result);
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
                           {0, 0, 1/d, 0}}};

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
    matrix_multiply( &scale,&trans2, vtm);
    matrix_multiply( &rotate,vtm, vtm);
    matrix_multiply( &trans,vtm,vtm);
}
void matrix_setView3D(Matrix *vtm, View3D *view) {
    // Step 1: Initialize VTM to the identity matrix
    matrix_identity(vtm);

    // Step 2: Translate VRP to the origin
    Matrix translate;
    matrix_identity(&translate);
    matrix_translate(&translate, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
    matrix_multiply(&translate, vtm, vtm);
#if PRINT_MVP
    printf("after VRP translation\n");
    matrix_print(vtm,stdout);
#endif
    // Step 3: VPN + VUP to get the rotate matrix
    Vector u, v, w;
    vector_copy(&w, &view->vpn);
    vector_normalize(&w);
    vector_cross(&view->vup, &w, &u);
    vector_normalize(&u);
    vector_cross(&w, &u, &v);
    vector_normalize(&v);
    // step 4 : form the rotate matrix
    Matrix rotate;
    matrix_identity(&rotate);
    rotate.m[0][0] = u.val[0]; rotate.m[0][1] = u.val[1]; rotate.m[0][2] = u.val[2];
    rotate.m[1][0] = v.val[0]; rotate.m[1][1] = v.val[1]; rotate.m[1][2] = v.val[2];
    rotate.m[2][0] = w.val[0]; rotate.m[2][1] = w.val[1]; rotate.m[2][2] = w.val[2];
    matrix_multiply(&rotate, vtm, vtm);
#if PRINT_MVP
    printf("View reference axes\n");
    matrix_print(vtm,stdout);
#endif
    // step 4.5 : translate to COP
    Matrix translateCOP;
    matrix_identity(&translateCOP);
    matrix_translate(&translateCOP,0,0,view->d);
    matrix_multiply(&translateCOP,vtm,vtm);
#if PRINT_MVP
    printf("after translating COP to origin\n");
    matrix_print(vtm,stdout);
#endif
    // Step 5: Scale to CVV (I thought what is CVV mean for several hours, finally find out its just NDC...)
    double depth = view->d + view->b;
    matrix_scale(vtm,2.0*view->d/(view->du * depth),2.0*view->d/(view->dv * depth),1.0 / depth);
#if PRINT_MVP
    printf("After scaling to CVV\n");
    matrix_print(vtm, stdout);
#endif
    // Step 6: Apply perspective projection
    matrix_perspective(vtm, view->d/depth);
#if PRINT_MVP
    printf("After perspective\n");
    matrix_print(vtm, stdout);
#endif
    // step 7 Scale to image coords
    matrix_scale(vtm, -view->screenx*0.5/(view->d / depth), -view->screeny*0.5/(view->d / depth), 1.0);
#if PRINT_MVP
    printf("After scale to image coords\n");
    matrix_print(vtm, stdout);
#endif
    // step 8 translate into place
    matrix_translate2D(vtm, view->screenx*0.5, view->screeny*0.5);
#if PRINT_MVP
    printf("After final translation to image coords\n");
    matrix_print(vtm, stdout);
#endif
}



void printCharArray(char *c, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", c[i]);
    }
}


#ifdef __cplusplus
}
#endif


