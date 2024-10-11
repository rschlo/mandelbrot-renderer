#include "../include/colors.h"

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

/**
 * Interpolates between two colors. The result is stored in p_result.
 *
 * @param start_color The start color.
 * @param end_color The end color.
 * @param t The progress between the start and end color. Must be between 0 and 1.
 * @param p_result A pointer to store the result.
 */
void _interpolate(uint32_t start_color, uint32_t end_color, double t, uint32_t *p_result) {
    uint8_t red = (uint8_t)(get_red(start_color) + t * (get_red(end_color) - get_red(start_color)));
    uint8_t green = (uint8_t)(get_green(start_color) + t * (get_green(end_color) - get_green(start_color)));
    uint8_t blue = (uint8_t)(get_blue(start_color) + t * (get_blue(end_color) - get_blue(start_color)));
    *p_result = rgb_to_uint32(red, green, blue);
}

int calculate_color(size_t num_iterations, Configuration settings, uint32_t *result) {
    uint32_t inner_color = settings.inner_color;
    uint32_t *outer_colors = settings.outer_colors;
    size_t num_outer_colors = settings.num_outer_colors;
    size_t iteration_depth = settings.iteration_depth;

    // If the number of iterations is greater than the maximum number of iterations, return an error
    if (num_iterations > iteration_depth) {
        return ERROR_COLORS_INVALID_NUM_ITERATIONS;
    }
    // If there are no outer colors, return an error
    if (num_outer_colors < 1) {
        return ERROR_COLORS_NO_OUTER_COLORS;
    }
    // If the maximum number of iterations is 0, return an error
    if (iteration_depth == 0) {
        return ERROR_COLORS_INVALID_N_MAX;
    }
    // there are not enough different values for num_iterations to map to the outer colors. We don't know which color to dismiss?
    if (num_outer_colors > iteration_depth) {
        return ERROR_COLORS_TOO_MANY_OUTER_COLORS;
    }

    // If number of iterations equals iteration_depth, assume the point is in the Mandelbrot set and return the inner color.
    if (num_iterations == iteration_depth) {
        *result = inner_color;
        return SUCCESS;
    }
    // If number of iterations is less than iteration_depth and there is only one outer color, return the outer color
    if (num_iterations < iteration_depth && num_outer_colors == 1) {
        *result = outer_colors[0];
        return SUCCESS;
    }

    // Calculate segment size and segment index. The segment size is the size of the color interval in the number of iterations.
    // Example: I have 3 outer colors and iteration_depth = 4. Then num_iterions can be 0, 1, 2, 3 (4 is mapped to inner color, see clause above).
    // We have 2=num_outer_colors-1 color intervals [c1, c2], [c2, c3]. Then we map these intervals to the [0, 1.5], [1.5, 3] in the number of iterations, where 1.5 = (iteration_depth-1)/(num_outer_colors-1).
    double segment_size = (iteration_depth - 1) / (double)(num_outer_colors - 1);
    if (isnan(segment_size) || isinf(segment_size)) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    size_t segment_index = num_iterations / segment_size;
    // Calculate the progress within the segment
    double t = num_iterations / segment_size - segment_index;

    uint32_t start_color = outer_colors[segment_index];
    uint32_t end_color = outer_colors[segment_index + 1];

    _interpolate(start_color, end_color, t, result);
    return SUCCESS;
}