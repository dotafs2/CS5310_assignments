//
// Created by 曲辰熙 on 2024/5/26.
//

#ifndef ASSIGNMENT3_ANTI_ALIAS_H
#define ASSIGNMENT3_ANTI_ALIAS_H

#include "Image.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

/**
 *
 * @param src: input graph
 * @param sample_rate: sample per pixel
 * Uniform SSAA, maybe add poisson disk sampling later
 */
void SSAA(Image* src, int sample_rate);

#endif //ASSIGNMENT3_ANTI_ALIAS_H
