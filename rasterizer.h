#ifndef RASTERIZER_H
#define RASTERIZER_H

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y;
} vec2;

typedef struct {
    char r, g, b;
} color;

void put_pixel(char img[], int x, int y, color* col);

float *interpolate(int i0, float d0, int i1, float d1);

void draw_line(char img[], vec2* p0, vec2* p1, color* col);

void draw_wireframe_triangle(char img[], vec2* p0, vec2* p1, vec2* p2, color* col);

void draw_filled_triangle(char img[], vec2* p0, vec2* p1, vec2* p2, color* col);
#endif