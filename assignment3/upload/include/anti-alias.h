/**
 * Created by 曲辰熙 on 2024/5/26.
 * I'm trying to encapsulate all the anti-alias algorithms and plan to implement e.g. MMSA SSAA... in it, encapsulating
 * these functions I think will significantly reduce the speed of my algorithms, but I want to do it as a learning perspective.

**/
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
