/**
// Created by 曲辰熙 on 2024/5/27.
 Describe the basic ball geometry
**/

#include "ball.h"



vector<vector<Point>> init_vertex(int sliceX, int sliceY, Point center, float scale, float time) {
    vector<vector<Point>> vertex_set;
    for (int y = 0; y <= sliceY; y++) {
        vector<Point> sub_vertex_set;
        for (int x = 0; x <= sliceX; x++) {
            Point sb;
            float xSegment = (float)x / (float)sliceX;
            float ySegment = (float)y / (float)sliceY;
            float xPos = cos(time + xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            float yPos = cos(time + ySegment * M_PI);
            float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            sb.val[0] = xPos * scale + center.val[0];
            sb.val[1] = yPos * scale + center.val[1];
            sb.val[2] = zPos * scale + center.val[2];
            sb.val[3] = 1;
            sub_vertex_set.push_back(sb);
        }
        vertex_set.push_back(sub_vertex_set);
    }
    return vertex_set;
}

vector<Triangle> init_triangles(vector<vector<Point>> vertex_set) {
    vector<Triangle> triangle_set;
    if(vertex_set.empty())
        return triangle_set;
    int Y_SEGMENTS = vertex_set[0].size() - 1;
    int X_SEGMENTS = vertex_set.size() - 1;
    for ( int y = 0 ; y < Y_SEGMENTS; y ++) {
        for (int x = 0; x < X_SEGMENTS; x ++) {
            Point p1 = vertex_set[y][x];
            Point p2 = vertex_set[y + 1][x];
            Point p3 = vertex_set[y + 1][x + 1];
            Point p4 = vertex_set[y][x + 1];
            triangle_set.push_back({p1, p2, p3, 1});
            triangle_set.push_back({p1, p3, p4, 1});

        }
    }
    return triangle_set;
}

void draw_ball(Image* src, int sliceX, int sliceY, Point center, float scale, Color c, bool fill, float time) {
    vector<vector<Point>> vertex_set = init_vertex(sliceX, sliceY, center, scale, time);
    vector<Triangle> triangle_set = init_triangles(vertex_set);
    for (Triangle& t : triangle_set) {
        fill ? triangle_fill(&t, src, c) : triangle_draw(&t, src, c);
    }
}