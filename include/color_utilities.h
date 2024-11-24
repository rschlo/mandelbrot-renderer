#ifndef COLOR_UTILITIES_H
#define COLOR_UTILITIES_H

#include <stdint.h>

#include "config.h"

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
 * Interpolates between two colors. The result is stored in p_result.
 *
 * @param start_color The start color.
 * @param end_color The end color.
 * @param t The progress between the start and end color. Must be between 0 and 1.
 * @param p_result A pointer to store the result.
 */
void interpolate_color(uint32_t start_color, uint32_t end_color, double t, uint32_t *p_result);

#endif  // COLOR_UTILITIES_H