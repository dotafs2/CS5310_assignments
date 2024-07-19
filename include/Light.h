//
// Created by DOTAFS on 2024/7/17.
//
#ifdef __cplusplus
extern "C" {
#endif
#ifndef LIGHT_H
#define LIGHT_H
#include "Image.h"
#include "Polygon.h"
#include "Line.h"

#define MAX_LIGHTS 64

typedef enum {
    LightNone,
    LightAmbient,
    LightDirect,
    LightPoint,
    LightSpot,
} LightType;

typedef struct {
    LightType type;
    Color color;
    Vector direction;
    Point position;
    float cutoff; // stores the cosine of the cutoff angle of a spotlight
    float sharpness; // coefficient of the falloff function (power for cosine)
} Light;

typedef struct {
    int nLights;
    Light light[MAX_LIGHTS];
} Lighting;


void light_init( Light *light ); // initialize the light to default values.
void light_copy( Light *to, Light *from ); // copy the light information.
Lighting *lighting_create( void ); // allocate and return a new lighting structure set to default values.
void lighting_delete( Lighting *lights ); // delete the Lighting struct.
void lighting_init( Lighting *l ); // initialize the lighting structure to default values.
void lighting_clear( Lighting *l ); // reset the Lighting struct to 0 lights.
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness ); // add a new light to the Lighting structure given the parameters, some of which may be NULL, depending upon the type. Make sure you don’t add more lights than MAX LIGHTS.
void lighting_shading( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c ); // calculate the proper color given the normal N, view vector V, 3D point P, body color Cb, surface color Cs, sharpness value s, the lighting, and whether the polygon is one-sided or two-sided. Put the result in the Color c.



#endif //LIGHT_H


#ifdef __cplusplus
}
#endif