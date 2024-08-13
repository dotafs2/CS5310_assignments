//
// Created by DOTAFS on 2024/7/19. all the struct is put in here
//

#include "stddef.h"
#include "stdlib.h"
#include <stdio.h>
#include "math.h"
#ifndef DEFINITION_H
#define DEFINITION_H

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_LIGHTS 64

typedef enum {
    LightNone,
    LightAmbient,
    LightDirect,
    LightPoint,
    LightSpot,
} LightType;


    // Point structure
    typedef struct {
        double val[4];  // Point3D
    } Point;

    typedef struct {
        double val[2]; // Point2D
    } Point2;

    // Line structure
    typedef struct {
        int zBuffer;  // whether to use the z-buffer, should default to true (1)
        Point a;      // starting point
        Point b;      // ending point
    } Line;

    // Circle structure
    typedef struct {
        double r;  // radius
        Point c;   // center
    } Circle;

    // Ellipse structure
    typedef struct {
        double ra;  // major axis radius
        double rb;  // minor axis radius
        Point c;    // center
        double a;   // angle of major axis relative to the X-axis (optional)
    } Ellipse;

    // Polyline structure
    typedef struct {
        int zBuffer;   // whether to use the z-buffer, should default to true (1)
        int numVertex; // number of vertices
        Point *vertex; // vertex information
    } Polyline;

    // Triangle functions
    typedef struct {
        Point a, b, c;
        int zBuffer;
    } Triangle;


// Structure for a pixel with RGB color, alpha, and depth
typedef struct {
    float rgb[3];
    float a;
    float z;
} FPixel;

// Structure for an image, including pixel data, dimensions, and auxiliary data
typedef struct {
    FPixel *data;
    int rows;
    int cols;
    float *depth;
    float *alpha;
    int max_val;
    char filename[256]; // Filename for saving/loading image
} Image;

// Structure for a color with RGB components
typedef struct {
    float c[3];
} Color;

typedef struct {
    double val[4];
} Vector;

static Color red = {1.0f, 0.0f, 0.0f};
static Color green = {0.0f, 1.0f, 0.0f};
static Color blue = {0.0f, 0.0f, 1.0f};

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



#ifdef __cplusplus
    }
#endif



#endif //DEFINITION_H
