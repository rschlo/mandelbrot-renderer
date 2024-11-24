#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <stdint.h>

#include "config.h"

#define MAX_LINE_LENGTH 256

/**
 * Parses the ini file and extracts the values for the viewport, the maximum iteration depth, the inner color, the outer colors and the number of outer colors.
 *
 * @param path The path to the ini file.
 * @param p_config A pointer to the configuration struct to store the values.
 * @return Status code.
 */
int parse_ini_file(const char *path, Configuration *p_config);

/**
 * Parses a the image width from a string.
 *
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 * @return Status code.
 */
int parse_image_width(const char *str, size_t *p_value);

#endif  // INPUT_PARSER_H