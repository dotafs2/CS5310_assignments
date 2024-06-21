#include "Module.h"

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
                e->obj.polyline = *(Polyline *)obj;
                break;
            case ObjPolygon:
                e->obj.polygon = *(Polygon *)obj;
                break;
            case ObjModule:
                e->obj.module = obj;
                break;
            case ObjTranslate2D:
            case ObjScale2D:
            case ObjRotateZ:
            case ObjShear2D:
                e->obj.matrix = obj;
                break;
            default:
                free(e);
                return NULL;
        }
    }
    return e;
}

// Delete an element and free its memory
void element_delete(Element *e) {
    if (e) {
        if (e->type == ObjModule || e->type == ObjTranslate2D || e->type == ObjScale2D || e->type == ObjRotateZ || e->type == ObjShear2D) {
            free(e->obj.module);
        }
        free(e);
    }
}

// Create an empty module
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
        Element *e = element_create();
        e->type = ObjIdentity;
        module_insert(md, e);
    }
}

// Add a translation matrix to the module
void module_translate2D(Module *md, double tx, double ty) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_translate2D(m, tx, ty);
        Element *e = element_init(ObjTranslate2D, m);
        module_insert(md, e);
    }
}

// Add a scale matrix to the module
void module_scale2D(Module *md, double sx, double sy) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_scale2D(m, sx, sy);
        Element *e = element_init(ObjScale2D, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix about the Z axis to the module
void module_rotateZ(Module *md, double cth, double sth) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateZ(m, cth, sth);
        Element *e = element_init(ObjRotateZ, m);
        module_insert(md, e);
    }
}

// Add a 2D shear matrix to the module
void module_shear2D(Module *md, double shx, double shy) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_shear2D(m, shx, shy);
        Element *e = element_init(ObjShear2D, m);
        module_insert(md, e);
    }
}

void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src) {
    if (!md || !VTM || !GTM || !ds || !src) {
        return;
    }

    Element *current = md->head;
    Matrix LTM; // Local Transformation Matrix
    Matrix tempGTM;

    while (current) {
        switch (current->type) {
            case ObjNone:
                // Do nothing
                break;
            case ObjPoint:
                // Transform the point and draw it
            {
                Point tempPoint;
                matrix_xformPoint(GTM, &(current->obj.point), &tempPoint);
                matrix_xformPoint(VTM, &tempPoint, &tempPoint);
                point_draw(&tempPoint, src, ds->color);
            }
                break;
            case ObjLine:
                // Transform the line and draw it
            {
                Line tempLine = current->obj.line;
                matrix_xformLine(GTM, &tempLine);
                matrix_xformLine(VTM, &tempLine);
                line_draw(&tempLine, src, ds->color);
            }
                break;
            case ObjPolyline:
                // Transform the polyline and draw it
            {
                Polyline tempPolyline;
                polyline_copy(&tempPolyline, &(current->obj.polyline));
                matrix_xformPolyline(GTM, &tempPolyline);
                matrix_xformPolyline(VTM, &tempPolyline);
                polyline_draw(&tempPolyline, src, ds->color);
                polyline_free(&tempPolyline);
            }
                break;
            case ObjPolygon:
                // Transform the polygon and draw it
            {
                Polygon tempPolygon;
                polygon_copy(&tempPolygon, &(current->obj.polygon));
                matrix_xformPolygon(GTM, &tempPolygon);
                matrix_xformPolygon(VTM, &tempPolygon);
                polygon_draw(&tempPolygon, src, ds->color);
                polygon_free(&tempPolygon);
            }
                break;
            case ObjModule:
                // Recursively draw the submodule
            {
                Matrix newGTM;
                matrix_multiply(GTM, GTM, &newGTM);
                module_draw(current->obj.module, VTM, &newGTM, ds, lighting, src);
            }
                break;
            case ObjTranslate2D:
            case ObjScale2D:
            case ObjRotateZ:
            case ObjShear2D:
                // Apply the transformation matrix
                matrix_multiply(GTM, (Matrix *)current->obj.matrix, &tempGTM);
                matrix_copy(GTM, &tempGTM);
                break;
            case ObjIdentity:
                // Reset the GTM to identity
                matrix_identity(GTM);
                break;
            default:
                // Unknown type, do nothing
                break;
        }
        current = current->next;
    }
}

// Add a 3D translation matrix to the module
void module_translate(Module *md, double tx, double ty, double tz) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_translate(m, tx, ty, tz);
        Element *e = element_init(ObjTranslate, m);
        module_insert(md, e);
    }
}

// Add a 3D scale matrix to the module
void module_scale(Module *md, double sx, double sy, double sz) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_scale(m, sx, sy, sz);
        Element *e = element_init(ObjScale, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix about the X axis to the module
void module_rotateX(Module *md, double cth, double sth) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateX(m, cth, sth);
        Element *e = element_init(ObjRotateX, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix about the Y axis to the module
void module_rotateY(Module *md, double cth, double sth) {
    if (md) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateY(m, cth, sth);
        Element *e = element_init(ObjRotateY, m);
        module_insert(md, e);
    }
}

// Add a rotation matrix that orients to the orthonormal axes u, v, w to the module
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w) {
    if (md && u && v && w) {
        Matrix *m = (Matrix *)malloc(sizeof(Matrix));
        matrix_identity(m);
        matrix_rotateXYZ(m, u, v, w);
        Element *e = element_init(ObjRotateXYZ, m);
        module_insert(md, e);
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
                {1,vertices[1], vertices[5] },\
                {1, vertices[2], vertices[6] },
                {1,vertices[3], vertices[7] }
        };

        // Define the faces of the cube
        Polygon faces[6];
        for (int i = 0; i < 6; i++) {
            polygon_init(&faces[i]);
        }

        // Front face
        polygon_set(&faces[0], 4, (Point[]){ vertices[0], vertices[1], vertices[2], vertices[3] });
        // Back face
        polygon_set(&faces[1], 4, (Point[]){ vertices[4], vertices[5], vertices[6], vertices[7] });
        // Left face
        polygon_set(&faces[2], 4, (Point[]){ vertices[0], vertices[3], vertices[7], vertices[4] });
        // Right face
        polygon_set(&faces[3], 4, (Point[]){ vertices[1], vertices[2], vertices[6], vertices[5] });
        // Top face
        polygon_set(&faces[4], 4, (Point[]){ vertices[3], vertices[2], vertices[6], vertices[7] });
        // Bottom face
        polygon_set(&faces[5], 4, (Point[]){ vertices[0], vertices[1], vertices[5], vertices[4] });

        if (solid) {
            // Add polygons to the module
            for (int i = 0; i < 6; i++) {
                module_polygon(md, &faces[i]);
            }
        } else {
            // Add lines to the module
            for (int i = 0; i < 12; i++) {
                module_line(md, &edges[i]);
            }
        }

        // Free the polygons
        for (int i = 0; i < 6; i++) {
            polygon_clear(&faces[i]);
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
    ds->color = (Color){0, 0, 0}; // Default to black
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
