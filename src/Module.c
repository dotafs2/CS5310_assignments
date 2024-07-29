#include "Module.h"
#define PRINT_MODULE 0
// Create an empty element
Element *element_create() {
    Element *e = (Element *)malloc(sizeof(Element));

    if (e) {
        e->type = ObjNone;
        e->obj.module = NULL;
        e->next = NULL;
    }
    return e;
}

// Initialize an element with a specific type and object
Element *element_init(ObjectType type, void *obj) {
    Element *e = element_create();
    if (e) {
        e->type = type;
        switch (type) {
            case ObjPoint:
                e->obj.point = *(Point *)obj;
                break;
            case ObjLine:
                e->obj.line = *(Line *)obj;
                break;
            case ObjPolyline:
                polyline_init(&(e->obj.polyline));
                polyline_copy(&e->obj.polyline,obj);
                break;
            case ObjPolygon:
                polygon_init(&(e->obj.polygon));
                polygon_copy(&e->obj.polygon,obj);
                break;
            case ObjLight:
                light_init(&(e->obj.light));
                light_copy(&e->obj.light,obj);
                break;
            case ObjModule:
                e->obj.module = obj;
                break;
            case ObjMatrix:
            case ObjIdentity:
                e->obj.matrix = obj;
                break;
            case ObjBezierCurve:
                bezierCurve_init(& (e->obj.bezierCurve));
                e->obj.bezierCurve = *(BezierCurve *)obj;
                break;
            case ObjBezierSurface:
                bezierSurface_init(&(e->obj.bezierSurface));
                e->obj.bezierSurface = *(BezierSurface *)obj;
                break;
            case ObjColor:
            case ObjBodyColor:
            case ObjSurfaceColor:
                color_init(&(e->obj.color));
                color_copy(&e->obj.color,obj);
                break;
            case ObjSurfaceCoeff:
                e->obj.coeff = *(float *)obj;
            break;
            default:
                free(e);
                return NULL;
        }
    }
    return e;
}

void element_delete(Element *e) {
    if (e == NULL) return;

    if(e->type == ObjPolyline)
        polyline_clear(&e->obj.polyline);
    if(e->type == ObjPolygon)
        polygon_clear(&e->obj.polygon);

    free(e);
}


Module *module_create() {
    Module *md = (Module *)malloc(sizeof(Module));
    if (md) {
        md->head = NULL;
        md->tail = NULL;
    }
    return md;
}

// Clear the module's list of elements
void module_clear(Module *md) {
    if (md) {
        Element *current = md->head;
        while (current) {
            Element *next = current->next;
            element_delete(current);
            current = next;
        }
        md->head = NULL;
        md->tail = NULL;
    }
}

// Delete a module and free its memory
void module_delete(Module *md) {
    if (md) {
        module_clear(md);
        free(md);
    }
}

// Insert an element into the module at the tail of the list
void module_insert(Module *md, Element *e) {
    if (md && e) {
        if (md->tail) {
            md->tail->next = e;
        } else {
            md->head = e;
        }
        md->tail = e;
    }
}

// Add a submodule to the module
void module_module(Module *md, Module *sub) {
    if (md && sub) {
        Element *e = element_init(ObjModule, sub);
        module_insert(md, e);
    }
}

void module_addLight( Module *md, Light *light ) {
    if(md == NULL || light == NULL) {
        return;
    }
    Element *e = element_init(ObjLight,md);
    module_insert(md,e);
}

// Add a point to the module
void module_point(Module *md, Point *p) {
    if (md && p) {
        Element *e = element_init(ObjPoint, p);
        module_insert(md, e);
    }
}

// Add a line to the module
void module_line(Module *md, Line *p) {
    if (md && p) {
        Element *e = element_init(ObjLine, p);
        module_insert(md, e);
    }
}

// Add a polyline to the module
void module_polyline(Module *md, Polyline *p) {
    if (md && p) {
        Element *e = element_init(ObjPolyline, p);
        module_insert(md, e);
    }
}

void module_bezierCurve(Module *md, BezierCurve *b, int divisions) {
    if( md && b) {
        Element *e = element_init(ObjBezierCurve, b);
        module_insert(md,e);
    }
}

void module_bezierSurface(Module *md, BezierSurface *b, int divisions, int solid) {
    if( md && b) {
        Element *e = element_init(ObjBezierSurface, b);
        module_insert(md,e);
    }
}

// Add a polygon to the module
void module_polygon(Module *md, Polygon *p) {
    if (md && p) {
        Element *e = element_init(ObjPolygon, p);
        module_insert(md, e);
    }
}

// Set the current transform to the identity
void module_identity(Module *md) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        Element *e = element_init(ObjIdentity,m);
        module_insert(md, e);
    }
}

// Add a translation matrix to the module
void module_translate2D(Module *md, double tx, double ty) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_translate2D(m, tx, ty);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a scale matrix to the module
void module_scale2D(Module *md, double sx, double sy) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_scale2D(m, sx, sy);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix about the Z axis to the module
void module_rotateZ(Module *md, double cth, double sth) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateZ(m, cth, sth);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a 2D shear matrix to the module
void module_shear2D(Module *md, double shx, double shy) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_shear2D(m, shx, shy);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}



// Add a 3D translation matrix to the module
void module_translate(Module *md, double tx, double ty, double tz) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_translate(m, tx, ty, tz);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a 3D scale matrix to the module
void module_scale(Module *md, double sx, double sy, double sz) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_scale(m, sx, sy, sz);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix about the X axis to the module
void module_rotateX(Module *md, double cth, double sth) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateX(m, cth, sth);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix about the Y axis to the module
void module_rotateY(Module *md, double cth, double sth) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateY(m, cth, sth);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix that orients to the orthonormal axes u, v, w to the module
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w) {
    if (md && u && v && w) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateXYZ(m, u, v, w);
        Element *e = element_init(ObjMatrix, m);
        module_insert(md, e);
    }
}
void module_parseLighting(Module *md, Matrix *GTM, Lighting *lighting) {
    if ( (md == NULL) || (GTM == NULL) || (lighting == NULL)) {
        return;
    }
    Element *current = md->head;
    Matrix LTM, tempGTM;
    matrix_identity(&LTM);
    while (current) {
        matrix_identity(&tempGTM);
        matrix_multiply(GTM,&LTM,&tempGTM);
        switch (current->type) {
            case ObjLight: {

                Light transformedLight;
                light_copy(&transformedLight, &current->obj.light);
                matrix_xformPoint(&LTM, &transformedLight.position, &transformedLight.position);
                matrix_xformVector(&LTM, &transformedLight.direction, &transformedLight.direction);
                matrix_xformPoint(GTM, &transformedLight.position, &transformedLight.position);
                matrix_xformVector(GTM, &transformedLight.direction, &transformedLight.direction);
                light_print(&transformedLight);
                lighting_add(lighting, transformedLight.type, &transformedLight.color, &transformedLight.direction, &transformedLight.position, transformedLight.cutoff, transformedLight.sharpness);
                break;
            }
            case ObjMatrix: {
                Matrix tempLTM;
                matrix_identity(&tempLTM);
                matrix_copy(&tempLTM,&LTM);
                matrix_multiply((current->obj.matrix), &tempLTM, &LTM);
            }
            break;
            case ObjIdentity: {
                matrix_identity(&LTM);
                break;
            }
            case ObjModule: {
                module_parseLighting(current->obj.module,&tempGTM, lighting);
            }
            break;
            default:
                    break;
        }
        current = current->next;
    }
}



void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src) {
    if ((md == NULL) || (VTM == NULL) || (GTM == NULL || (ds == NULL) || (src == NULL))) {
        return;
    }
    if(lighting == NULL) {
        lighting = lighting_create();
        if(PRINT_MODULE) {
            printf("lighting of module_draw input of module : ");
            printCharArray(md->name,10);
            printf(" is NULL\n");
        }
    }
    if(ds->color.c == NULL) {
        ds->color.c[0] = 1;
        ds->color.c[1] = 1;
        ds->color.c[2] = 1;
        if(PRINT_MODULE) {
            printf("ds->color of module : ");
            printCharArray(md->name,10);
            printf(" is NULL\n");
        }
    }

    Element *current = md->head;
    Matrix LTM, tempGTM;
    matrix_identity(&LTM);
    while (current) {
        matrix_identity(&tempGTM);
        matrix_multiply(GTM,&LTM,&tempGTM);
        switch (current->type) {
            case ObjNone:

                break;
            case ObjPoint: {
                Point tempPoint = current->obj.point;
                matrix_xformPoint(&tempGTM, &tempPoint, &tempPoint);
                matrix_xformPoint(VTM, &tempPoint, &tempPoint);
                //  printf("Point: %f,%f,%f ", tempPoint.val[0],tempPoint.val[1],tempPoint.val[2]);
                point_draw(&tempPoint, src, ds->color);
            }
                break;

            case ObjLine: {
                Line tempLine = current->obj.line;
                matrix_xformLine(&tempGTM, &tempLine);
                matrix_xformLine(VTM, &tempLine);
                line_normalize(&tempLine);
                line_draw(&tempLine, src, ds->color);
            }
                break;
            case ObjBezierCurve: {
                BezierCurve tempBCurve1 = current->obj.bezierCurve;
                BezierCurve tempBCurve2 = current->obj.bezierCurve;
                BezierCurve tempBCurve3 = current->obj.bezierCurve;
                for (int i = 0; i < 4; ++i) {
                    matrix_xformPoint(&tempGTM, &tempBCurve1.p[i],&tempBCurve2.p[i]);
                    matrix_xformPoint(VTM, &tempBCurve2.p[i],&tempBCurve3.p[i]);
                }
                bezierCurve_draw(&tempBCurve3,src,ds->color, BezierDeCasteljau);
            }
                break;
            case ObjBezierSurface: {
                BezierSurface tempBSurface1 = current->obj.bezierSurface;
                BezierSurface tempBSurface2 = current->obj.bezierSurface;
                BezierSurface tempBSurface3 = current->obj.bezierSurface;
                for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        matrix_xformPoint(&tempGTM, &tempBSurface1.p2[i][j],&tempBSurface2.p2[i][j]);
                        matrix_xformPoint(VTM, &tempBSurface2.p2[i][j],&tempBSurface3.p2[i][j]);
                    }
                }
                bezierSurface_draw(&tempBSurface3,src,ds->color, 4,0);
            }
                break;
            case ObjPolyline: {
                Polyline tempPolyline;
                polyline_init(&tempPolyline);
                polyline_copy(&tempPolyline,&current->obj.polyline);
                matrix_xformPolyline(&tempGTM, &tempPolyline);
                matrix_xformPolyline(VTM, &tempPolyline);
                polyline_normalize(&tempPolyline);
                polyline_draw(&tempPolyline, src, ds->color);
                polyline_clear(&tempPolyline);
            }
                break;
            case ObjPolygon: {
                Polygon tempPolygon = *polygon_create();
                polygon_copy( &tempPolygon, &current->obj.polygon);
                matrix_xformPolygon(&tempGTM, &tempPolygon);
                switch(ds->shade) {
                    case ShadeGouraud:
                        //  Call polygon_shade to calculate the color at each vertex using P
                            polygon_shade(&tempPolygon,lighting,ds);

                    break;
                    default:
                        break;
                }

                matrix_xformPolygon(VTM, &tempPolygon);
                polygon_normalize(&tempPolygon);
                for (int i = 0; i <  tempPolygon.numVertex; ++i) {
                    if(tempPolygon.vertex->val[1] < 0 )
                        printf("%f\n",tempPolygon.vertex->val[1]);
                }
               //  polygon_print(&tempPolygon,stdout);
                switch(ds->shade) {
                    case ShadeConstant:
                        polygon_drawFill(&tempPolygon,src,ds->color);
                        break;
                    case ShadeDepth:
                        polygon_drawShade(&tempPolygon,src,ds,lighting);
                        break;
                    case ShadeGouraud:
                        // Call polygon_drawShade with P, the draw state and the lighting
                        polygon_drawShade(&tempPolygon,src,ds,lighting);
                        break;
                    default:
                         polygon_drawFill(&tempPolygon,src,ds->color);
                            break;
                }
                polygon_clear(&tempPolygon);
            }
                break;
            case ObjModule: {
                DrawState tempDrawState;
                drawstate_copy(&tempDrawState,ds);
                module_draw(current->obj.module, VTM, &tempGTM, &tempDrawState, lighting, src);
            }
                break;
            case ObjMatrix: {
                Matrix tempLTM;
                matrix_identity(&tempLTM);
                matrix_copy(&tempLTM,&LTM);
                matrix_multiply((current->obj.matrix), &tempLTM, &LTM);
            }
                break;
            case ObjIdentity: {
                matrix_identity(&LTM);
            }
                break;
            case ObjColor: {
                ds->color = current->obj.color;
            }
                break;
            case ObjBodyColor:
                drawstate_setBody( ds, current->obj.color );
            break;

            case ObjSurfaceColor:
                drawstate_setSurface( ds, current->obj.color );
            break;

            case ObjSurfaceCoeff:
                drawstate_setSurfaceCoeff( ds, current->obj.coeff );
            break;

            default:
                break;
        }
        current = current->next;
    }
}

// Add a unit cube to the module
void module_cube(Module *md, int solid) {
    if (md) {
        // Define the vertices of the cube
        Point vertices[8] = {
                { -0.5, -0.5, -0.5, 1.0 },
                {  0.5, -0.5, -0.5, 1.0 },
                {  0.5,  0.5, -0.5, 1.0 },
                { -0.5,  0.5, -0.5, 1.0 },
                { -0.5, -0.5,  0.5, 1.0 },
                {  0.5, -0.5,  0.5, 1.0 },
                {  0.5,  0.5,  0.5, 1.0 },
                { -0.5,  0.5,  0.5, 1.0 }
        };

        // Define the edges of the cube
        Line edges[12] = {
                { 1,vertices[0], vertices[1] },
                { 1,vertices[1], vertices[2] },
                { 1,vertices[2], vertices[3] },
                { 1,vertices[3], vertices[0] },
                {1, vertices[4], vertices[5] },
                {1, vertices[5], vertices[6] },
                {1, vertices[6], vertices[7] },
                {1, vertices[7], vertices[4] },
                {1, vertices[0], vertices[4] },
                {1,vertices[1], vertices[5] },
                {1, vertices[2], vertices[6] },
                {1,vertices[3], vertices[7] }
        };
        if (solid) {
            // Define the faces of the cube
            Polygon faces[6];
            // Front face
            Point front[4] = { vertices[0], vertices[1], vertices[2], vertices[3] };
            // Back face
            Point back[4] = { vertices[5], vertices[4], vertices[7], vertices[6] };
            // Left face
            Point left[4] = { vertices[0], vertices[3], vertices[7], vertices[4] };
            // Right face
            Point right[4] = { vertices[1], vertices[5], vertices[6], vertices[2] };
            // Top face
            Point top[4] = { vertices[3], vertices[2], vertices[6], vertices[7] };
            // Bottom face
            Point bottom[4] = { vertices[0], vertices[4], vertices[5], vertices[1] };

Vector normal;
Vector nlist[4];
Color color ={1,1,1};
// Initialize polygons
for (int i = 0; i < 6; ++i) {
    polygon_init(&faces[i]);
}

// Calculate normals and set faces
normal_calculation(&front[0], &front[1], &front[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[0], 4, front);
polygon_setNormals(&faces[0], 4, nlist);
polygon_setSided(&faces[0], 1);
polygon_setColors(&faces[0], 4, &color);
normal_calculation(&back[0], &back[1], &back[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[1], 4, back);
polygon_setNormals(&faces[1], 4, nlist);
polygon_setSided(&faces[1], 1);
polygon_setColors(&faces[1], 4, &color);

normal_calculation(&left[0], &left[1], &left[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[2], 4, left);
polygon_setNormals(&faces[2], 4, nlist);
polygon_setSided(&faces[2], 1);
polygon_setColors(&faces[2], 4, &color);

normal_calculation(&right[0], &right[1], &right[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[3], 4, right);
polygon_setNormals(&faces[3], 4, nlist);
polygon_setSided(&faces[3], 1);
polygon_setColors(&faces[3], 4, &color);

normal_calculation(&top[0], &top[1], &top[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[4], 4, top);
polygon_setNormals(&faces[4], 4, nlist);
polygon_setSided(&faces[4], 1);
polygon_setColors(&faces[4], 4, &color);

normal_calculation(&bottom[0], &bottom[1], &bottom[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[5], 4, bottom);
polygon_setNormals(&faces[5], 4, nlist);
polygon_setSided(&faces[5], 1);
polygon_setColors(&faces[5], 4, &color);

for (int i = 0; i < 6; i++) {
    module_polygon(md, &faces[i]);
   // polygon_print(&faces[i],stdout);
}

        }else {
            // Add lines to the module
            for (int i = 0; i < 12; i++) {
                module_line(md, &edges[i]);
            }
        }
    }
}

void module_plane(Module *md, Point **points) {
    if (md) {
        Color color = {0.0f, 0.5f, 1.0f};

        for (int i = 0; i < WIDTH - 1; ++i) {
            for (int j = 0; j < HEIGHT - 1; ++j) {
                Polygon quad;
                Point quad_points[4] = {
                    points[i][j],
                    points[i + 1][j],
                    points[i + 1][j + 1],
                    points[i][j + 1]
                };

                Vector normal;
                Vector nlist[4];
                polygon_init(&quad);
                polygon_set(&quad, 4, quad_points);
                normal_calculation(&quad_points[0], &quad_points[1], &quad_points[2], &normal);
                for (int k = 0; k < 4; ++k) nlist[k] = normal;
                polygon_setNormals(&quad, 4, nlist);
                polygon_setSided(&quad, 1);
                polygon_setColors(&quad, 4, &color);
                module_polygon(md, &quad);
            }
        }
    }
}



void module_color(Module *md, Color *c) {
    if (md == NULL || c == NULL) {
        return;
    }
    Element *e = element_create();
    if (e == NULL) {
        return;
    }
    e->type = ObjColor;
    e->obj.color = *c;
    module_insert(md, e);
}

void module_bodyColor(Module *md, Color *c) {
    if (md == NULL || c == NULL) {
        return;
    }
    Element *e = element_create();
    if (e == NULL) {
        return;
    }
    e->type = ObjBodyColor;
    e->obj.color = *c;
    module_insert(md, e);
}

void module_surfaceColor(Module *md, Color *c) {
    if (md == NULL || c == NULL) {
        return;
    }
    Element *e = element_create();
    if (e == NULL) {
        return;
    }
    e->type = ObjSurfaceColor;
    e->obj.color = *c;
    module_insert(md, e);
}

void module_surfaceCoeff(Module *md, float coeff) {
    if (md == NULL) {
        return;
    }
    Element *e = element_create();
    if (e == NULL) {
        return;
    }
    e->type = ObjSurfaceCoeff;
    e->obj.coeff = coeff;
    module_insert(md, e);
}



DrawState *drawstate_create(void) {
    DrawState *ds = (DrawState *)malloc(sizeof(DrawState));
    if (ds == NULL) {
        return NULL;
    }
    // Initialize the fields
    ds->color = (Color){1, 1, 1}; // Default to white
    ds->flatColor = (Color){0, 0, 0}; // Default to black
    ds->body = (Color){0, 0, 0}; // Default to black
    ds->surface = (Color){0, 0, 0}; // Default to black
    ds->surfaceCoeff = 0.0f; // Default coefficient
    ds->shade = ShadeFrame; // Default shading method
    ds->zBufferFlag = 0; // Default z-buffer flag
    ds->viewer = (Point){0, 0, 0}; // Default viewer position
    return ds;
}

// Set the color field to c
void drawstate_setColor(DrawState *ds, Color c) {
    if (ds == NULL) {
        return;
    }
    ds->color = c;
}

// Set the body field to c
void drawstate_setBody(DrawState *ds, Color c) {
    if (ds == NULL) {
        return;
    }
    ds->body = c;
}

// Set the surface field to c
void drawstate_setSurface(DrawState *ds, Color c) {
    if (ds == NULL) {
        return;
    }
    ds->surface = c;
}

// Set the surfaceCoeff field to f
void drawstate_setSurfaceCoeff(DrawState *ds, float f) {
    if (ds == NULL) {
        return;
    }
    ds->surfaceCoeff = f;
}

// Copy the DrawState data from one structure to another
void drawstate_copy(DrawState *to, DrawState *from) {
    if (to == NULL || from == NULL) {
        return;
    }
    to->color = from->color;
    to->flatColor = from->flatColor;
    to->body = from->body;
    to->surface = from->surface;
    to->surfaceCoeff = from->surfaceCoeff;
    to->shade = from->shade;
    to->zBufferFlag = from->zBufferFlag;
    to->viewer = from->viewer;
}

