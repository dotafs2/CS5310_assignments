#include "Polygon.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function to create an empty Polygon
Polygon *polygon_create() {
    Polygon *p = (Polygon *)malloc(sizeof(Polygon));
    if (p == NULL) return NULL;
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
    if (p == NULL) return NULL;
    polygon_set(p, numV, vlist);
    return p;
}

// Function to free the memory of a Polygon
void polygon_free(Polygon *p) {
    if (p == NULL) return;
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
    if (p == NULL) return;
    p->numVertex = 0;
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->zBuffer = 1;
    p->oneSided = 0;
}



// Function to set the vertices of a Polygon
void polygon_set(Polygon *p, int numV, Point *vlist) {
    if (p == NULL) return;
    if (p->vertex) free(p->vertex);
    p->vertex = (Point *)malloc(numV * sizeof(Point));
    if (p->vertex == NULL) {
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
    if (p->vertex != NULL)  free(p->vertex);
    if (p->color != NULL)   free(p->color);
    if (p->normal != NULL)  free(p->normal);
    p->numVertex = 0;
}

// Function to set the oneSided field of a Polygon
void polygon_setSided(Polygon *p, int oneSided) {
    if (p == NULL) return;
    p->oneSided = oneSided;
}

// Function to set the colors of a Polygon
void polygon_setColors(Polygon *p, int numV, Color *clist) {
    if (p == NULL) return;
    if (p->color) free(p->color);
    p->color = (Color *)malloc(numV * sizeof(Color));
    if (p->color == NULL) return;
    for (int i = 0; i < numV; i++) {
        p->color[i] = clist[i];
    }
}

// Function to set the normals of a Polygon
void polygon_setNormals(Polygon *p, int numV, Vector *nlist) {
    if (p == NULL) return;
    if (p->normal) free(p->normal);
    p->normal = (Vector *)malloc(numV * sizeof(Vector));
    if (p->normal == NULL) return;
    for (int i = 0; i < numV; i++) {
        p->normal[i] = nlist[i];
    }
}

// Function to set all properties of a Polygon
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided) {
    if (p == NULL) return;
    polygon_set(p, numV, vlist);
    polygon_setColors(p, numV, clist);
    polygon_setNormals(p, numV, nlist);
    p->zBuffer = zBuffer;
    p->oneSided = oneSided;
}

// Function to set the z-buffer flag of a Polygon
void polygon_zBuffer(Polygon *p, int flag) {
    if (p == NULL) return;
    p->zBuffer = flag;
}

// Function to copy one Polygon to another
void polygon_copy(Polygon *to, Polygon *from) {
    if ((to == NULL ) || (from == NULL)) return;
    // Allocate new memory and copy the data
    to->vertex = (Point *)malloc(from->numVertex * sizeof(Point));
    if (to->vertex == NULL) return;
    memcpy(to->vertex, from->vertex, from->numVertex * sizeof(Point));

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
    if ( (p == NULL) || (fp == NULL)) return;
    fprintf(fp, "Polygon with %d vertices:\n", p->numVertex);
    for (int i = 0; i < p->numVertex; i++) {
        fprintf(fp, "  Vertex %d: (%f, %f, %f, %f)\n", i, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
    }
}

// Function to normalize the vertices of a Polygon
void polygon_normalize(Polygon *p) {
    if (p == NULL) return;
    for (int i = 0; i < p->numVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}

    // Create an empty EdgeRec
EdgeRec *EdgeRec_create() {
EdgeRec *e = (EdgeRec *)malloc(sizeof(EdgeRec));
    e->xIntersect = 0.0;
    e->zIntersect = 0.0;
    e->dxPerScanline = 0.0;
    e->dzPerScanline = 0.0;
    e->yUpper = 0;
    e->next = NULL;

    return e;
}
void makeEdgeRec(Point v1, Point v2, EdgeRec *rec) {
    if (v1.val[1] > v2.val[1])
        swap_points(&v1,&v2);
    rec->xIntersect = v1.val[0];
    rec->zIntersect = 1.0f / v1.val[2];
    rec->yUpper = (int)v2.val[1];
    rec->dxPerScanline = (v2.val[0] - v1.val[0]) / (v2.val[1] - v1.val[1]);
    rec->dzPerScanline = (1.0f / v2.val[2] - 1.0f / v1.val[2]) / (v2.val[1] - v1.val[1]);
    rec->next = NULL;
}

void processEdgeList(EdgeRec *edgeTable[], int y, EdgeRec **activeList) {
    // 更新当前活动边的交点
    EdgeRec *current = *activeList;
    while (current) {
        current->xIntersect += current->dxPerScanline;
        current->zIntersect += current->dzPerScanline;
        current = current->next;
    }

    // 删除已经处理完的边
    EdgeRec **p = activeList;
    while (*p) {
        EdgeRec *q = *p;
        if (q->yUpper == y) {
            *p = q->next;
            free(q);
        } else {
            p = &(*p)->next;
        }
    }

    // 将新的边加入活动边列表
    current = edgeTable[y];
    while (current) {
        EdgeRec *temp = current;
        current = current->next;
        temp->next = *activeList;
        *activeList = temp;
    }
}

void fillScan(int y, EdgeRec *activeList, Image *src, Color c) {
    EdgeRec *p1, *p2;
    int i;

    for (p1 = activeList; p1 && p1->next; p1 = p1->next->next) {
        p2 = p1->next;
        if (p1->xIntersect > p2->xIntersect) {
            EdgeRec* temp = p1;
            p1 = p2;
            p2 = temp;
        }

        float dzPerColumn = (p2->zIntersect - p1->zIntersect) / (p2->xIntersect - p1->xIntersect);
        float curZ = p1->zIntersect;

        for (i = ceil(p1->xIntersect); i <= floor(p2->xIntersect); i++) {
            if (i >= 0 && i < src->cols && y >= 0 && y < src->rows) {
                int index = y * src->cols + i;
                if (curZ > src->data[index].z) {
                    src->data[index].z = curZ;
                    FPixel pixel = { {c.c[0], c.c[1], c.c[2]}, curZ, curZ };
                    src->data[index] = pixel;
                }
            }
            curZ += dzPerColumn;
        }
    }
}


    void polygon_drawFill(Polygon *p, Image *src, Color c) {
    if (p == NULL || src == NULL || p->numVertex < 3) return;

    int minY = src->rows, maxY = 0;
    for (int i = 0; i < p->numVertex; i++) {
        int y = (int)p->vertex[i].val[1];
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    if (minY < 0) minY = 0;
    if (maxY >= src->rows) maxY = src->rows - 1;

    // Allocate memory for edge table
    EdgeRec **edgeTable = (EdgeRec **)calloc(maxY + 1, sizeof(EdgeRec *));
    if (edgeTable == NULL) return; // Check for calloc failure

    for (int i = 0; i < p->numVertex; i++) {
        Point v1 = p->vertex[i];
        Point v2 = p->vertex[(i + 1) % p->numVertex];
        if ((int)v1.val[1] == (int)v2.val[1]) continue; // Skip horizontal edges
        EdgeRec *rec = (EdgeRec *)malloc(sizeof(EdgeRec));
        if (rec == NULL) continue; // Check for malloc failure
        makeEdgeRec(v1, v2, rec);
        int yStart = (int)ceil(v1.val[1]);
        rec->next = edgeTable[yStart];
        edgeTable[yStart] = rec;
    }

    EdgeRec *activeList = NULL;

    for (int y = minY; y <= maxY; y++) {
        processEdgeList(edgeTable, y, &activeList);
        fillScan(y, activeList, src, c);
    }

    // Free edge table
    for (int y = minY; y <= maxY; y++) {
        EdgeRec *current = edgeTable[y];
        while (current) {
            EdgeRec *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(edgeTable);
}



void polygon_draw(Polygon *p, Image *src, Color c) {
    if (!(int)p || !(int)src || p->numVertex < 2) return;

    Line line;
    for (int i = 0; i < p->numVertex; i++) {
        int next = (i + 1) % p->numVertex;
        line_set(&line, p->vertex[i], p->vertex[next]);
        line_draw(&line, src, c);
    }
}

// Function to fill a Polygon using the Barycentric coordinates algorithm
// TODO: did not fit z-buffer right now
void polygon_drawFillB(Polygon *p, Image *src, Color c) {
    if ( (p == NULL) || (src == NULL) || p->numVertex < 3) return;

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
    double denom = (vlist[1].val[1] - vlist[2].val[1]) * (vlist[0].val[0] - vlist[2].val[0]) +
                  (vlist[2].val[0] - vlist[1].val[0]) * (vlist[0].val[1] - vlist[2].val[1]);

    *alpha = ((vlist[1].val[1] - vlist[2].val[1]) * (px - vlist[2].val[0]) +
              (vlist[2].val[0] - vlist[1].val[0]) * (py - vlist[2].val[1])) / denom;
    *beta = ((vlist[2].val[1] - vlist[0].val[1]) * (px - vlist[2].val[0]) +
             (vlist[0].val[0] - vlist[2].val[0]) * (py - vlist[2].val[1])) / denom;
    *gamma = 1.0f - *alpha - *beta;

    return (*alpha >= 0.0f && *beta >= 0.0f && *gamma >= 0.0f);
}

    void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light) {
        if (p == NULL || src == NULL || ds == NULL || p->numVertex < 3) return;
        switch (ds->shade) {
            case ShadeFrame:
                // Draw only the outline of the polygon using the DrawState color field
                    polygon_draw(p, src, ds->color);
            break;

            case ShadeConstant:
                // Fill the polygon with the DrawState color field
                    polygon_drawFill(p, src, ds->color);
            break;

    case ShadeDepth:
        for (int i = 0; i < p->numVertex; i++) {
            float depth = p->vertex[i].val[2];
            float shade = 1.0f - depth;
            Color depthColor = {0.5, 0.5, 0.5};
            polygon_drawFill(p, src, ds->color);
        }
      break;
        }
    }


#ifdef __cplusplus
}
#endif