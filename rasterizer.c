#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rasterizer.h"
#include "main.h"
#include "util.h"


void put_pixel(char img[], int x, int y, color* col) {

    int offset = 3 * (y * image_width + x); // Offset into image buffer

    img[offset] = col->r;
    img[offset+1] = col->g;
    img[offset+2] = col->b;
}


void draw_line(char img[], vec2* p0, vec2* p1, color* col) {
    float x0, y0, x1, y1;

    if (abs(p1->x - p0->x) > abs(p1->y - p0->y)){
        // Line is horizontal-ish
        // Make sure x0 < x1
        if (p0->x > p1->x) {
            vec2* temp = p0;
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
    }
    else {
        // Line is vertical-ish
        // Make sure y0 > y1
        if (p0->y > p1->y) {
            vec2* temp = p0;
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

void draw_wireframe_triangle(char img[], vec2* p0, vec2* p1, vec2* p2, color* col) {
    draw_line(img, p0, p1, col);
    draw_line(img, p1, p2, col);
    draw_line(img, p2, p0, col);
}

void draw_filled_triangle(char img[], vec2* p0, vec2* p1, vec2* p2, color* col) {
    // Sort the points so that y0 <= y1 <= y2
    // if y1 < y0: swap(p1, p0)
    if (p1->y < p0->y) {
        vec2* temp = p1;
        p1 = p0;
        p0 = temp;
    }
    // if y2 < y0: swap(p2, p0)
    if (p2->y < p0->y) {
        vec2* temp = p2;
        p2 = p0;
        p0 = temp;
    }
    // if y2 < y1: swap(p2, p1) 
    if (p2->y < p1->y) {
        vec2* temp = p2;
        p2 = p1;
        p1 = temp;
    }
    // Test to see if points are in correct order
    printf("p0: (%f, %f)\n", p0->x, p0->y);
    printf("p1: (%f, %f)\n", p1->x, p1->y);
    printf("p2: (%f, %f)\n", p2->x, p2->y);

    // Get struct members
    int x0 = p0->x;
    int y0 = p0->y;
    int x1 = p1->x;
    int y1 = p1->y;
    int x2 = p2->x;
    int y2 = p2->y;

    // Compute the x coordinates of the triangle edges
    float* x01 = interpolate(y0, x0, y1, x1);
    float* x12 = interpolate(y1, x1, y2, x2);
    float* x02 = interpolate(y0, x0, y2, x2);

    // Concantenate the short sides
    // Allocate memory for storing interpolated values
    // float* x012 = (float*)malloc((y2-y0+1) * sizeof(float)); 
    // for (int y = 0; y <= y2; y++) {
    //     if (y < y1) {
    //         *(x012 + y-y0) = *(x01 + y-y0);
    //     }
    //     else {
    //         *(x012 + y-y0) = *(x12 + y-y0);
    //     }
    // }

    // Determine which is left and which is right
    // float* x_left;
    // float* x_right;
    // int m = (int)((y2 - y0) / 2);
    // if ((x02+m) < (x01+m)) {
    //     x_left = x02;
    //     x_right = x012;
    // }
    // else {
    //     x_left = x012;
    //     x_right = x02;
    // }

    // // Draw the horizontal segments
    // for (int y = y0; y <= y2; y++) {
    //     for (int x = (int)*(x_left + y-y0); x <= (int)*(x_right + y-y0); x++) {
    //         put_pixel(img, x, y, col);
    //     }
    // }




    // free(x_left);
    // free(x_right);
    free(x01);
    free(x12);
}

// Linear interpolation from d0 = f(i0) to d1 = f(i1)
// Independent variables are always integers, 
// while dependent variables are always floats.
// Returns pointer to beginning of array of interpolated vals. 
float *interpolate(int i0, float d0, int i1, float d1) {
    float *values;
    if (i0 == i1) {
        *values = d0;
        return values;
    }

    // Allocate memory for storing interpolated values
    // First index has d0, last has d1
    values = (float*)malloc((abs(i1-i0) + 1) * sizeof(float)); 

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