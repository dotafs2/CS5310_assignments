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
    if ((to == NULL) || (from == NULL)) return;
    if(from->vertex) {
        to->vertex = (Point *)malloc(from->numVertex * sizeof(Point));
        if (to->vertex == NULL) return;
        memcpy(to->vertex, from->vertex, from->numVertex * sizeof(Point));
    }  else {
        to->vertex = NULL;
    }

    if (from->color) {
        to->color = (Color *)malloc(from->numVertex * sizeof(Color));
        if (to->color == NULL) return;
        memcpy(to->color, from->color, from->numVertex * sizeof(Color));
    } else {
        to->color = NULL;
    }
    if (from->normal) {
        to->normal = (Vector *)malloc(from->numVertex * sizeof(Vector));
        if (to->normal == NULL) return;
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

    void swap_double(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void polygon_drawFill(Polygon *p, Image *src, Color c) {
    DrawState ds;
    ds.color = c;
    ds.shade = ShadeConstant;
    polygon_drawShade( p, src, &ds, NULL );
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

    void fillScanLine(Point p0, Point p1, Image *src, DrawState *ds) {
    // Start and end points of the line's coordinates
    int x0 = (int)p0.val[0];
    int y0 = (int)p0.val[1];
    int x1 = (int)p1.val[0];
    int y1 = (int)p1.val[1];
    // Depth of z0 and z1 in NDC
    float z0 = (float)p0.val[2];
    float z1 = (float)p1.val[2];


    // Ensure x0 is less than x1 for simplicity
    if (x0 > x1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;
        float tempz = z0;
        z0 = z1;
        z1 = tempz;
    }

    // Calculate the change in depth per pixel using 1/z for perspective projection
    float invZ0 = 1.0f / z0;
    float invZ1 = 1.0f / z1;
    float deltaInvZ = (invZ1 - invZ0) / (float)(x1 - x0);

    // Start from invZ0
    float invZ = invZ0;
    for (int x = x0; x <= x1; x++) {
        int targetIndex = y0 * src->cols + x;
        FPixel* targetPixel = &src->data[targetIndex];
        // Z-buffer test
        if (invZ > src->depth[targetIndex]) {

            // Update the z-buffer value
            src->depth[targetIndex] = invZ;

            // Set the color based on the shading mode
            switch(ds->shade) {
                case ShadeDepth:
                    targetPixel->rgb[0] = ds->color.c[0] * (1.0f - 1/invZ);
                targetPixel->rgb[1] = ds->color.c[1] * (1.0f - 1/invZ);
                targetPixel->rgb[2] = ds->color.c[2] * (1.0f - 1/invZ);
                break;
                default:
                    targetPixel->rgb[0] = ds->color.c[0];
                    targetPixel->rgb[1] = ds->color.c[1];
                    targetPixel->rgb[2] = ds->color.c[2];
                break;
            }
        }

        // Update the current z value
        invZ += deltaInvZ;
    }
}

void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light) {
    // Find the minimum and maximum y values in the polygon to determine the range of y.
    int minY = src->rows, maxY = 0;
    for (int i = 0; i < p->numVertex; i++) {
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }

    // Loop through each y from minY to maxY to process scanlines.
    for (int y = minY; y <= maxY; y++) {
        int intersections[10]; // Array to store intersection points for the current scanline.
        double intersectionsZ[10];
        int numIntersections = 0; // Counter for the number of intersections.

        // Find intersections of the scanline y with all polygon edges.
        for (int i = 0; i < p->numVertex; i++) {
            Point v1 = p->vertex[i];
            Point v2 = p->vertex[(i + 1) % p->numVertex];
            if ((v1.val[1] <= y && v2.val[1] > y) || (v1.val[1] > y && v2.val[1] <= y)) {
                double t = (y - v1.val[1]) / (v2.val[1] - v1.val[1]);
                int x = (int)(v1.val[0] + t * (v2.val[0] - v1.val[0]));
                double z = v1.val[2] + t * (v2.val[2] - v1.val[2]);
                intersections[numIntersections] = x;
                intersectionsZ[numIntersections] = z;
                numIntersections++;
            }
        }

        // Sort the intersections by x coordinate.
        for (int i = 0; i < numIntersections - 1; i++) {
            for (int j = 0; j < numIntersections - i - 1; j++) {
                if (intersections[j] > intersections[j + 1]) {
                    swap_int(&intersections[j], &intersections[j + 1]);
                    swap_double(&intersectionsZ[j], &intersectionsZ[j + 1]);
                }
            }
        }

        // Fill the scanline by drawing lines between pairs of intersection points.
        for (int i = 0; i < numIntersections; i += 2) {
            Point start = {{(float)intersections[i], (float)y, (float)intersectionsZ[i]}};
            Point end = {{(float)intersections[i + 1], (float)y, (float)intersectionsZ[i + 1]}};
            fillScanLine(start, end, src, ds);
        }
    }
}



#ifdef __cplusplus
}
#endif
