
#include "../include/config.h"

#include "../include/errors.h"

int init_default_config(Configuration *p_config) {
    p_config->viewport.lower_left.real = -2;
    p_config->viewport.lower_left.imag = -2;
    p_config->viewport.upper_right.real = 2;
    p_config->viewport.upper_right.imag = 2;
    p_config->iteration_depth = 100;
    p_config->inner_color = 0x000000;
    p_config->num_outer_colors = 4;
    p_config->outer_colors[0] = 0xFFFFFF;
    p_config->outer_colors[1] = 0;
    p_config->outer_colors[2] = 0xFFFFFF;
    p_config->outer_colors[3] = 0xE200F2;
    return SUCCESS;
}

int validate_config(Configuration config) {
    // TODO: Implement this function
    if (config.iteration_depth == 0) {
        return GENERIC_ERROR;
    }
    if (config.num_outer_colors < 1) {
        return GENERIC_ERROR;
    }
    if (config.viewport.lower_left.real == config.viewport.upper_right.real ||
        config.viewport.lower_left.imag == config.viewport.upper_right.imag) {
        return GENERIC_ERROR;
    }
    if (config.viewport.lower_left.real > config.viewport.upper_right.real ||
        config.viewport.lower_left.imag > config.viewport.upper_right.imag) {
        return GENERIC_ERROR;
    }
    return SUCCESS;
}
