/**
 * Created by 曲辰熙 on 2024/5/26.
 * I'm trying to encapsulate all the anti-alias algorithms and plan to implement e.g. MMSA SSAA... in it, encapsulating
 * these functions I think will significantly reduce the speed of my algorithms, but I want to do it as a learning perspective.

**/

#include "Image.h"
#include "anti-alias.h"

using namespace std;


void SSAA(Image* src, int sample_rate){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(-1.0, 1.0);
    for(int c = 0; c < src->cols ; c++){
        for(int r = 0; r < src->rows ; r++){
            Color val;
            val.c[0] = 0;
            val.c[1] = 0;
            val.c[2] = 0;
            for (int a = 0; a < sample_rate; a++) {
                int bias1 = (distribution(gen)) > 0 ? -1 : 1;
                int bias2 = (distribution(gen)) > 0 ? -1 : 1;
                int sampleX = c + bias1;
                int sampleY = r + bias2;

                // bound
                if (sampleX >= 0 && sampleX < src->cols && sampleY >= 0 && sampleY < src->rows) {
                    val.c[0] += src->data[sampleY * src->cols + sampleX].rgb[0];
                    val.c[1] += src->data[sampleY * src->cols + sampleX].rgb[1];
                    val.c[2] += src->data[sampleY * src->cols + sampleX].rgb[2];
                }
            }
            src->data[r * src->cols + c].rgb[0] = (val.c[0] /= (float)sample_rate);
            src->data[r * src->cols + c].rgb[1] = (val.c[1] /= (float)sample_rate);
            src->data[r * src->cols + c].rgb[2] = (val.c[2] /= (float)sample_rate);

        }
    }
}