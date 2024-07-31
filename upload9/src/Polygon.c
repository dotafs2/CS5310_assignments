#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"
#include "Polygon.h"
#include "fsMath.h"
#ifdef __cplusplus
extern "C" {
#endif

// Function to create an empty Polygon
    Polygon *polygon_create() {
        Polygon *p = malloc( sizeof(Polygon ) );
        polygon_init( p );
        return(p);
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
    p->zBuffer = 0;
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
    if (p == NULL) return;
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
    if (p == NULL) return;
    p->oneSided = oneSided;
}

    void polygon_setColors(Polygon *p, int numV, Color *clist){
    if( numV == p->numVertex ) {
        if( p->color == NULL ) {
            p->color = malloc(sizeof(Color) * numV );
        }
        memcpy( p->color, clist, sizeof(Color) * numV );
    }
}

    void polygon_setNormals(Polygon *p, int numV, Vector *nlist) {
    if (p == NULL) return;
    if (p->normal) free(p->normal);
    p->normal = (Vector *)malloc(numV * sizeof(Vector));
    if (!p->normal) return;
    for (int i = 0; i < numV; i++) {
        memcpy(&(p->normal[i]), &(nlist[i]), sizeof(Vector));
    }
}


void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided){
    if(p != NULL) {
        polygon_clear(p);
        if( numV > 0 ) {
            p->vertex = malloc(sizeof(Point) * numV );
            memcpy( p->vertex, vlist, sizeof(Point) * numV );
            if( clist != NULL ) {
                p->color = malloc(sizeof(Color) * numV );
                memcpy( p->color, clist, sizeof(Color) * numV );
            }

            if( nlist != NULL ) {
                p->normal = malloc(sizeof(Vector) * numV );
                memcpy( p->normal, nlist, sizeof(Vector) * numV );
            }

        }
        p->numVertex = numV;
        p->zBuffer = zBuffer;
        p->oneSided = oneSided;
    }
}
// Function to set the z-buffer flag of a Polygon
void polygon_zBuffer(Polygon *p, int flag) {
    if (p==NULL) return;
    p->zBuffer = flag;
}

void polygon_copy( Polygon *to, Polygon *from ) {
    polygon_setAll( to, from->numVertex, from->vertex, from->color, from->normal, from->zBuffer, from->oneSided );
}

// Function to print the data of a Polygon
    void polygon_print(Polygon *p, FILE *fp) {
    if ((p == NULL) || (fp == NULL)) return;
    fprintf(fp, "Polygon with %d vertices:\n", p->numVertex);
    fprintf(fp, "  oneSided: %d\n", p->oneSided);
    fprintf(fp, "  zBuffer: %d\n", p->zBuffer);

    for (int i = 0; i < p->numVertex; i++) {
        fprintf(fp, "  Vertex %d: (%f, %f, %f, %f)\n", i, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
         if (p->color) {
             fprintf(fp, "    Color: (%f, %f, %f)\n", p->color[i].c[0], p->color[i].c[1], p->color[i].c[2]);
         } else {
             fprintf(fp, "    Color: None\n");
        }
        if (p->normal) {
            fprintf(fp, "    Normal: (%f, %f, %f, %f)\n", p->normal[i].val[0], p->normal[i].val[1], p->normal[i].val[2], p->normal[i].val[3]);
        } else {
            fprintf(fp, "    Normal: None\n");
        }
    }
}

// Function to normalize the vertices of a Polygon
void polygon_normalize(Polygon *p) {
    if (p == NULL) return;
        for(int i=0;i<p->numVertex;i++) {
            p->vertex[i].val[0] /= p->vertex[i].val[3];
            p->vertex[i].val[1] /= p->vertex[i].val[3];
            p->vertex[i].val[3] = 1.0;
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
    if ((p == NULL)|| (src == NULL) || p->numVertex < 2) return;

    Line line;
    for (int i = 0; i < p->numVertex; i++) {
        int next = (i + 1) % p->numVertex;
        line_set(&line, p->vertex[i], p->vertex[next]);
        line_draw(&line, src, c);
    }
}

void polygon_shade( Polygon *p, Lighting *l, DrawState *ds ) {
    if(ds == NULL || l == NULL || p == NULL) return;
        if( p->color == NULL ) p->color = malloc( sizeof(Color) * p->numVertex );

    switch(ds->shade) {
        case ShadeGouraud:
            for(int i=0; i<p->numVertex; i++) {
                Vector V;
                V.val[0] = ds->viewer.val[0] - p->vertex[i].val[0];
                V.val[1] = ds->viewer.val[1] - p->vertex[i].val[1];
                V.val[2] = ds->viewer.val[2] - p->vertex[i].val[2];
                lighting_shading( l, &(p->normal[i]), &V, &(p->vertex[i]), &(ds->body), &(ds->surface), ds->surfaceCoeff, p->oneSided, &(p->color[i]) );
            }

        break;
        default:
            break;
    }
}

// Helper function to compute Barycentric coordinates
int barycentric(Point *vlist, int px, int py, float *alpha, float *beta, float *gamma) {
    double denom = (vlist[1].val[1] - vlist[2].val[1]) * (vlist[0].val[0] - vlist[2].val[0]) +
                  (vlist[2].val[0] - vlist[1].val[0]) * (vlist[0].val[1] - vlist[2].val[1]);

    *alpha = ((vlist[1].val[1] - vlist[2].val[1]) * (px - vlist[2].val[0]) +
              (vlist[2].val[0] - vlist[1].val[0]) * (py - vlist[2].val[1])) / denom;
    *beta = ((vlist[2].val[1] - vlist[0].val[1]) * (px - vlist[2].val[0]) +
             (vlist[0].val[0] - vlist[2].val[0]) * (py - vlist[2].val[1])) / denom;
    *gamma = 1.0f - *alpha - *beta;

    return (*alpha >= 0.0f && *beta >= 0.0f && *gamma >= 0.0f);
}

    void fillScanLine(Point p0, Point p1, Color c0, Color c1, Image *src, DrawState *ds, int zbuffer) {
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
    Color deltaColor;
    for (int i = 0; i < 3; i++) {
        deltaColor.c[i] = (c1.c[i] - c0.c[i]) / (float)(x1 - x0);
    }
    // Start from invZ0 and c1
    float invZ = invZ0;
    Color currentColor = c0;

    for (int x = x0; x <= x1; x++) {
        int targetIndex = y0 * src->cols + x;
        FPixel* targetPixel = &src->data[targetIndex];
        // if do not add z-buffer test
        if(!zbuffer) {
            targetPixel->rgb[0] = currentColor.c[0];
            targetPixel->rgb[1] = currentColor.c[1];
            targetPixel->rgb[2] = currentColor.c[2];
            printf("color%f,%f,%f\n",targetPixel->rgb[0],targetPixel->rgb[1],targetPixel->rgb[2]);
        }else {
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
                    case ShadeGouraud:
                        targetPixel->rgb[0] = currentColor.c[0];
                        targetPixel->rgb[1] = currentColor.c[1];
                        targetPixel->rgb[2] = currentColor.c[2];
                    break;
                    default:
                        targetPixel->rgb[0] = currentColor.c[0];
                        targetPixel->rgb[1] = currentColor.c[1];
                        targetPixel->rgb[2] = currentColor.c[2];
                    break;
                }
            }
        }

        // Update the current z value
        invZ += deltaInvZ;
    // Update the current color
         for (int i = 0; i < 3; i++) {
        currentColor.c[i] += deltaColor.c[i];
    }
    }
}

void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light) {
    // Find the minimum and maximum y values in the polygon to determine the range of y.
    int minY = src->rows, maxY = 0;
    Color vertexColors[p->numVertex];

    for (int i = 0; i < p->numVertex; i++) {
        // calculate vertex color
        vertexColors[i] = p->color ? p->color[i] : ds->color;
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }


    // Loop through each y from minY to maxY to process scanlines.
    for (int y = minY; y <= maxY; y++) {
        int intersections[10]; // Array to store intersection points for the current scanline.
        double intersectionsZ[10];
        Color intersectionsC[10];
        int numIntersections = 0; // Counter for the number of intersections.

        // Find intersections of the scanline y with all polygon edges.
        for (int i = 0; i < p->numVertex; i++) {
            Point v1 = p->vertex[i];
            Point v2 = p->vertex[(i + 1) % p->numVertex];
            Color c1 = vertexColors[i];
            Color c2 = vertexColors[(i + 1) % p->numVertex];
            if ((v1.val[1] <= y && v2.val[1] > y) || (v1.val[1] > y && v2.val[1] <= y)) {
                double t = (y - v1.val[1]) / (v2.val[1] - v1.val[1]);
                int x = (int)(v1.val[0] + t * (v2.val[0] - v1.val[0]));
                double z = v1.val[2] + t * (v2.val[2] - v1.val[2]);

                Color c;
                for (int j = 0; j < 3; j++) {
                    c.c[j] = c1.c[j] + (float)t * (c2.c[j] - c1.c[j]);
                }

                intersections[numIntersections] = x;
                intersectionsZ[numIntersections] = z;
                intersectionsC[numIntersections] = c;
                numIntersections++;
            }
        }

        // Sort the intersections by x coordinate.
        for (int i = 0; i < numIntersections - 1; i++) {
            for (int j = 0; j < numIntersections - i - 1; j++) {
                if (intersections[j] > intersections[j + 1]) {
                    swap_int(&intersections[j], &intersections[j + 1]);
                    swap_double(&intersectionsZ[j], &intersectionsZ[j + 1]);
                    swap_color(&intersectionsC[j], &intersectionsC[j + 1]);
                }
            }
        }
        // Fill the scanline by drawing lines between pairs of intersection points.
        for (int i = 0; i < numIntersections; i += 2) {
            Point start = {{(float)intersections[i], (float)y, (float)intersectionsZ[i]}};
            Point end = {{(float)intersections[i + 1], (float)y, (float)intersectionsZ[i + 1]}};
            fillScanLine(start, end, intersectionsC[i], intersectionsC[i + 1], src, ds, p->zBuffer);
        }
    }
}



#ifdef __cplusplus
}
#endif
