#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#include "viewport.h"

#define MAX_LINE_LENGTH 256
#define MAX_ARRAY_SIZE 100
#define ERROR_FILE_NOT_FOUND -1
#define MAX_NUM_COLORS 100

/**
 * Represents the configuration for the visualization as read from the configuration file.
 */
typedef struct {
    Viewport viewport;
    size_t n_max;
    uint32_t inner_color;
    size_t num_outer_colors;
    uint32_t outer_colors[MAX_NUM_COLORS];
} Configuration;

/**
 * Parses the ini file and extracts the values for the viewport, the maximum iteration depth, the inner color, the gradient and the gradient size.
 */
int parse_ini_file(const char *filename, Configuration *p_config);

/**
 * Parses a size_t value from a string.
 */
int parse_size_t(const char *str, size_t *p_value);

#endif  // PARSER_H