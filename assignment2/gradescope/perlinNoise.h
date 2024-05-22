//
// Created by 曲辰熙 on 2024/5/22.
//
/*
 * Implementation file for perlin noise, mainly focusing on the implementation of 2D perlin noise.
 */

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>

// PerlinNoise class to generate Perlin noise values
class perlinNoise {
public:
    // Constructor to initialize the permutation vector based on the seed
    perlinNoise(unsigned int seed = rand());

    // Function to generate Perlin noise value for given 3D coordinates (x, y, z)
    float noise(float x, float y, float z) const;

private:
    // Permutation vector used to generate Perlin noise
    std::vector<int> p;

    // Fade function to ease coordinate values to avoid abrupt changes
    static float fade(float t);

    // Linear interpolation function to blend values
    static float lerp(float t, float a, float b);

    // Gradient function to calculate dot product of the distance and gradient vectors
    static float grad(int hash, float x, float y, float z);
};

#endif // PERLINNOISE_H
