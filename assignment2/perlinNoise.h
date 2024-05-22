//
// Created by 曲辰熙 on 2024/5/22.
//

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>

class perlinNoise {
public:
    perlinNoise(unsigned int seed = rand());

    float noise(float x, float y, float z) const;

private:
    std::vector<int> p;

    static float fade(float t);
    static float lerp(float t, float a, float b);
    static float grad(int hash, float x, float y, float z);
};

#endif // PERLINNOISE_H
