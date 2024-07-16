//
// Created by 曲辰熙 on 2024/5/27.
//

#ifndef ASSIGNMENT3_BALL_H
#define ASSIGNMENT3_BALL_H

#include <cstdio>
#include <cstdlib>
#include "Line.h"
#include "Image.h"
#include "anti-alias.h"
#include <vector>
using namespace std;


vector<vector<Point>> init_vertex(int sliceX, int sliceY, Point center, float scale, float time);
vector<Triangle> init_triangles(vector<vector<Point>> vertex_set) ;
void draw_ball(Image* src, int sliceX, int sliceY, Point center, float scale, Color c, bool fill, float time);
#endif //ASSIGNMENT3_BALL_H

