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
    if(p->vertex)
    free(p->vertex);
    if(p->color)
    free(p->color);
    if(p->normal)
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
    p->vertex = NULL;
    free(p->color);
    p->color = NULL;
    free(p->normal);
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
    // Allocate new memory and copy the data
    to->vertex = (Point *)malloc(from->numVertex * sizeof(Point));
    if (!to->vertex) return;
    memcpy(to->vertex, from->vertex, from->numVertex * sizeof(Point));

    if (from->color) {
        to->color = (Color *)malloc(from->numVertex * sizeof(Color));
        if (!to->color) return;
        memcpy(to->color, from->color, from->numVertex * sizeof(Color));
    } else {
        to->color = NULL;
    }
    if (from->normal) {
        to->normal = (Vector *)malloc(from->numVertex * sizeof(Vector));
        if (!to->normal) return;
        memcpy(to->normal, from->normal, from->numVertex * sizeof(Vector));
    } else {
        to->normal = NULL;
    }

    to->numVertex = from->numVertex;
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

float edge_slope(Point a, Point b) {
    if (a.val[1] == b.val[1]) return 0;
    return (b.val[0] - a.val[0]) / (b.val[1] - a.val[1]);
}

void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


void polygon_drawFill(Polygon *p, Image *src, Color c) {
    if (!p || !src || p->numVertex < 3) return;

    // Find the minimum and maximum y values in the polygon to determine the range of y.
    int minY = src->rows, maxY = 0;
    for (int i = 0; i < p->numVertex; i++) {
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }
    printf("minY:%d maxY: %d", minY, maxY);

    // Loop through each y from minY to maxY to process scanlines.
    for (int y = minY; y <= maxY; y++) {
        int intersections[10]; // Array to store intersection points for the current scanline.
        int numIntersections = 0; // Counter for the number of intersections.

        // Find intersections of the scanline y with all polygon edges.
        for (int i = 0; i < p->numVertex; i++) {
            Point v1 = p->vertex[i]; // First vertex of the edge.
            Point v2 = p->vertex[(i + 1) % p->numVertex]; // Second vertex, wrapped around to the start of the polygon.

            // Check if the edge intersects with the scanline y.
            if ((v1.val[1] <= y && v2.val[1] > y) || (v1.val[1] > y && v2.val[1] <= y)) {
                float slope = edge_slope(v1, v2); // Calculate the slope of the edge.
                int x = (int)(v1.val[0] + slope * (y - v1.val[1])); // Calculate the x coordinate of the intersection.
                intersections[numIntersections++] = x; // Store the intersection point.
            }
        }

        // Sort the intersections by x coordinate.
        for (int i = 0; i < numIntersections - 1; i++) {
            for (int j = 0; j < numIntersections - i - 1; j++) {
                if (intersections[j] > intersections[j + 1]) {
                    swap_int(&intersections[j], &intersections[j + 1]);
                }
            }
        }

        //TODO I will add shade method here.
        // Fill the scanline by drawing lines between pairs of intersection points.
        for (int i = 0; i < numIntersections; i += 2) {
            Point start = {{(float)intersections[i], (float)y, 1.0}};
            Point end = {{(float)intersections[i + 1], (float)y, 1.0}};
            Line line;
            line_set(&line, start, end);
            line_draw(&line, src, c);
        }
    }
}

void polygon_draw(Polygon *p, Image *src, Color c) {
    if (!p || !src || p->numVertex < 2) return;

    Line line;
    for (int i = 0; i < p->numVertex; i++) {
        int next = (i + 1) % p->numVertex;
        line_set(&line, p->vertex[i], p->vertex[next]);
        line_draw(&line, src, c);
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



  void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light) {
    if (p == NULL || src == NULL || ds == NULL) return;
Color c = {0.5,0.5,0.5};
    polygon_drawFill(p,src,c);
}



#ifdef __cplusplus
}
#endif
