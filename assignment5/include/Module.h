#ifndef MODULE_H
#define MODULE_H

#include <stdlib.h>
#include <string.h>
#include "fsMath.h"
#include "Polygon.h"
#include "Line.h"
#include "Image.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum { // example of an enumerated type
  ObjNone,
  ObjLine,
  ObjPoint,
  ObjPolyline,
  ObjPolygon,
  ObjIdentity,
  ObjMatrix,
  ObjColor,
  ObjBodyColor,
  ObjSurfaceColor,
  ObjSurfaceCoeff,
  ObjLight,
  ObjModule
} ObjectType;

typedef enum{
    ShadeFrame,
    ShadeConstant,
    ShadeFlat,
    ShadeGouraud,
    ShadePhong
} ShadeMethod;

typedef struct DrawState {
    Color color;
    Color flatColor;
    Color body;
    Color surface;
    float surfaceCoeff;
    ShadeMethod shade;
    int zBufferFlag;
    Point viewer;
} DrawState;


typedef union {
    Point point;
    Line line;
    Polyline polyline;
    Polygon polygon;
    void *module;
    void *matrix;
    Color color;
    float coeff;
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


typedef struct {
}Lighting;

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
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);
void module_cube(Module *md, int solid);
void module_color(Module *md, Color *c);
void module_bodyColor(Module *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float coeff);
DrawState *drawstate_create( void );
void drawstate_setColor( DrawState *s, Color c );
void drawstate_setBody( DrawState *s, Color c );
void drawstate_setSurface( DrawState *s, Color c );
void drawstate_setSurfaceCoeff( DrawState *s, float f );
void drawstate_copy( DrawState *to, DrawState *from );

#ifdef __cplusplus
}
#endif

#endif // MODULE_H


// Module.c
