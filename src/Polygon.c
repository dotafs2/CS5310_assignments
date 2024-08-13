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

void polygon_setTexture(Polygon *p, TextureCoord *stlist) {
      if(p->texture == NULL) {
          p->texture = malloc(sizeof(TextureCoord) * p->numVertex);
      }
        memcpy(p->texture, stlist, sizeof(TextureCoord) * p->numVertex);
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

    void polygon_setColors(Polygon *p, int numV, Color *clist) {
        if (numV == p->numVertex) {
            if (p->color == NULL) {
                p->color = malloc(sizeof(Color) * numV);
            } else {
                free(p->color);
                p->color = malloc(sizeof(Color) * numV);
            }
            if (p->color != NULL && clist != NULL) {
                memcpy(p->color, clist, sizeof(Color) * numV);
            }
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


void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided, TextureCoord *tlist){
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

            if(tlist != NULL) {
                p->texture = malloc(sizeof(TextureCoord) * numV );
                memcpy( p->texture, tlist, sizeof(TextureCoord) * numV );
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
    polygon_setAll( to, from->numVertex, from->vertex, from->color, from->normal, from->zBuffer, from->oneSided, from->texture);
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
        case ShadeFlat:
        case ShadeGouraud:
            for(int i=0; i<p->numVertex; i++) {
                Vector V;
                V.val[0] = ds->viewer.val[0] - p->vertex[i].val[0];
                V.val[1] = ds->viewer.val[1] - p->vertex[i].val[1];
                V.val[2] = ds->viewer.val[2] - p->vertex[i].val[2];
                lighting_shading( l, &(p->normal[i]), &V, &(p->vertex[i]), &(ds->body), &(ds->surface), ds->surfaceCoeff, p->oneSided, &(p->color[i]));
            }
        break;
        case ShadePhong:
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

void fillScanLine(Point p0, Point p1, Color c0, Color c1,TextureCoord t0, TextureCoord t1, Vector n0, Vector n1, Image *src, DrawState *ds, int zbuffer, Lighting *light) {
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

        // Swap colors
        Color tempColor = c0;
        c0 = c1;
        c1 = tempColor;

        // Swap texture coordinates
        TextureCoord tempTex = t0;
        t0 = t1;
        t1 = tempTex;

        // Swap normal
        Vector tempNormal = n0;
        n0 = n1;
        n1 = tempNormal;

    }


        if (x1 < 0 || x0 >= src->cols) {
            return; // Skip this line
        }

        // if (x0 < 0) x0 = 0;
        // if (x0 >= src->cols) x0 = src->cols - 1;
        // if (x1 < 0) x1 = 0;
        // if (x1 >= src->cols) x1 = src->cols - 1;

    // Calculate the change in depth per pixel using 1/z for perspective projection
    float invZ0 = 1.0f / z0;
    float invZ1 = 1.0f / z1;
    float deltaInvZ = (invZ1 - invZ0) / (float)(x1 - x0);
    Color deltaColor;
    for (int i = 0; i < 3; i++) {
        deltaColor.c[i] = (c1.c[i] - c0.c[i]) / (float)(x1 - x0);
    }

        TextureCoord deltaTex;
        deltaTex.r = (t1.r - t0.r) / (float)(x1 - x0);
        deltaTex.c = (t1.c - t0.c) / (float)(x1 - x0);

        Vector deltaNormal = {0,0,0,0};
        for (int i = 0; i < 3; i++) {
            deltaNormal.val[i] = (n1.val[i] - n0.val[i]) / (float)(x1 - x0);
        }


        // Adjust x0 and related values if x0 is out of bounds
        if (x0 < 2) {
            invZ0 += -(float)x0 * deltaInvZ;
            for (int i = 0; i < 3; i++) {
                c0.c[i] += deltaColor.c[i] + (float)(-x0);
            }
            t0.r += deltaTex.r * (float)(-x0);
            t0.c += deltaTex.c * (float)(-x0);

            for (int i = 0; i < 3; i++) {
                n0.val[0] += deltaNormal.val[0] *(float)(-x0);
            }
             x0 = 2;
        }

        // Adjust x1 if it's out of bounds
        if (x1 >= src->cols) {
            x1 = src->cols - 1;
        }

    // Start from invZ0 and c1
    float invZ = invZ0;
    Color currentColor = c0;
    TextureCoord currentTex = t0;
    Vector currentNormal = n0;
    for (int x = x0; x <= x1; x++) {
        int targetIndex = y0 * src->cols + x;
        FPixel* targetPixel = &src->data[targetIndex];
        Color tempColor = {0.0f, 0.0f, 0.0f};
        // if do not add z-buffer test
        if(!zbuffer) {
            targetPixel->rgb[0] = currentColor.c[0];
            targetPixel->rgb[1] = currentColor.c[1];
            targetPixel->rgb[2] = currentColor.c[2];
            if (t1.textureMapIntex != -1 ) {
                int texRows = (int)currentTex.r;
                int texCols = (int)currentTex.c;
                targetPixel->rgb[0] *= ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[0];
                targetPixel->rgb[1] *= ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[1];
                targetPixel->rgb[2] *= ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[2];
            }
            //printf("color%f,%f,%f\n",targetPixel->rgb[0],targetPixel->rgb[1],targetPixel->rgb[2]);
        }else {
            // Z-buffer test
            if (invZ > src->depth[targetIndex]) {

                Vector V;
                float alphaValue = 1.0f; // Default to fully opaque
                // Set the color based on the shading mode
                switch(ds->shade) {
                    case ShadeDepth:
                        tempColor.c[0] = ds->color.c[0] * (1.0f - 1/invZ);
                        tempColor.c[1] = ds->color.c[1] * (1.0f - 1/invZ);
                        tempColor.c[2]= ds->color.c[2] * (1.0f - 1/invZ);
                    break;
                    case ShadeGouraud:
                        tempColor.c[0]= currentColor.c[0];
                       tempColor.c[1]= currentColor.c[1];
                       tempColor.c[2] = currentColor.c[2];
                    break;
                    case ShadePhong:
                        V.val[0] = ds->viewer.val[0] - x;
                        V.val[1] = ds->viewer.val[1] - y0;
                        V.val[2] = ds->viewer.val[2] - 1/invZ;
                        Point currentPosition = {x,y0,1/invZ,1};
                        Color resultColor;
                        // default
                        Color ka = {0.1f,0.1f,0.1f};
                        Color kd = {0.6f,0.6f,0.6f};
                        Color ks = {0.8f,0.8f,0.8f};
                        int R = 1;
                        // Ka 材质的环境光反射系数, kd 漫反射光照系数, ks 镜面反射光照系数, R 为高光强度, alphaValue alpha通道
                        if (t1.textureMapIntex != -1 && (int)currentTex.r != -1 && (int)currentTex.c != -1) {
                            ka = ds->textureMaps[t1.textureMapIntex].ambientCoefficients;
                            kd = ds->textureMaps[t1.textureMapIntex].diffuseCoefficients;
                            ks = ds->textureMaps[t1.textureMapIntex].specularCoefficients;
                            R = ds->textureMaps[t1.textureMapIntex].R;
                            alphaValue = ds->textureMaps[t1.textureMapIntex].alpha[(int)(currentTex.c) * ds->textureMaps[t1.textureMapIntex].cols + (int)(currentTex.r)];
                        }
                        lighting_shading_phong(light, &currentNormal, &V, &currentPosition, &(ds->surface), &(ds->surface), ds->surfaceCoeff, 1, &resultColor, ka,kd,ks, R);
                        // 先不复制因为要进行alpha混合
                        tempColor.c[0] = resultColor.c[0];
                        tempColor.c[1] = resultColor.c[1];
                        tempColor.c[2] = resultColor.c[2];
                    break;
                    default:
                         tempColor.c[0] = currentColor.c[0];
                         tempColor.c[1] = currentColor.c[1];
                         tempColor.c[2] = currentColor.c[2];
                    break;
                }


                if (t1.textureMapIntex != -1 && (int)currentTex.r != -1 && (int)currentTex.c != -1) {
                    int texRows = (int)currentTex.r;
                    int texCols = (int)currentTex.c;
                    if(ds->textureMaps[t1.textureMapIntex].alphaFlag) {
                        // 使用alphatest并且当前更加向前的话则使用混合，并且不要更新zbuffer。
                        targetPixel->rgb[0] = tempColor.c[0] * alphaValue *ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[0] + (1 - alphaValue)* targetPixel->rgb[0];
                        targetPixel->rgb[1] = tempColor.c[1] * alphaValue *ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[1] + (1 - alphaValue)* targetPixel->rgb[1];
                        targetPixel->rgb[2] = tempColor.c[2] * alphaValue *ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[2] + (1 - alphaValue)* targetPixel->rgb[2];
                    }else {
                        // 如果没用alphaflag没有被使用，直接覆盖当前所有像素。
                        targetPixel->rgb[0] = tempColor.c[0] * ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[0];
                        targetPixel->rgb[1] = tempColor.c[1] * ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[1];
                        targetPixel->rgb[2] = tempColor.c[0] * ds->textureMaps[t1.textureMapIntex].data[texCols * ds->textureMaps[t1.textureMapIntex].cols + texRows].c[2];
                    }
                }
                else { // 没有使用贴图的话
                    targetPixel->rgb[0] = tempColor.c[0];
                    targetPixel->rgb[1] = tempColor.c[1];
                    targetPixel->rgb[2] = tempColor.c[0];
                }

                // Update the z-buffer value, 但如果用alpha混合就不覆盖了, 并且alphavalue是1的话也不需要覆盖
                if((!ds->textureMaps[t1.textureMapIntex].alphaFlag) || (alphaValue == 1))
                    src->depth[targetIndex] = invZ;

            }
        }

        // Update the current z value
        invZ += deltaInvZ;
    // Update the current color
         for (int i = 0; i < 3; i++) {
        currentColor.c[i] += deltaColor.c[i];
        // Update the current texture coordinate
        currentNormal.val[i] += deltaNormal.val[i];
    }
        currentTex.r += deltaTex.r;
        currentTex.c += deltaTex.c;
    }
}

void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light) {
        // speed up when not using shadephong
    // Find the minimum and maximum y values in the polygon to determine the range of y.
    // need to ensure that minY should > 0 ,and maxY should lower than rows.
    int minY = src->rows, maxY = 0;
    Color vertexColors[p->numVertex];
    TextureCoord vertexTextures[p->numVertex];
    vertexTextures[p->numVertex].textureMapIntex = 0; // default texturemap
    Vector vertexNormals[p->numVertex];
    for (int i = 0; i < p->numVertex; i++) {
        // calculate vertex color
        vertexColors[i] = p->color ? p->color[i] : ds->color;
        vertexTextures[i] = p->texture ? p->texture[i] : (TextureCoord){-1, -1};
        vertexNormals[i] = p->normal ? p->normal[i] : (Vector){0,1,0,0};
        // y out of range, which means the current point is not in the view plane
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }

        if (minY < 0) minY = 0;
        if (maxY > src->rows - 1) maxY = src->rows - 1;


    // Loop through each y from minY to maxY to process scanlines.
    for (int y = minY; y <= maxY; y++) {
        int intersections[10]; // Array to store intersection points for the current scanline.
        double intersectionsZ[10];
        Color intersectionsC[10];
        Vector intersectionsN[10];
        TextureCoord intersectionsT[10];
        int numIntersections = 0; // Counter for the number of intersections.

        // Find intersections of the scanline y with all polygon edges.
        for (int i = 0; i < p->numVertex; i++) {
            // if(p->vertex[i].val[1] < 0|| p->vertex[(i + 1) % p->numVertex].val[1] < 0 || p->vertex[i].val[1] > src->cols|| p->vertex[(i + 1) % p->numVertex].val[1] > src->cols )
            //     continue;

            Point v1 = p->vertex[i];
            Point v2 = p->vertex[(i + 1) % p->numVertex];
        //   printf("v1: %f,%f,%f\n",v1.val[0],v1.val[1],v1.val[2]);
        //    printf("v2: %f,%f,%f\n",v1.val[0],v1.val[1],v1.val[2]);
            Color c1 = vertexColors[i];
            Color c2 = vertexColors[(i + 1) % p->numVertex];
            TextureCoord t1 = vertexTextures[i];
            TextureCoord t2 = vertexTextures[(i + 1) % p->numVertex];
            Vector n1 = vertexNormals[i];
            Vector n2 = vertexNormals[(i + 1) % p->numVertex];

            if ((v1.val[1] <= y && v2.val[1] > y) || (v1.val[1] > y && v2.val[1] <= y)) {
                double t = (y - v1.val[1]) / (v2.val[1] - v1.val[1]);
                int x = (int)(v1.val[0] + t * (v2.val[0] - v1.val[0]));
                double z = v1.val[2] + t * (v2.val[2] - v1.val[2]);

                Color c;
                TextureCoord tex;
                Vector n = {0,0,0,0};

                for (int j = 0; j < 3; j++) {
                    c.c[j] = c1.c[j] + (float)t * (c2.c[j] - c1.c[j]);
                }

                tex.c = t1.c + (float)t * (t2.c - t1.c);
                tex.r = t1.r + (float)t * (t2.r - t1.r);
                tex.textureMapIntex =  t1.textureMapIntex; // one of the point's textureMap


                for (int j = 0; j < 3; j++) {
                    n.val[j] = n1.val[j] + t * (n2.val[j] - n1.val[j]);
                }
                vector_normalize(&n);


                intersections[numIntersections] = x;
                intersectionsZ[numIntersections] = z;
                intersectionsC[numIntersections] = c;
                intersectionsT[numIntersections] = tex;
                intersectionsN[numIntersections] = n;
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
                    swap_texture(&intersectionsT[j], &intersectionsT[j + 1]);
                    swap_vector(&intersectionsN[j],&intersectionsN[j + 1]);
                }
            }
        }
        // Fill the scanline by drawing lines between pairs of intersection points.
        for (int i = 0; i < numIntersections - 1; i += 2) {
            Point start, end;
            start.val[0] = intersections[i];
            start.val[1] = y;
            start.val[2] = intersectionsZ[i];
            end.val[0] = intersections[i+1];
            end.val[1] = y;
            end.val[2] = intersectionsZ[i+1];
            if(start.val[1] < 0) continue;
            if(start.val[1] > src->rows - 1) continue;
            // if(start.val[0] < 0 || end.val[0] < 0) continue;
            // if(end.val[0] > src->cols || end.val[0] > src->cols) continue;
            // if(start.val[2] < 0 || end.val[2] < 0) continue;
            // if(start.val[2] >= 1 || end.val[2] >= 1) continue;

            fillScanLine(start, end, intersectionsC[i], intersectionsC[i + 1], intersectionsT[i],intersectionsT[i + 1], intersectionsN[i],intersectionsN[i + 1], src, ds, p->zBuffer, light);
        }
    }
}



#ifdef __cplusplus
}
#endif
