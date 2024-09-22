#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

#include "parser.h"

#define ERROR_NO_OUTER_COLORS -1
#define ERROR_TOO_MANY_OUTER_COLORS -4
#define ERROR_INVALID_NUM_ITERATIONS -2
#define ERROR_INVALID_N_MAX -3

/**
 * Extracts the red part of a color stored as a uint32_t.
 *
 * @param color The color to extract the red part from.
 * @return The red part of the color.
 */
uint8_t get_red(uint32_t color);

/**
 * Extracts the green part of a color stored as a uint32_t.
 *
 * @param color The color to extract the green part from.
 * @return The green part of the color.
 */
uint8_t get_green(uint32_t color);

/**
 * Extracts the blue part of a color stored as a uint32_t.
 *
 * @param color The color to extract the blue part from.
 * @return The blue part of the color.
 */
uint8_t get_blue(uint32_t color);

/**
 * Converts the RGB values to a uint32_t color.
 *
 * @param red The red part of the color.
 * @param green The green part of the color.
 * @param blue The blue part of the color.
 * @return The color as a uint32_t.
 */
uint32_t rgb_to_uint32(uint8_t red, uint8_t green, uint8_t blue);

/**
 * Calculates the color for a given number of iterations.
 * The color is determined by the number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 *
 * @param num_iterations The number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 * @param config The configuration struct.
 * @param p_result A pointer to store the calculated color.
 * @return Status code.
 */
int calculate_color(size_t num_iterations, Configuration config, uint32_t *p_result);

#endif  // COMPLEX_H