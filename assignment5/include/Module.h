#ifndef MODULE_H
#define MODULE_H

#include <stdlib.h>
#include <string.h>
#include "fsMath.h"
#include "Polygon.h"
#include "Line.h"
#include "Image.h"

typedef enum {
    ObjNone,
    ObjPoint,
    ObjLine,
    ObjPolyline,
    ObjPolygon,
    ObjModule,
    ObjTranslate2D,
    ObjScale2D,
    ObjRotateZ,
    ObjShear2D,
    ObjIdentity
} ObjectType;




typedef union {
    Point point;
    Line line;
    Polyline polyline;
    Polygon polygon;
    void *module;
    void *matrix;
} Object;

typedef struct Element {
    ObjectType type;
    Object obj;
    struct Element *next;
} Element;

typedef struct {
    Element *head;
    Element *tail;
} Module;

Element *element_create();
Element *element_init(ObjectType type, void *obj);
void element_delete(Element *e);

Module *module_create();
void module_clear(Module *md);
void module_delete(Module *md);
void module_insert(Module *md, Element *e);
void module_module(Module *md, Module *sub);
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);
void module_identity(Module *md);
void module_translate2D(Module *md, double tx, double ty);
void module_scale2D(Module *md, double sx, double sy);
void module_rotateZ(Module *md, double cth, double sth);
void module_shear2D(Module *md, double shx, double shy);

#ifdef __cplusplus
}
#endif

#endif // MODULE_H

// Module.c
