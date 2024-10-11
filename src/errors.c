#include "../include/errors.h"

char* get_status_message(int status_code) {
    switch (status_code) {
        case SUCCESS:
            return "Success";
            break;
        case GENERIC_ERROR:
            return "Generic error";
            break;
        case ERROR_ARITHMETIC_OVERFLOW:
            return "Arithmetic overflow. Please reduce value of input parameters. ";
            break;
        case ERROR_MEMORY_ALLOC: 
            return "Memory allocation error. Please reduce image size. ";
            break;
        case ERROR_INVALID_NUM_ITERATIONS:
            return "Invalid number of iterations";
            break;
        case ERROR_INVALID_ITERATION_DEPTH:
            return "Invalid iteration depth";
            break;
        case ERROR_FILE_ACCESS:
            return "Error while accessing file";
            break;
        case ERROR_IMAGE_SIZE_0:
            return "Image size is 0. Please provide a valid image size";
            break;
        case ERROR_FILE_NOT_FOUND:
            return "Configuration file not found";
            break;
        case ERROR_INVALID_VIEWPORT:
            return "Invalid viewport. Please provide valid viewport values";
            break;
        case ERROR_INVALID_INNER_COLOR:
            return "Invalid inner color in configuration file";
            break;
        case ERROR_INVALID_OUTER_COLORS:
            return "Invalid outer colors in configuration file";
            break;
        case ERROR_NO_OUTER_COLORS:
            return "No outer colors specified in configuration file";
            break;
        case ERROR_TOO_MANY_OUTER_COLORS:
            return "Too many outer colors specified in configuration file";
            break;
        case ERROR_INVALID_IMAGE_WIDTH:
            return "Invalid image width. Please provide a valid image width";
            break;
        case ERROR_INVALID_CONFIG_KEY:
            return "Invalid key in configuration file";
            break;
        default:
            return "Generic status message";
            break;
    }
}