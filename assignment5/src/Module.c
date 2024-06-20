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
                e->obj.module = obj; // No need to duplicate modules
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