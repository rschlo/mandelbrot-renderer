#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>
#include "parser.h"

/**
 * Extracts the red part of a color stored as a uint32_t.
 */
uint8_t get_red(uint32_t color);

/**
 * Extracts the green part of a color stored as a uint32_t.
 */
uint8_t get_green(uint32_t color);

/**
 * Extracts the blue part of a color stored as a uint32_t.
 */
uint8_t get_blue(uint32_t color);

/**
 * Converts the RGB values to a uint32_t color.
 */
uint32_t rgb_to_uint32(uint8_t red, uint8_t green, uint8_t blue);

/**
 * Calculates the color for a given number of iterations.
 */
int calculate_color(size_t num_iterations, Configuration config, uint32_t *result);

#endif  // COMPLEX_H