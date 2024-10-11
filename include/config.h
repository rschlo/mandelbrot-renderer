/**
 * @file config.h
 * @brief This file contains the configuration struct and functions to initialize and validate the configuration.
 *
 * It is used by the parser to populate the configuration settings and is accessed
 * throughout the program wherever these parameters are needed.
 *
 * @author Robert Schlosshan
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <stdint.h>

#include "complex.h"

#define MAX_NUM_COLORS 100

/**
 * Represents the configuration for the visualization as read from the configuration file.
 * The configuration includes the viewport, the maximum iteration depth, the inner color, the outer colors and the number of outer colors.
 * This includes every information about how the image will look like. Only the resolution of the image is not included here.
 */
typedef struct {
    Viewport viewport;
    size_t iteration_depth;
    uint32_t inner_color;
    size_t num_outer_colors;
    uint32_t outer_colors[MAX_NUM_COLORS];
} Configuration;

/**
 * Initializes the configuration with default values. The default values are:
 * - viewport: lower_left = (-2, -2), upper_right = (2, 2)
 * - iteration_depth: 100
 * - inner_color: 0x000000
 * - outer_colors: 0xFFFFFF, 0, 0xFFFFFF, 0xE200F2
 *
 * @param p_config A pointer to the configuration struct to initialize.
 */
int init_default_config(Configuration *p_config);

/**
 * Validates the configuration. The function checks if the configuration is valid.
 *
 * @param config The configuration to validate.
 */
int validate_config(Configuration config);

#endif  // CONFIG_H
