#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"
#include "Polygon.h"

#ifdef __cplusplus
extern "C" {
#endif
// Function to create an empty Polygon
Polygon *polygon_create() {
    Polygon *p = (Polygon *)malloc(sizeof(Polygon));
    if (!p) return NULL;
    p->numVertex = 0;
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->zBuffer = 1;
    p->oneSided = 0;
    return p;
}

// Function to create a Polygon with a given number of vertices
Polygon *polygon_createp(int numV, Point *vlist) {
    Polygon *p = polygon_create();
    if (!p) return NULL;
    polygon_set(p, numV, vlist);
    return p;
}

// Function to free the memory of a Polygon
void polygon_free(Polygon *p) {
    if (!p) return;
    free(p->vertex);
    free(p->color);
    free(p->normal);
    free(p);
}

// Function to initialize an existing Polygon to an empty Polygon
void polygon_init(Polygon *p) {
    if (!p) return;
    p->numVertex = 0;
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->zBuffer = 1;
    p->oneSided = 0;
}

// Function to set the vertices of a Polygon
void polygon_set(Polygon *p, int numV, Point *vlist) {
    if (!p) return;
    if (p->vertex) free(p->vertex);
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

// Function to clear the internal data of a Polygon
void polygon_clear(Polygon *p) {
    if (!p) return;
    free(p->vertex);
    free(p->color);
    free(p->normal);
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->numVertex = 0;
}

// Function to set the oneSided field of a Polygon
void polygon_setSided(Polygon *p, int oneSided) {
    if (!p) return;
    p->oneSided = oneSided;
}

// Function to set the colors of a Polygon
void polygon_setColors(Polygon *p, int numV, Color *clist) {
    if (!p) return;
    if (p->color) free(p->color);
    p->color = (Color *)malloc(numV * sizeof(Color));
    if (!p->color) return;
    for (int i = 0; i < numV; i++) {
        p->color[i] = clist[i];
    }
}

// Function to set the normals of a Polygon
void polygon_setNormals(Polygon *p, int numV, Vector *nlist) {
    if (!p) return;
    if (p->normal) free(p->normal);
    p->normal = (Vector *)malloc(numV * sizeof(Vector));
    if (!p->normal) return;
    for (int i = 0; i < numV; i++) {
        p->normal[i] = nlist[i];
    }
}

// Function to set all properties of a Polygon
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided) {
    if (!p) return;
    polygon_set(p, numV, vlist);
    polygon_setColors(p, numV, clist);
    polygon_setNormals(p, numV, nlist);
    p->zBuffer = zBuffer;
    p->oneSided = oneSided;
}

// Function to set the z-buffer flag of a Polygon
void polygon_zBuffer(Polygon *p, int flag) {
    if (!p) return;
    p->zBuffer = flag;
}

// Function to copy one Polygon to another
void polygon_copy(Polygon *to, Polygon *from) {
    if (!to || !from) return;
    polygon_clear(to);
    polygon_set(to, from->numVertex, from->vertex);
    polygon_setColors(to, from->numVertex, from->color);
    polygon_setNormals(to, from->numVertex, from->normal);
    to->zBuffer = from->zBuffer;
    to->oneSided = from->oneSided;
}

// Function to print the data of a Polygon
void polygon_print(Polygon *p, FILE *fp) {
    if (!p || !fp) return;
    fprintf(fp, "Polygon with %d vertices:\n", p->numVertex);
    for (int i = 0; i < p->numVertex; i++) {
        fprintf(fp, "  Vertex %d: (%f, %f, %f, %f)\n", i, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
    }
}

// Function to normalize the vertices of a Polygon
void polygon_normalize(Polygon *p) {
    if (!p) return;
    for (int i = 0; i < p->numVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}


// 辅助函数：计算两点之间的斜率
float edge_slope(Point a, Point b) {
    if (a.val[1] == b.val[1]) return 0;
    return (b.val[0] - a.val[0]) / (b.val[1] - a.val[1]);
}

// 辅助函数：交换两个整数
void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 辅助函数：绘制水平线
void draw_horizontal_line(Image *src, int y, int x1, int x2, Color c) {
    if (x1 > x2) swap_int(&x1, &x2);
    for (int x = x1; x <= x2; x++) {
        FPixel pixel = { {c.c[0], c.c[1], c.c[2]}, 1.0f, 1.0f };
        src->data[y * src->cols + x] = pixel;
    }
}

// 多边形填充算法实现
void polygon_drawFill(Polygon *p, Image *src, Color c) {
    if (!p || !src || p->numVertex < 3) return;

    // 找到多边形的最小和最大Y值
    int minY = src->rows, maxY = 0;
    for (int i = 0; i < p->numVertex; i++) {
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }

    // 为每个扫描线存储交点
    for (int y = minY; y <= maxY; y++) {
        int intersections[10]; // 假设多边形顶点数最多为10
        int numIntersections = 0;

        for (int i = 0; i < p->numVertex; i++) {
            Point v1 = p->vertex[i];
            Point v2 = p->vertex[(i + 1) % p->numVertex];

            if ((v1.val[1] <= y && v2.val[1] > y) || (v1.val[1] > y && v2.val[1] <= y)) {
                float slope = edge_slope(v1, v2);
                int x = (int)(v1.val[0] + slope * (y - v1.val[1]));
                intersections[numIntersections++] = x;
            }
        }

        // 排序交点
        for (int i = 0; i < numIntersections - 1; i++) {
            for (int j = 0; j < numIntersections - i - 1; j++) {
                if (intersections[j] > intersections[j + 1]) {
                    swap_int(&intersections[j], &intersections[j + 1]);
                }
            }
        }

        // 绘制扫描线
        for (int i = 0; i < numIntersections; i += 2) {
            draw_horizontal_line(src, y, intersections[i], intersections[i + 1], c);
        }
    }
}

// Function to fill a Polygon using the Barycentric coordinates algorithm
void polygon_drawFillB(Polygon *p, Image *src, Color c) {
    if (!p || !src || p->numVertex < 3) return;

    int minX = src->cols, minY = src->rows, maxX = 0, maxY = 0;
    for (int i = 0; i < p->numVertex; i++) {
        if (p->vertex[i].val[0] < minX) minX = (int)p->vertex[i].val[0];
        if (p->vertex[i].val[0] > maxX) maxX = (int)p->vertex[i].val[0];
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float alpha, beta, gamma;
            if (barycentric(p->vertex, x, y, &alpha, &beta, &gamma)) {
                FPixel pixel = { {c.c[0], c.c[1], c.c[2]}, 1.0f, 1.0f };
                src->data[y * src->cols + x] = pixel;
            }
        }
    }
}

// Helper function to compute Barycentric coordinates
int barycentric(Point *vlist, int px, int py, float *alpha, float *beta, float *gamma) {
    float denom = (vlist[1].val[1] - vlist[2].val[1]) * (vlist[0].val[0] - vlist[2].val[0]) +
                  (vlist[2].val[0] - vlist[1].val[0]) * (vlist[0].val[1] - vlist[2].val[1]);

    *alpha = ((vlist[1].val[1] - vlist[2].val[1]) * (px - vlist[2].val[0]) +
              (vlist[2].val[0] - vlist[1].val[0]) * (py - vlist[2].val[1])) / denom;
    *beta = ((vlist[2].val[1] - vlist[0].val[1]) * (px - vlist[2].val[0]) +
             (vlist[0].val[0] - vlist[2].val[0]) * (py - vlist[2].val[1])) / denom;
    *gamma = 1.0f - *alpha - *beta;

    return (*alpha >= 0.0f && *beta >= 0.0f && *gamma >= 0.0f);
}

#ifdef __cplusplus
}
#endif
