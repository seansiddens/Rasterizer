#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "rasterizer.h"
#include "util.h"

void put_pixel(char img[], int x, int y, color *col) {

    int offset = 3 * (y * image_width + x); // Offset into image buffer

    char r = (char)(255 * col->r);
    char g = (char)(255 * col->g);
    char b = (char)(255 * col->b);

    img[offset] = r;
    img[offset + 1] = g;
    img[offset + 2] = b;
}

void draw_line(char img[], vec2 *p0, vec2 *p1, color *col) {
    float x0, y0, x1, y1;

    if (fabsf(p1->x - p0->x) > fabsf(p1->y - p0->y)) {
        // Line is horizontal-ish
        // Make sure x0 < x1
        if (p0->x > p1->x) {
            vec2 *temp = p0;
            p0 = p1;
            p1 = temp;
        }
        // Get struct members
        x0 = p0->x;
        y0 = p0->y;
        x1 = p1->x;
        y1 = p1->y;

        int y;
        for (int x = (int)x0; x <= (int)x1; x++) {
            y = (int)lerp((int)x0, y0, (int)x1, y1, x);
            put_pixel(img, x, y, col);
        }
    } else {
        // Line is vertical-ish
        // Make sure y0 > y1
        if (p0->y > p1->y) {
            vec2 *temp = p0;
            p0 = p1;
            p1 = temp;
        }

        // Get struct members
        x0 = p0->x;
        y0 = p0->y;
        x1 = p1->x;
        y1 = p1->y;

        int x;
        for (int y = (int)y0; y <= (int)y1; y++) {
            x = (int)lerp((int)y0, x0, (int)y1, x1, y);
            put_pixel(img, x, y, col);
        }
    }
}

void draw_wireframe_triangle(char img[], vec2 *p0, vec2 *p1, vec2 *p2, color *col) {
    draw_line(img, p0, p1, col);
    draw_line(img, p1, p2, col);
    draw_line(img, p2, p0, col);
}

void draw_filled_triangle(char img[], vec2 *p0, vec2 *p1, vec2 *p2, color *col) {
    // Sort the points so that y0 <= y1 <= y2
    // if y1 < y0: swap(p1, p0)
    if (p1->y < p0->y) {
        vec2 *temp = p1;
        p1 = p0;
        p0 = temp;
    }
    // if y2 < y0: swap(p2, p0)
    if (p2->y < p0->y) {
        vec2 *temp = p2;
        p2 = p0;
        p0 = temp;
    }
    // if y2 < y1: swap(p2, p1)
    if (p2->y < p1->y) {
        vec2 *temp = p2;
        p2 = p1;
        p1 = temp;
    }
    // Test to see if points are in correct order
    printf("p0: (%f, %f)\n", p0->x, p0->y);
    printf("p1: (%f, %f)\n", p1->x, p1->y);
    printf("p2: (%f, %f)\n", p2->x, p2->y);

    // draw_line(img, p0, p1, col);
    // draw_line(img, p1, p2, col);
    // draw_line(img, p2, p0, col);

    // Get struct members
    int x0 = p0->x;
    int y0 = p0->y;
    int x1 = p1->x;
    int y1 = p1->y;
    int x2 = p2->x;
    int y2 = p2->y;

    float h0 = p0->h;
    float h1 = p1->h;
    float h2 = p2->h;

    // Check which is left or right side
    int h = (int)((y1 - y0) / 2);
    float x02 = lerp(y0, x0, y2, x2, h);
    float x01 = lerp(y0, x0, y1, x1, h);

    // Shade triangle with scanlines
    // Long edge to the left of shorter edges
    color shaded_color;
    float h_left, h_right, h_seg;
    int x_left, x_right;
    if (x02 < x01) {
        // Draw scanlines from y0 to y1
        for (int y = y0; y <= y1; y++) {
            h_left = lerp(y0, h0, y2, h2, y);
            h_right = lerp(y0, h0, y1, h1, y);
            x_left = lerp(y0, x0, y2, x2, y);
            x_right = lerp(y0, x0, y1, x1, y);
            for (int x = x_left; x <= x_right; x++) {
                h_seg = lerp(x_left, h_left, x_right, h_right, x);
                shaded_color.r = col->r * h_seg;
                shaded_color.g = col->g * h_seg;
                shaded_color.b = col->b * h_seg;
                put_pixel(img, x, y, &shaded_color);
            }
        }
        // Draw scanlines from y1 to y2
        for (int y = y1 + 1; y <= y2; y++) {
            h_left = lerp(y0, h0, y2, h2, y);
            h_right = lerp(y1, h1, y2, h2, y);
            x_left = lerp(y0, x0, y2, x2, y);
            x_right = lerp(y1, x1, y2, x2, y);
            for (int x = x_left; x <= x_right; x++) {
                h_seg = lerp(x_left, h_left, x_right, h_right, x);
                shaded_color.r = col->r * h_seg;
                shaded_color.g = col->g * h_seg;
                shaded_color.b = col->b * h_seg;
                put_pixel(img, x, y, &shaded_color);
            }
        }
    } else {
        // Draw scanlines from y0 to y1
        for (int y = y0; y <= y1; y++) {
            h_left = lerp(y0, h0, y1, h1, h);
            h_right = lerp(y0, h0, y2, h2, h);
            x_left = lerp(y0, x0, y1, x1, y);
            x_right = lerp(y0, x0, y2, x2, y);
            for (int x = x_left; x <= x_right; x++) {
                h_seg = lerp(x_left, h_left, x_right, h_right, x);
                shaded_color.r = col->r;
                shaded_color.g = col->g * h_seg;
                shaded_color.b = col->b * h_seg;
                put_pixel(img, x, y, &shaded_color);
            }
        }
        // Draw scanlines from y1 to y2
        for (int y = y1 + 1; y <= y2; y++) {
            h_left = lerp(y1, h1, y2, h2, h);
            h_right = lerp(y0, h0, y2, h2, h);
            x_left = lerp(y1, x1, y2, x2, y);
            x_right = lerp(y0, x0, y2, x2, y);
            for (int x = x_left; x <= x_right; x++) {
                h_seg = lerp(x_left, h_left, x_right, h_right, x);
                shaded_color.r = col->r * h_seg;
                shaded_color.g = col->g * h_seg;
                shaded_color.b = col->b * h_seg;
                put_pixel(img, x, y, &shaded_color);
            }
        }
    }
}

// Linear interpolation from d0 = f(i0) to d1 = f(i1)
// Independent variables are always integers,
// while dependent variables are always floats.
// Returns pointer to beginning of array of interpolated vals.
float *interpolate(int i0, float d0, int i1, float d1) {
    float *values = NULL;
    if (i0 == i1) {
        *values = d0;
        return values;
    }

    // Allocate memory for storing interpolated values
    // First index has d0, last has d1
    values = (float *)malloc((abs(i1 - i0) + 1) * sizeof(float));

    float a = (d1 - d0) / (i1 - i0); // Slope of linear function
    float d = d0;
    float *ptr = values;
    for (int i = i0; i <= i1; i++) {
        *ptr = d;
        ptr++;
        d = d + a;
    }

    return values;
}
