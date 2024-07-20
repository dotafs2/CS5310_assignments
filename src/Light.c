//
// Created by DOTAFS on 2024/7/17.
//

#include "Light.h"
#include "stddef.h"
#include "fsMath.h"


#define PRINT_LIGHT 0

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


void light_copy(Light *to, Light *from) {
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

void lighting_clear(Lighting *l) {
    if (l == NULL) return;
    l->nLights = 0;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        light_init(&l->light[i]);
    }
}

void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness) {
    if ((l == NULL) || l->nLights >= MAX_LIGHTS) return;

    Light *newLight = &l->light[l->nLights];
    newLight->type = type;

    if (c) {
        newLight->color = *c;
    } else {
        newLight->color.c[0] = 0.0f;
        newLight->color.c[1] = 0.0f;
        newLight->color.c[2] = 0.0f;
    }

    if (dir) {
        newLight->direction = *dir;
    } else {
        for (int i = 0; i < 4; ++i) {
            newLight->direction.val[i] = 0.0;
        }
    }

    if (pos) {
        newLight->position = *pos;
    } else {
        for (int i = 0; i < 4; ++i) {
            newLight->position.val[i] = 0.0;
        }
    }

    newLight->cutoff = cutoff;
    newLight->sharpness = sharpness;

    l->nLights++;
}

void lighting_shading( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c ) {
    // Normal N, ViewPoint V, 3D point P, Cb body color, Cs surface color, Sharpness s, result color C

    Vector L, H, tempN, tempV;
    vector_copy(&tempN, N);
    vector_copy(&tempV, V);
    vector_normalize(&tempN);
    vector_normalize(&tempV);

    Color result = { {0.0f, 0.0f, 0.0f} };



    for (int i = 0; i < l->nLights; ++i) {
        Light *light = &l->light[i];
        Color I = { {0.0f, 0.0f, 0.0f} };

        if (light->type == LightAmbient) {
            // Ambient lighting
            for (int j = 0; j < 3; ++j) {
                I.c[j] = light->color.c[j] * Cb->c[j];
            }
#if PRINT_LIGHT
            printf("ambient light %d: %.2f %.2f %.2f\n", i, I.c[0], I.c[1], I.c[2]);
#endif
        } else {
            // light direction vector L
            if (light->type == LightDirect) {
                vector_copy(&L, &light->direction);
            } else {
                vector_subtract((Vector *) &light->position, (Vector *) p, &L);
            }

            vector_normalize(&L);
            // Calculate halfway vector H : L + V / ||L + V||
            vector_add(&L, &tempV,&H);
            vector_divide_num(&H,2);
            vector_normalize(&H);


            float NdotL = (float)vector_dot(&L,&tempN);
            float NdotH = (float)vector_dot(&H,&tempN);

            // if twoside light could calculate with oppsite normal
            if (!oneSided) {
                NdotL = fabsf(NdotL);
                NdotH = fabsf(NdotH);
            }
            // if oneside and direction of light and normal are oppsite, skip
            if (oneSided && (vector_dot(&L,&tempN)< 0)) {
                continue;
            }
            // if viewpoint and light are oppsite, skip
            if (vector_dot(&L, &tempV) < 0) {
                continue;
            }

            // I = Body+Surface = Cb x C_Ld x (L dot N) +  C_Ld x C_s x (H dot N)^n
            // Point and Spot lighting
            if (light->type == LightDirect || light->type == LightPoint || light->type == LightSpot) {
                for (int j = 0; j < 3; ++j) {
                    I.c[j] = Cb->c[j] * light->color.c[j] * NdotL + Cs->c[j] * light->color.c[j] * powf(NdotH, s);
                }

                if (light->type == LightSpot) {
                    float cosAlpha = -(float)vector_dot(&light->direction, &L);
                    if (cosAlpha > light->cutoff) {
                        float falloff = powf(cosAlpha, light->sharpness);
                        for (int j = 0; j < 3; ++j) {
                            I.c[j] *= falloff;
                        }
                    } else {
                        for (int j = 0; j < 3; ++j) {
                            I.c[j] = 0.0f;
                        }
                    }
                }
            }
#if PRINT_LIGHT
            printf("point light %d: %.2f %.2f %.2f\n", 1, result.c[0], result.c[1], result.c[2]);
#endif
        }
        for (int j = 0; j < 3; ++j) {
            result.c[j] += I.c[j];
        }
    }

    for (int j = 0; j < 3; ++j) {
        if (result.c[j] > 1.0f) result.c[j] = 1.0f;
        if (result.c[j] < 0.0f) result.c[j] = 0.0f;
    }
    *c = result;
}
