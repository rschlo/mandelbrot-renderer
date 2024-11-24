#include "..\include\color_utilities.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t get_red(uint32_t color) {
    return (color & 0xFF0000) >> 16;
}

uint8_t get_green(uint32_t color) {
    return (color & 0xFF00) >> 8;
}

uint8_t get_blue(uint32_t color) {
    return (color & 0xFF);
}

uint32_t rgb_to_uint32(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = red;
    color = (color << 8) + green;
    color = (color << 8) + blue;
    return color;
}

void interpolate_color(uint32_t start_color, uint32_t end_color, double t, uint32_t *p_result) {
    uint8_t red = (uint8_t)(get_red(start_color) + t * (get_red(end_color) - get_red(start_color)));
    uint8_t green = (uint8_t)(get_green(start_color) + t * (get_green(end_color) - get_green(start_color)));
    uint8_t blue = (uint8_t)(get_blue(start_color) + t * (get_blue(end_color) - get_blue(start_color)));
    *p_result = rgb_to_uint32(red, green, blue);
}