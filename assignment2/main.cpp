//
// Created by 曲辰熙 on 2024/5/22.
//
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <perlinNoise.h>
extern "C" {
#include "Image.h"
}

static const int numOfFrames = 100;

bool folder_exists(const char* folder) {
    struct stat info;
    if (stat(folder, &info) != 0) {
        std::cerr << "Cannot access " << folder << ": " << strerror(errno) << std::endl;
        return false;
    } else if (!(info.st_mode & S_IFDIR)) {
        std::cerr << folder << " is not a directory" << std::endl;
        return false;
    }
    return true;
}

bool execute_command(const char* command) {
    int result = system(command);
    if (result != 0) {
        std::cerr << "Command failed: " << command << std::endl;
        return false;
    }
    return true;
}

// one level perlin
void generateCloudsPerlin(Image *src, float scale, float persistence) {
    int rows = src->rows;
    int cols = src->cols;

    perlinNoise pn;

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = (float)col / cols * scale;
            float y = (float)r / rows * scale;

            float noise = pn.noise(x, y, 0.5);
            float t = pow(noise, persistence);

            float red = t;
            float green = t;
            float blue = t;

            FPixel pixel = { { red, green, blue }, 1.0f, 1.0f };
            image_setf(src, r, col, pixel);
        }
    }
}

// several level perlin
void generateCloudsPerlin2(Image *src, float baseScale, float basePersistence) {
    int rows = src->rows;
    int cols = src->cols;

    perlinNoise pn;

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = (float)col / cols;
            float y = (float)r / rows;

            float noise = 0.0f;
            float scale = baseScale;
            float persistence = basePersistence;

            for (int i = 0; i < 6; i++) {
                noise += pn.noise(x * scale, y * scale, 0.5) * persistence;
                scale *= 2.0f;
                persistence *= 0.5f;
            }

            noise = (noise - 1.0f) / 2.0f;
            noise = pow(noise, 3.0f);

            float red = 0.5f + 0.5f * noise;
            float green = 0.5f + 0.5f * noise;
            float blue = 0.5f + 0.5f * noise;

            FPixel pixel = { { red, green, blue }, 1.0f, 1.0f };
            image_setf(src, r, col, pixel);
        }
    }
}
void generateCloudsPerlin3(Image *src, float baseScale, float basePersistence, perlinNoise& perlin, float time) {
    int rows = src->rows;
    int cols = src->cols;

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = (float)col / cols;
            float y = (float)r / rows;

            float noise = 0.0f;
            float scale = baseScale;
            float persistence = basePersistence;

            for (int i = 0; i < 5; i++) {
                noise += perlin.noise(x*scale, y*scale, time) * persistence;
                scale *= 2.0f;
                persistence *= 0.5f;
            }

            noise = (noise - 1.0f) / 2.0f;
            noise = pow(noise, 3.0f);

            float red = 0.5f + 0.5f * noise;
            float green = 0.5f + 0.5f * noise;
            float blue = 0.5f + 0.5f * noise;

            FPixel pixel = { { red, green, blue }, 1.0f, 1.0f };
            image_setf(src, r, col, pixel);
        }
    }
}
void generateCloudsPerlin4(Image *src, float baseScale, float basePersistence, perlinNoise& perlin, float time) {
    int rows = src->rows;
    int cols = src->cols;

    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < cols; col++) {
            float x = (float)col / cols;
            float y = (float)r / rows;

            float noise = 0.0f;
            float scale = baseScale;
            float persistence = basePersistence;

            for (int i = 0; i < 5; i++) {
                noise += perlin.noise(x * scale, y * scale, time) * persistence;
                scale *= 2.0f;
                persistence *= 0.5f;
            }

            noise = (noise - 1.0f) / 2.0f;
            noise = pow(noise, 3.0f);
            float red, green, blue;
            if (noise > 0.2f) {
                // cloud
                red = green = blue = 0.8f;
            } else {
                // sky
                red = 0.6f + 0.2f * noise; // 淡红色
                green = 0.7f + 0.1f * noise; // 淡绿色
                blue = 0.9f + 0.1f * noise; // 浅蓝色
            }

            FPixel pixel = { { red, green, blue }, 1.0f, 1.0f };
            image_setf(src, r, col, pixel);
        }
    }
}


int main(int argc, char *argv[]) {
    const char* folder = "cloud";

    // Check if the folder exists
    if (!folder_exists(folder)) {
        return 1;
    }

    perlinNoise perlin(42); // 固定种子
    for (int i = 0; i < numOfFrames; ++i) {
        Image *srd = image_create(750, 1000);
        //generateCloudsPerlin2(srd, 5.0f, 1.0f);
        generateCloudsPerlin4(srd, 5.0f, 1.0f,perlin,(float)i*0.1);
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/cloud_%02d.ppm", folder, i);
        image_write(srd,filename);
        image_free(srd);
    }
    // Use ImageMagick to combine the PPM files into a GIF
    char command[512];
    snprintf(command, sizeof(command), "convert -delay 20 -loop 0 %s/cloud_*.ppm cloud.gif", folder);
    if (!execute_command(command)) {
        return 1;
    }

    std::cout << "GIF created successfully: cloud.gif" << std::endl;

    return 0;
}
