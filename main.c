#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "rasterizer.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(int argc, char *argv[]) {

    char img[image_width * image_height * 3]; // Declare image buffer

    // Initialize image buffer to 0 values
    memset(img, 0, sizeof(img));

    color c = {255, 0, 0};
    vec2 p0 = {10, 128};
    vec2 p1 = {240, 115};
    vec2 p2 = {10, 15};

    draw_filled_triangle(img, &p0, &p1, &p2, &c);

    c.b = 255;
    put_pixel(img, p0.x, p0.y, &c);
    put_pixel(img, p1.x, p1.y, &c);
    put_pixel(img, p2.x, p2.y, &c);




    // Write out image data to file
    if (stbi_write_bmp("out.bmp", image_width, image_height, 3, img) == 0) {
        printf("Failed to write image");
    }

    return 1;
}