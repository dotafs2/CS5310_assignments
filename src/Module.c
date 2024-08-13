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
            case ObjFog:
                e->obj.fog = *(Fog *)obj;
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
                lighting_init(&(e->obj.lighting));
                lighting_copy(&e->obj.lighting,obj);
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

// Add a point to the module
void module_point(Module *md, Point *p) {
    if (md && p) {
        Element *e = element_init(ObjPoint, p);
        module_insert(md, e);
    }
}


// Add a point to the module
void module_fog(Module *md, Fog *f) {
    if (md && f) {
        Element *e = element_init(ObjFog, f);
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


void module_addLight( Module *md, Lighting *lighting ) {
    if(md == NULL || lighting == NULL) {
        return;
    }
    Element *e = element_init(ObjLight,lighting);
    module_insert(md,e);
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
void module_parseLighting(Module *md, Matrix *GTM, Matrix *VTM, Lighting *lighting) {
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
                Lighting transformedLighting;
                lighting_copy(&transformedLighting, &current->obj.lighting);
                for (int i = 0; i < transformedLighting.nLights; ++i) {
                    Light transformedLight = transformedLighting.light[i];
                    matrix_xformPoint(&tempGTM, &transformedLight.position, &transformedLight.position);
                    matrix_xformVector(&tempGTM, &transformedLight.direction, &transformedLight.direction);
                    matrix_xformPoint(VTM, &transformedLight.position, &transformedLight.position);
                    matrix_xformVector(VTM, &transformedLight.direction, &transformedLight.direction);
                    point_normalize(&transformedLight.position);
                    vector_normalize(&transformedLight.direction);
                 //   light_print(&transformedLight);
                    lighting_add(lighting, transformedLight.type, &transformedLight.color, &transformedLight.direction, &transformedLight.position, transformedLight.cutoff, transformedLight.sharpness);
                }
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
                module_parseLighting(current->obj.module,&tempGTM, VTM, lighting);
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

    Matrix LightGTM;
    matrix_identity(&LightGTM);
    matrix_copy(&LightGTM,GTM);

     module_parseLighting(md, &LightGTM, VTM,lighting);


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
                point_normalize(&tempPoint);
                //  printf("Point: %f,%f,%f ", tempPoint.val[0],tempPoint.val[1],tempPoint.val[2]);
                point_draw(&tempPoint, src, ds->color);
            }
                break;

            case ObjFog: {
                Fog tempFog = current->obj.fog;
                matrix_xformPoint(&tempGTM, &tempFog.position, &tempFog.position);
                matrix_xformPoint(VTM, &tempFog.position, &tempFog.position);
                point_normalize(&tempFog.position);
                //  printf("Point: %f,%f,%f ", tempPoint.val[0],tempPoint.val[1],tempPoint.val[2]);
                fog_draw(&tempFog, src, tempFog.c);
                break;
            }
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
               //  polygon_print(&tempPolygon,stdout);
                switch(ds->shade) {
                    case ShadeConstant:
                        polygon_drawFill(&tempPolygon,src,ds->color);
                        break;
                    case ShadePhong:
                    case ShadeDepth:
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

void set_skybox(Image *src, Matrix *VTM, View3D view) {
        // Define the vertices of the cube
    Color colors1[4] = {
        {1.0, 0.0, 0.0},  // 红色
        {1.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {1.0, 0.0, 0.0}
    };

    Color colors2[4] = {
        {0.0, 1.0, 0.0},  // 绿色
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 0.0}
    };

    Color colors3[4] = {
        {0.0, 0.0, 1.0},  // 蓝色
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0}
    };

    Color colors4[4] = {
        {1.0, 1.0, 0.0},  // 黄色
        {1.0, 1.0, 0.0},
        {1.0, 1.0, 0.0},
        {1.0, 1.0, 0.0}
    };

    Color colors5[4] = {
        {1.0, 0.0, 1.0},  // 品红色
        {1.0, 0.0, 1.0},
        {1.0, 0.0, 1.0},
        {1.0, 0.0, 1.0}
    };

    Color colors6[4] = {
        {0.0, 1.0, 1.0},  // 青色
        {0.0, 1.0, 1.0},
        {0.0, 1.0, 1.0},
        {0.0, 1.0, 1.0}
    };

    Point vertices[8] = {
        { -500, -500, -500, 1.0 },
        {  500, -500, -500, 1.0 },
        {  500,  500, -500, 1.0 },
        { -500,  500, -500, 1.0 },
        { -500, -500,  500, 1.0 },
        {  500, -500,  500, 1.0 },
        {  500,  500,  500, 1.0 },
        { -500,  500,  500, 1.0 }
    };
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
        // Initialize polygons
        for (int i = 0; i < 6; ++i) {
            polygon_init(&faces[i]);
            polygon_zBuffer(&faces[i],1);
        }
    // Calculate normals and set faces
    normal_calculation(&front[0], &front[1], &front[2], &normal);
    for (int i = 0; i < 4; ++i) nlist[i] = normal;
    polygon_set(&faces[0], 4, front);
    polygon_setNormals(&faces[0], 4, nlist);
    polygon_setSided(&faces[0], 1);
    polygon_setColors(&faces[0], 4, colors1);
    normal_calculation(&back[0], &back[1], &back[2], &normal);
    for (int i = 0; i < 4; ++i) nlist[i] = normal;
    polygon_set(&faces[1], 4, back);
    polygon_setNormals(&faces[1], 4, nlist);
    polygon_setSided(&faces[1], 1);
    polygon_setColors(&faces[1], 4, colors2);

    normal_calculation(&left[0], &left[1], &left[2], &normal);
    for (int i = 0; i < 4; ++i) nlist[i] = normal;
    polygon_set(&faces[2], 4, left);
    polygon_setNormals(&faces[2], 4, nlist);
    polygon_setSided(&faces[2], 1);
    polygon_setColors(&faces[2], 4, colors3);

    normal_calculation(&right[0], &right[1], &right[2], &normal);
    for (int i = 0; i < 4; ++i) nlist[i] = normal;
    polygon_set(&faces[3], 4, right);
    polygon_setNormals(&faces[3], 4, nlist);
    polygon_setSided(&faces[3], 1);
    polygon_setColors(&faces[3], 4, colors4);

    normal_calculation(&top[0], &top[1], &top[2], &normal);
    for (int i = 0; i < 4; ++i) nlist[i] = normal;
    polygon_set(&faces[4], 4, top);
    polygon_setNormals(&faces[4], 4, nlist);
    polygon_setSided(&faces[4], 1);
    polygon_setColors(&faces[4], 4, colors5);

    normal_calculation(&bottom[0], &bottom[1], &bottom[2], &normal);
    for (int i = 0; i < 4; ++i) nlist[i] = normal;
    polygon_set(&faces[5], 4, bottom);
    polygon_setNormals(&faces[5], 4, nlist);
    polygon_setSided(&faces[5], 1);
    polygon_setColors(&faces[5], 4, colors6);



    Matrix vtm, scale;
    Point skyBoxVertices[4];
    for (int i = 0; i < 6; ++i) {
        if(i == 0)
            continue;
        if(i == 3)
            continue;
        if(i == 4)
            continue;
        // Step 1: Initialize VTM to the identity matrix
        matrix_identity(&vtm);
        matrix_identity(&scale);
        matrix_scale(&scale,0.0006,0.0006,0.0006);
        matrix_multiply(&scale, &vtm, &vtm);
        // Step 2: Translate VRP to the origin
        Matrix translate;
        matrix_identity(&translate);
        // matrix_translate(&translate, view.vrp.val[0], view.vrp.val[1], view.vrp.val[2]);
        //      matrix_translate(&translate,0.35,0.4,-0.5);
        matrix_multiply(&translate, &vtm, &vtm);
        // Step 3: VPN + VUP to get the rotate matrix
        Vector u, v, w;
        vector_copy(&w, &view.vpn);
        vector_normalize(&w);
        vector_cross(&view.vup, &w, &u);
        vector_normalize(&u);
        vector_cross(&w, &u, &v);
        vector_normalize(&v);
        // step 4 : form the rotate matrix
        Matrix rotate;
        matrix_identity(&rotate);
        rotate.m[0][0] = u.val[0]; rotate.m[0][1] = u.val[1]; rotate.m[0][2] = u.val[2];
        rotate.m[1][0] = v.val[0]; rotate.m[1][1] = v.val[1]; rotate.m[1][2] = v.val[2];
        rotate.m[2][0] = w.val[0]; rotate.m[2][1] = w.val[1]; rotate.m[2][2] = w.val[2];
        matrix_multiply(&rotate, &vtm, &vtm);

        // step 4.5 : translate to COP
        Matrix translateCOP;
        matrix_identity(&translateCOP);
        matrix_translate(&translateCOP,0,0,view.d);
        matrix_multiply(&translateCOP,&vtm,&vtm);

        // Step 5: Scale to CVV (I thought what is CVV mean for several hours, finally find out its just NDC...)
        double depth = view.d + view.b;
        matrix_scale(&vtm,2.0*view.d/(view.du * depth),2.0*view.d/(view.dv * depth),1.0 / depth);

        // Step 6: Apply perspective projection
        matrix_perspective(&vtm, view.d/depth);

        // step 7 Scale to image coords
        matrix_scale(&vtm, -view.screenx*0.5/(view.d / depth), -view.screeny*0.5/(view.d / depth), 1.0);

        // step 8 translate into place
        matrix_translate2D(&vtm, view.screenx*0.5, view.screeny*0.5);

        matrix_xformPolygon(&vtm,&faces[i]);
        polygon_normalize(&faces[i]);

        DrawState d;
        TextureCoord tex[4];


        faces[i].zBuffer = 0;
        TextureMap* skyMap;
        if(i == 1) {

            tex[0].c = 600;
            tex[0].r = 600;

            tex[1].c = 600;
            tex[1].r = 0;

            tex[2].c = 0;
            tex[2].r = 0;

            tex[3].c = 0;
            tex[3].r = 600;

            skyMap = texture_read("skyboxZ.ppm");
        }
        else if(i == 2) {
            tex[0].c = 600;
            tex[0].r = 0;

            tex[1].c = 0;
            tex[1].r = 0;

            tex[2].c = 0;
            tex[2].r = 600;

            tex[3].c = 600;
            tex[3].r = 600;
            skyMap = texture_read("skyboxX.ppm");
        }
        else {
            tex[0].c = 0;
            tex[0].r = 600;

            tex[1].c = 600;
            tex[1].r = 600;


            tex[2].c = 600;
            tex[2].r = 0;

            tex[3].c = 0;
            tex[3].r = 0;

            skyMap = texture_read("skyboxY.ppm");
        }
        faces[i].texture = tex;
        d.shade = ShadeGouraud;
        d.textureMaps[0] = *skyMap;
            polygon_drawShade(&faces[i], src,&d, NULL);
    }
}

// Add the bottom face as a separate module
void module_checkboard(Module *md) {
    Color colors[4] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    if (md) {
        // Define the vertices of the cube
        Point vertices[8] = {
            { 0,  0, 0, 1.0 },
            {  4,  0, 0, 1.0 },
            {  4,  0,  4, 1.0 },
            { 0,  0,  4, 1.0 }
        };

        // Define the bottom face of the cube
        Polygon bottom;
        Point bottom_vertices[4] = { vertices[0], vertices[1], vertices[2], vertices[3] };

        Vector normal;
        Vector nlist[4];
        // Initialize polygon
        polygon_init(&bottom);

        // Calculate normals and set face
        normal_calculation(&bottom_vertices[0], &bottom_vertices[1], &bottom_vertices[2], &normal);
        for (int i = 0; i < 4; ++i) nlist[i] = normal;
        polygon_set(&bottom, 4, bottom_vertices);
        polygon_setNormals(&bottom, 4, nlist);
        polygon_setSided(&bottom, 0);
        polygon_setColors(&bottom, 4, colors);

        TextureCoord texlist[4];
        texlist[0].c = 0;
        texlist[0].r = 0;
        texlist[0].textureMapIntex = 1;
        texlist[1].c = 0;
        texlist[1].r = 300;
        texlist[1].textureMapIntex = 1;
        texlist[2].c = 300;
        texlist[2].r = 300;
        texlist[2].textureMapIntex = 1;
        texlist[3].c = 300;
        texlist[3].r = 0;
        texlist[3].textureMapIntex = 1;
        bottom.texture = texlist;
        module_polygon(md, &bottom);


        Line line1;
        Point pa1 = {  1,0,1,1};
        Point pb1 = {1,2,1,1};
        line1.a = pa1;
        line1.b = pb1;
        line1.zBuffer = 1;

        module_line(md,&line1);


        Line line2;
        Point pa2 = {3,0,1,1};
        Point pb2 = {3,2,1,1};
        line2.a = pa2;
        line2.b = pb2;
        line2.zBuffer = 1;

        module_line(md,&line2);

        Line line3;
        Point pa3 = {3,0,3,1};
        Point pb3 = {3,2,3,1};
        line3.a = pa3;
        line3.b = pb3;
        line3.zBuffer = 1;

        module_line(md,&line3);

        Line line4;
        Point pa4 = {1,0,3,1};
        Point pb4 = {1,2,3,1};
        line4.a = pa4;
        line4.b = pb4;
        line4.zBuffer = 1;

        module_line(md,&line4);


        Line line5;
        Point pa5 = {1,2,1,1};
        Point pb5 = {3,2,1,1};
        line5.a = pa5;
        line5.b = pb5;
        line5.zBuffer = 1;

        module_line(md,&line5);


        Line line6;
        Point pa6 = {3,2,3,1};
        Point pb6 = {3,2,1,1};
        line6.a = pa6;
        line6.b = pb6;
        line6.zBuffer = 1;

        module_line(md,&line6);

        Line line7;
        Point pa7 = {1,2,3,1};
        Point pb7 = {3,2,3,1};
        line7.a = pa7;
        line7.b = pb7;
        line7.zBuffer = 1;

        module_line(md,&line7);

        Line line8;
        Point pa8 = {1,2,3,1};
        Point pb8 = {1,2,1,1};
        line8.a = pa8;
        line8.b = pb8;
        line8.zBuffer = 1;

        module_line(md,&line8);
    }
}

// 在C代码中初始化顶点
void init_vertex(int sliceX, int sliceY, float scaleY, Point center, float radius, Point **vertex_set, int *vertex_count) {
    *vertex_count = (sliceX + 1) * (sliceY + 1);
    *vertex_set = (Point *)malloc((*vertex_count) * sizeof(Point));
    int index = 0;

    for (int y = 0; y <= sliceY; y++) {
        for (int x = 0; x <= sliceX; x++) {
            Point sb;
            float xSegment = (float)x / (float)sliceX;
            float ySegment = (float)y / (float)sliceY;
            float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            float yPos = cos(ySegment * M_PI);
            float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            sb.val[0] = xPos * radius * scaleY + center.val[0];
            sb.val[1] = yPos * radius + center.val[1];
            sb.val[2] = zPos * radius * scaleY + center.val[2];
            sb.val[3] = 1;
            (*vertex_set)[index++] = sb;
        }
    }
}

// 在C代码中初始化三角形
void init_triangles(Point *vertex_set, int sliceX, int sliceY, Triangle **triangle_set, int *triangle_count) {
    *triangle_count = sliceX * sliceY * 2;
    *triangle_set = (Triangle *)malloc((*triangle_count) * sizeof(Triangle));
    int index = 0;

    for (int y = 0; y < sliceY; y++) {
        for (int x = 0; x < sliceX; x++) {
            Point p1 = vertex_set[y * (sliceX + 1) + x];
            Point p2 = vertex_set[(y + 1) * (sliceX + 1) + x];
            Point p3 = vertex_set[(y + 1) * (sliceX + 1) + (x + 1)];
            Point p4 = vertex_set[y * (sliceX + 1) + (x + 1)];
            (*triangle_set)[index++] = (Triangle){p1, p2, p3, 1};
            (*triangle_set)[index++] = (Triangle){p1, p3, p4, 1};
        }
    }
}

void module_sphere(Module *md, Point center, float radius, float scaleY, int sliceX, int sliceY) {
    Point *vertex_set;
    int vertex_count;
    Triangle *triangle_set;
    int triangle_count;
    Color colors[3] = {
        {0.1, 0.1, 1},  // 红色
        {0.1, 0.1, 1},
        {0.1, 0.1, 1}
    };

    init_vertex(sliceX, sliceY, scaleY, center, radius, &vertex_set, &vertex_count);
    init_triangles(vertex_set, sliceX, sliceY, &triangle_set, &triangle_count);

    for (int i = 0; i < triangle_count; i++) {
        Polygon face;
        polygon_init(&face);
        Point pts[3] = {triangle_set[i].a, triangle_set[i].b, triangle_set[i].c};
        Vector normal;
        Vector nlist[3];
        normal_calculation(&pts[0], &pts[1], &pts[2], &normal);
        for (int j = 0; j < 3; j++) nlist[j] = normal;
        polygon_set(&face, 3, pts);
        polygon_setNormals(&face, 3, nlist);
        polygon_setSided(&face, 1);
        polygon_setColors(&face,3,colors);

        TextureCoord texlist[4];
        texlist[0].textureMapIntex = 0;
        texlist[1].textureMapIntex = 0;
        texlist[2].textureMapIntex = 0;
        texlist[3].textureMapIntex = 0;

        texlist[0].c = 10;
        texlist[0].r = 10;
        texlist[0].textureMapIntex = 0;
        texlist[1].c = 10;
        texlist[1].r = 50;
        texlist[1].textureMapIntex = 0;
        texlist[2].c = 50;
        texlist[2].r = 50;
        texlist[2].textureMapIntex = 0;
        texlist[3].c = 50;
        texlist[3].r = 10;
        texlist[3].textureMapIntex = 0;
        face.texture = texlist;
        module_polygon(md, &face);
    }
    free(vertex_set);
    free(triangle_set);
}



// Add a unit cube to the module
void module_cube(Module *md, int solid) {
    Color colors[4] = {
        {1, 0.5, 1},
        {1, 0.5, 1},
        {1, 0.5, 1},
        {1, 0.5, 1}
    };

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
polygon_setColors(&faces[0], 4, colors);
normal_calculation(&back[0], &back[1], &back[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[1], 4, back);
polygon_setNormals(&faces[1], 4, nlist);
polygon_setSided(&faces[1], 1);
polygon_setColors(&faces[1], 4, colors);

normal_calculation(&left[0], &left[1], &left[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[2], 4, left);
polygon_setNormals(&faces[2], 4, nlist);
polygon_setSided(&faces[2], 1);
polygon_setColors(&faces[2], 4, colors);

normal_calculation(&right[0], &right[1], &right[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[3], 4, right);
polygon_setNormals(&faces[3], 4, nlist);
polygon_setSided(&faces[3], 1);
polygon_setColors(&faces[3], 4, colors);

normal_calculation(&top[0], &top[1], &top[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[4], 4, top);
polygon_setNormals(&faces[4], 4, nlist);
polygon_setSided(&faces[4], 1);
polygon_setColors(&faces[4], 4, colors);

normal_calculation(&bottom[0], &bottom[1], &bottom[2], &normal);
for (int i = 0; i < 4; ++i) nlist[i] = normal;
polygon_set(&faces[5], 4, bottom);
polygon_setNormals(&faces[5], 4, nlist);
polygon_setSided(&faces[5], 1);
polygon_setColors(&faces[5], 4, colors);

            TextureCoord texlist[4];
            texlist[0].c = 10;
            texlist[0].r = 10;
            texlist[0].textureMapIntex = -1;
            texlist[1].c = 10;
            texlist[1].r = 500;
            texlist[1].textureMapIntex = -1;
            texlist[2].c = 500;
            texlist[2].r = 500;
            texlist[2].textureMapIntex = -1;
            texlist[3].c = 500;
            texlist[3].r = 10;
            texlist[3].textureMapIntex = -1;
for (int i = 0; i < 6; i++) {

    faces[i].texture = texlist;
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



void module_ball(Module *md, float radius, Point center, int solid) {
    Color colors[4] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    if (md) {
        // Golden ratio
        float phi = (1.0 + sqrt(5.0)) / 2.0;

        // Normalize the golden ratio to fit within a unit sphere
        float a = 1.0;
        float b = 1.0 / phi;

        // Define the vertices of the icosahedron
        Point vertices[12] = {
            { -a,  b,  0, 1.0 },
            {  a,  b,  0, 1.0 },
            { -a, -b,  0, 1.0 },
            {  a, -b,  0, 1.0 },
            {  0, -a,  b, 1.0 },
            {  0,  a,  b, 1.0 },
            {  0, -a, -b, 1.0 },
            {  0,  a, -b, 1.0 },
            {  b,  0, -a, 1.0 },
            {  b,  0,  a, 1.0 },
            { -b,  0, -a, 1.0 },
            { -b,  0,  a, 1.0 }
        };

        // Scale and translate vertices to the given radius and center
        for (int i = 0; i < 12; ++i) {
            vertices[i].val[0] = vertices[i].val[0] * radius + center.val[0];
            vertices[i].val[1] = vertices[i].val[1] * radius + center.val[1];
            vertices[i].val[2] = vertices[i].val[2] * radius + center.val[2];
        }

        // Define the faces of the icosahedron (each face is a triangle)
        int faces[20][3] = {
            {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
            {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
            {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
            {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
        };

        if (solid) {
            // Create polygons for each face
            Polygon poly;
            for (int i = 0; i < 20; ++i) {
                polygon_init(&poly);
                Point pts[3] = { vertices[faces[i][0]], vertices[faces[i][1]], vertices[faces[i][2]] };

                Vector normal;
                normal_calculation(&pts[0], &pts[1], &pts[2], &normal);

                Vector nlist[3] = { normal, normal, normal };

                polygon_set(&poly, 3, pts);
                polygon_setNormals(&poly, 3, nlist);
                polygon_setSided(&poly, 1);
                polygon_setColors(&poly, 3, colors);

                TextureCoord texlist[3] = {
                    {10, 10, 0},
                    {500, 10, 0},
                    {500, 500, 0}
                };
                poly.texture = texlist;

                module_polygon(md, &poly);
                polygon_clear(&poly);
            }
        } else {
            // Define the edges of the icosahedron
            int edge_indices[30][2] = {
                {0, 11}, {0, 5}, {0, 1}, {0, 7}, {0, 10},
                {1, 5}, {5, 11}, {11, 10}, {10, 7}, {7, 1},
                {1, 8}, {8, 7}, {7, 6}, {6, 10}, {10, 2},
                {2, 6}, {6, 3}, {3, 2}, {2, 4}, {4, 11},
                {4, 5}, {5, 9}, {9, 4}, {4, 3}, {3, 9},
                {9, 8}, {8, 1}, {8, 3}, {3, 6}, {6, 10}
            };

            // Create lines for each edge
            Line edge;
            for (int i = 0; i < 12; ++i) {
              //  line_set(&edge, vertices[edge_indices[i][0]], vertices[edge_indices[i][1]]);
              //  module_line(md, &edge);
                module_point(md,&vertices[i]);
            }
        }
    }
}





void module_plane(Module *md, Point **points, int WIDTH, int HEIGHT) {
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
                TextureCoord texlist[4];

                int texture_width = 1076;
                int texture_height = 1076;

                // Calculate texture coordinates
                texlist[0].c = (float)i / (WIDTH - 1) * texture_width;
                texlist[0].r = (float)j / (HEIGHT - 1) * texture_height;
                texlist[0].textureMapIntex = 0;
                texlist[1].c = (float)(i + 1) / (WIDTH - 1) * texture_width;
                texlist[1].r = (float)j / (HEIGHT - 1) * texture_height;
                texlist[1].textureMapIntex = 0;
                texlist[2].c = (float)(i + 1) / (WIDTH - 1) * texture_width;
                texlist[2].r = (float)(j + 1) / (HEIGHT - 1) * texture_height;
                texlist[2].textureMapIntex = 0;
                texlist[3].c = (float)i / (WIDTH - 1) * texture_width;
                texlist[3].r = (float)(j + 1) / (HEIGHT - 1) * texture_height;
                texlist[3].textureMapIntex = 0;

                polygon_init(&quad);
                polygon_set(&quad, 4, quad_points);
                normal_calculation(&quad_points[0], &quad_points[1], &quad_points[2], &normal);
                for (int k = 0; k < 4; ++k) nlist[k] = normal;
                polygon_setNormals(&quad, 4, nlist);
                polygon_setSided(&quad, 1);
                polygon_setColors(&quad, 4, &color);
                quad.texture = texlist;
                module_polygon(md, &quad);
            }
        }
    }
}



void module_plane_checkboard(Module *md, Point **points, int WIDTH, int HEIGHT) {
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
                TextureCoord texlist[4];

                int texture_width = 300;
                int texture_height = 300;

                // Calculate texture coordinates
                texlist[0].c = (float)i / (WIDTH - 1) * texture_width;
                texlist[0].r = (float)j / (HEIGHT - 1) * texture_height;
                texlist[0].textureMapIntex = 1;
                texlist[1].c = (float)(i + 1) / (WIDTH - 1) * texture_width;
                texlist[1].r = (float)j / (HEIGHT - 1) * texture_height;
                texlist[1].textureMapIntex = 1;
                texlist[2].c = (float)(i + 1) / (WIDTH - 1) * texture_width;
                texlist[2].r = (float)(j + 1) / (HEIGHT - 1) * texture_height;
                texlist[2].textureMapIntex = 1;
                texlist[3].c = (float)i / (WIDTH - 1) * texture_width;
                texlist[3].r = (float)(j + 1) / (HEIGHT - 1) * texture_height;
                texlist[3].textureMapIntex = 1;

                polygon_init(&quad);
                polygon_set(&quad, 4, quad_points);
                normal_calculation(&quad_points[0], &quad_points[1], &quad_points[2], &normal);
                for (int k = 0; k < 4; ++k) nlist[k] = normal;
                polygon_setNormals(&quad, 4, nlist);
                polygon_setSided(&quad, 1);
                polygon_setColors(&quad, 4, &color);
                quad.texture = texlist;
                module_polygon(md, &quad);
            }
        }
    }
}



// void module_particle(Module *md, Point *points, int size) {
//     for (int i = 0; i < size; ++i) {
//         module_point(md,&points[i]);
//     }
// }

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
    for (int i = 0; i < MAX_TEXTURES; i++) {
        ds->textureMaps[0].cols = -1;
        ds->textureMaps[0].rows = -1;
    }
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
    for (int i = 0; i < MAX_TEXTURES; i++) {
        to->textureMaps[i] = from->textureMaps[i];
    }
}


