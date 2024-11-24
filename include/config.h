#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <stdint.h>

#include "complex_utilities.h"

#define MAX_NUM_COLORS 100

/**
 * Represents a viewport in the complex plane. Both corners are complex numbers.
 */
typedef struct {
    Complex lower_left;
    Complex upper_right;
} Viewport;

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

#endif  // CONFIG_H
