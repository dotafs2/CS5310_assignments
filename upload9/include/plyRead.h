//
// Created by DOTAFS on 2024/7/22.
//
#include "fsMath.h"
#include "Polygon.h"
#ifndef PLYREAD_H
#define PLYREAD_H
#ifdef __cplusplus
extern "C" {
#endif
#define MaxVertices (10)

typedef enum {
    type_float32,
    type_uint8,
    type_int32,
    type_list,
    type_none
} ply_type;

typedef struct {
    ply_type type;
    ply_type listCardType;
    ply_type listDataType;
    char name[32];
    void *next;
} ply_property;

ply_type plyType(char *buffer);
int readPLY(char filename[], int *nPolygons, Polygon **plist, Color **clist, int estNormals);

#ifdef __cplusplus
}
#endif

#endif //PLYREAD_H
