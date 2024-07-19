//
// Created by DOTAFS on 2024/7/17.
//

#include "Light.h"



void light_init(Light *light) {
    if (light == NULL) return;

    light->type = LightNone;

    light->color.c[0] = 0.0f;
    light->color.c[1] = 0.0f;
    light->color.c[2] = 0.0f;

    for (int i = 0; i < 4; ++i) {
        light->direction.val[i] = 0.0;
        light->position.val[i] = 0.0;
    }

    light->cutoff = 1.0f;    // Assuming 1.0 as default value for cosine of the cutoff angle
    light->sharpness = 1.0f; // Assuming 1.0 as default sharpness
}


void light_copy(Light *to, const Light *from) {
    if ((to == NULL) || (from == NULL)) return;

    to->type = from->type;
    to->color = from->color;
    to->direction = from->direction;
    to->position = from->position;
    to->cutoff = from->cutoff;
    to->sharpness = from->sharpness;
}


Lighting* lighting_create(void) {
    Lighting *lighting = (Lighting*)malloc(sizeof(Lighting));
    if (lighting == NULL) return NULL;

    lighting->nLights = 0;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        light_init(&lighting->light[i]);
    }
    return lighting;
}