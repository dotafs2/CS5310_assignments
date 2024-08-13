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

#define XCOUNT 10
#define YCOUNT 10
#define ZCOUNT 10
#define PARTICLECOUNT (XCOUNT * YCOUNT * ZCOUNT)
#define GRAVITY (-9.8)
#define MAX_NEIGHBOURS 100
#define TIME_STEP 0.01
#define SOLVER_ITERATIONS 10
#define RADIUS 0.03
#define RANGE_H  (10 * RADIUS)
#define RESTDISTANCE (5* RADIUS)
#define STIFFNESS 0.01
#define RELAXATION_EPSILON 1000
#define DAMPING 0.95
#define RANGE_X_MIN 1
#define RANGE_X_MAX 3
#define RANGE_Y_MIN 0
#define RANGE_Y_MAX 2
#define RANGE_Z_MIN 1
#define RANGE_Z_MAX 3



#define FOGAMOUNT 1000


#define simSizeX ((int)ceil((RANGE_X_MAX - RANGE_X_MIN) / RANGE_H))
#define simSizeY ((int)ceil((RANGE_Y_MAX - RANGE_Y_MIN) / RANGE_H))
#define simSizeZ ((int)ceil((RANGE_Z_MAX - RANGE_Z_MIN) / RANGE_H))


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

typedef struct {
        float r;
        float c;
        int textureMapIntex;
} TextureCoord;

 // Polygon structure
typedef struct {
        int oneSided;  // whether to consider the polygon one-sided (1) or two-sided (0) for shading
        int numVertex; // Number of vertices
        Point *vertex; // vertex information
        Color *color;  // color information for each vertex
        Vector *normal;// surface normal information for each vertex
        int zBuffer;   // whether to use the z-buffer; should default to true (1)
        TextureCoord *texture;
    } Polygon;

typedef struct {
        Color *data;
        float* alpha;
        int alphaFlag;
        Color ambientCoefficients; // 环境光反射系数
        Color specularCoefficients; // 镜面反射系数数组
        Color diffuseCoefficients;  // 漫反射系数数组
        int R; // 光泽度
        int rows;
        int cols;
}TextureMap;



typedef struct Particle Particle;

typedef struct Fog Fog;

typedef struct {
        Particle **elements;
        int size;
        int capacity;
} Bin;

    // Particle structure for water simulation
    struct Particle{
        Point position;  // Position of the particle
        Point pred_position; // Next dt position
        Point buffer_position; // Store the position going to put into pred_position
        Vector v; // Velocity of the particle
        Vector acceleration; // Acceleration of the particle
        Vector vorticity;
        Vector externalForce;
        Vector oneTimeForce; // only use one time and dead
        int oneTimeForceAlive;
        double mass;     // Mass of the particle
        Particle **neighbours;
        int neighboursSize;
        int neighboursCapacity;
        double lambda;
        int id;
    };

    struct Fog {
        Point position;
        float alpha;
        Color c;
    };




#ifdef __cplusplus
    }
#endif



#endif //DEFINITION_H
