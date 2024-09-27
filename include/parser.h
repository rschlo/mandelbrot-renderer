#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#include "transform.h"

#define MAX_LINE_LENGTH 256
#define MAX_ARRAY_SIZE 100
#define MAX_NUM_COLORS 100

#define ERROR_PARSER_FILE_NOT_FOUND (ERROR_IO - 200)

/**
 * Represents the configuration for the visualization as read from the configuration file.
 * The configuration includes the viewport, the maximum iteration depth, the inner color, the outer colors and the number of outer colors.
 */
typedef struct {
    Viewport viewport;
    size_t n_max;
    uint32_t inner_color;
    size_t num_outer_colors;
    uint32_t outer_colors[MAX_NUM_COLORS];
} Configuration;

/**
 * Parses the ini file and extracts the values for the viewport, the maximum iteration depth, the inner color, the outer colors and the number of outer colors.
 *
 * The ini file should have the following format:
 *
 * # Area of the complex plane given by two numbers 'lower_left' and 'upper_right
 * lower_left_real = -2
 * lower_left_imag = -2
 * upper_right_real = 2
 * upper_right_imag = 2
 * # Maximum amount of iterations (iteration depth)
 * n_max = 100
 * # Color (hexadecimal representation) for points inside of the mandelbrot set
 * inner_color = 0x000000
 * # Color gradient (hexadecimal representation) for points outside of the mandelbrot set
 * # > First color: discarded after 1 iteration
 * # > Last color: discarded after n_max-1 iteration
 * # At least one color must be specified
 * outer_colors = 0xFFFFFF, 0, 0xFFFFFF, 0xE200F2
 *
 *
 * @param path The path to the ini file.
 * @param p_config The configuration struct to store the values.
 */
int parse_ini_file(const char *path, Configuration *p_config);

/**
 * Parses a size_t value from a string.
 *
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 */
int parse_size_t(const char *str, size_t *p_value);

/**
 * Parses a double value from a string.
 *
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 * @return Status code.
 */
int parse_double(const char *str, double *p_value);

/**
 * Parses a hex value from a string. The hex value is stored as a uint32_t.
 *
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 * @return Status code.
 */
int parse_hex(const char *str, uint32_t *p_value);

#endif  // PARSER_H