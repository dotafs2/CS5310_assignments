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

void lighting_init(Lighting *lighting) {
    if (lighting == NULL) return;

    lighting->nLights = 0; // 初始化光源数量为0

    // 初始化所有光源
    for (int i = 0; i < MAX_LIGHTS; i++) {
        lighting->light[i].type = LightNone;
        lighting->light[i].color.c[0] = 0.0f;
        lighting->light[i].color.c[1] = 0.0f;
        lighting->light[i].color.c[2] = 0.0f;

        for (int j = 0; j < 4; ++j) {
            lighting->light[i].direction.val[j] = 0.0;
            lighting->light[i].position.val[j] = 0.0;
        }

        lighting->light[i].cutoff = 1.0f;    // 默认值为1.0，表示聚光灯的角度为180度（cos(0) = 1）
        lighting->light[i].sharpness = 1.0f; // 默认锐度
    }
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


void lighting_copy(Lighting *to, Lighting *from) {
    if ((to == NULL) || (from == NULL)) return;

    // 复制光源的数量
    to->nLights = from->nLights;

    // 为每个光源分配并复制数据
    for (int i = 0; i < from->nLights; i++) {
        to->light[i].type = from->light[i].type;
        to->light[i].color = from->light[i].color;

        // 如果需要深拷贝向量和点
        for (int j = 0; j < 4; ++j) {
            to->light[i].direction.val[j] = from->light[i].direction.val[j];
            to->light[i].position.val[j] = from->light[i].position.val[j];
        }

        to->light[i].cutoff = from->light[i].cutoff;
        to->light[i].sharpness = from->light[i].sharpness;
    }
}



void light_print(Light *light) {
    if (light == NULL) {
        printf("Light is NULL\n");
        return;
    }

    const char* lightTypeStr;
    switch (light->type) {
        case LightNone:
            lightTypeStr = "LightNone";
        break;
        case LightAmbient:
            lightTypeStr = "LightAmbient";
        break;
        case LightDirect:
            lightTypeStr = "LightDirect";
        break;
        case LightPoint:
            lightTypeStr = "LightPoint";
        break;
        case LightSpot:
            lightTypeStr = "LightSpot";
        break;
        default:
            lightTypeStr = "Unknown";
        break;
    }
    printf("Light Information:\n");
    printf("  Type: %s\n", lightTypeStr);
    printf("  Color: [%.2f, %.2f, %.2f]\n", light->color.c[0], light->color.c[1], light->color.c[2]);
    printf("  Direction: [%.2f, %.2f, %.2f, %.2f]\n", light->direction.val[0], light->direction.val[1], light->direction.val[2], light->direction.val[3]);
    printf("  Position: [%.2f, %.2f, %.2f, %.2f]\n", light->position.val[0], light->position.val[1], light->position.val[2], light->position.val[3]);
    printf("  Cutoff: %.2f\n", light->cutoff);
    printf("  Sharpness: %.2f\n", light->sharpness);
}

void lighting_print(Lighting *lighting) {
    if (lighting == NULL) {
        printf("Lighting is NULL\n");
        return;
    }

    printf("Lighting Information:\n");
    printf("  Number of Lights: %d\n", lighting->nLights);

    for (int i = 0; i < lighting->nLights; ++i) {
        printf("  Light %d:\n", i);
        light_print(&lighting->light[i]);
    }
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
    if((N==NULL) || (V == NULL) || (l == NULL)) return;

    Vector L, H, tempN, tempV;
    vector_init(&tempN);
    vector_init(&tempV);
    vector_copy(&tempN, N);
    vector_copy(&tempV, V);
    vector_normalize(&tempN);
    vector_normalize(&tempV);
    Color result = { {0.0f, 0.0f, 0.0f} };
#if PRINT_LIGHT
    printf("input Normal N : %.2f %.2f %.2f\n", N->val[0], N->val[1], N->val[2]);
    printf("input View Vector V : %.2f %.2f %.2f\n", V->val[0], V->val[1], V->val[2]);
    printf("input oneSide: %d\n", oneSided);
#endif
  //  printf("input Normal N : %.2f %.2f %.2f\n", N->val[0], N->val[1], N->val[2]);

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
#if PRINT_LIGHT
            printf("light direction %d: %.2f %.2f %.2f\n", i, L.val[0], L.val[1],L.val[2]);
#endif

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
#if PRINT_LIGHT
                printf("skip because oneside and direction of light and normal are oppsite \n");
#endif
                continue;
            }
            // if viewpoint and light are oppsite, skip
            if (vector_dot(&L, &tempV) < 0) {
#if PRINT_LIGHT
                printf("skip because viewpoint and light are oppsite \n");
#endif
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
            printf("point light %d: %.2f %.2f %.2f\n", 1, I.c[0], I.c[1], I.c[2]);
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



void lighting_shading_phong( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c, Color ka, Color kd, Color ks, int R) {
    // Normal N, ViewPoint V, 3D point P, Cb body color, Cs surface color, Sharpness s, result color C
    if((N==NULL) || (V == NULL) || (l == NULL)) return;

    Vector L, H, tempN, tempV;
    vector_init(&tempN);
    vector_init(&tempV);
    vector_copy(&tempN, N);
    vector_copy(&tempV, V);
    vector_normalize(&tempN);
    vector_normalize(&tempV);
    Color result = { {0.0f, 0.0f, 0.0f} };
#if PRINT_LIGHT
    printf("input Normal N : %.2f %.2f %.2f\n", N->val[0], N->val[1], N->val[2]);
    printf("input View Vector V : %.2f %.2f %.2f\n", V->val[0], V->val[1], V->val[2]);
    printf("input oneSide: %d\n", oneSided);
#endif
  //  printf("input Normal N : %.2f %.2f %.2f\n", N->val[0], N->val[1], N->val[2]);

    for (int i = 0; i < l->nLights; ++i) {
        Light *light = &l->light[i];
        Color I = { {0.0f, 0.0f, 0.0f} };

        if (light->type == LightAmbient) {
            // Ka * Ia
            for (int j = 0; j < 3; ++j) {
                I.c[j] = light->color.c[j] * ka.c[j];
            }

#if PRINT_LIGHT
            printf("ambient light %d: %.2f %.2f %.2f\n", i, I.c[0], I.c[1], I.c[2]);
#endif
        } else {
            // light direction vector L
            if (light->type == LightDirect) {
                vector_copy(&L, &light->direction);
            } else {
                for (int j = 0; j < 3; ++j) {
                    L.val[j] = light->position.val[j] - p->val[j];
                }
            }
#if PRINT_LIGHT
            printf("light direction %d: %.2f %.2f %.2f\n", i, L.val[0], L.val[1],L.val[2]);
#endif

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
#if PRINT_LIGHT
                printf("skip because oneside and direction of light and normal are oppsite \n");
#endif
                continue;
            }
//             // if viewpoint and light are oppsite, skip
//             if (vector_dot(&L, &tempV) < 0) {
// #if PRINT_LIGHT
//                 printf("skip because viewpoint and light are oppsite \n");
// #endif
//                 continue;
//             }



            // 镜面反射用的
            Vector ReflectDir = {0,0,0,0};
            for(int j = 0 ; j < 3; ++j) {
                ReflectDir.val[j] = 2 * NdotL * N->val[j] - L.val[j];
            }
            vector_normalize(&ReflectDir);

            float RdotV = (float)vector_dot(&ReflectDir,&tempV);

            // I = Body+Surface = Cb x C_Ld x (L dot N) +  C_Ld x C_s x (H dot N)^n
            // Point and Spot lighting
            if (light->type == LightDirect || light->type == LightPoint || light->type == LightSpot) {
                for (int j = 0; j < 3; ++j) {
                    // 环境光 Ia = ka * Ia
                    I.c[j] += light->color.c[j] * ka.c[j];

                    // 漫反射光 Id = kd * Id * max(NdotL,0)
                    I.c[j] += kd.c[j] * light->color.c[j] * fmaxf(NdotL,0);

                    // 镜面反射光 Is = ks * Is * max(RdotV,0)^n
                    I.c[j] += ks.c[j] * light->color.c[j] * powf(fmaxf(RdotV,0),(float)R);
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
            printf("point light %d: %.2f %.2f %.2f\n", 1, I.c[0], I.c[1], I.c[2]);
#endif
        }
        for (int j = 0; j < 3; ++j) {
            result.c[j] += I.c[j];
        }

    }

    // 剪裁防止超出边界
    for (int j = 0; j < 3; ++j) {
        if (result.c[j] > 1.0f) result.c[j] = 1.0f;
        if (result.c[j] < 0.0f) result.c[j] = 0.0f;
    }

    *c = result;

}





