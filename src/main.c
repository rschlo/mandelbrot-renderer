#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "..\include\codes.h"
#include "..\include\colors.h"
#include "..\include\complex.h"
#include "..\include\image.h"
#include "..\include\parser.h"
#include "..\include\printer.h"
#include "..\include\renderer.h"
#include "..\include\transform.h"

#define ARG_POS_CONFIG_PATH 1
#define ARG_POS_WIDTH 2
#define ARG_POS_OUTPUT_PATH 3
#define EXPECTED_ARG_COUNT 4
#define EXTENSION ".bmp"

// This is a macro to measure the time of a function call.
// It returns the return value of the function call. The time is stored in the variable TIME_PTR.
#define CPUTIME(FCALL, TIME_PTR)                                  \
    ({                                                            \
        double START = clock();                                   \
        int return_value = FCALL;                                 \
        *(TIME_PTR) = ((double)clock() - START) / CLOCKS_PER_SEC; \
        return_value;                                             \
    })

/**
 * Generates a valid path by appending the specified extension if it is not already present.
 *
 * @param incomplete_path The incomplete path to append the extension to, if necessary.
 * @param extension The extension to append.
 * @param result The pointer to store the result.
 */
int generate_valid_path(char *incomplete_path, const char *extension, char **result) {
    size_t incomplete_path_length = strlen(incomplete_path);
    size_t extension_length = strlen(extension);

    if (strlen(incomplete_path) < extension_length || strcmp(incomplete_path + incomplete_path_length - extension_length, extension) != 0) {
        *result = malloc(incomplete_path_length + extension_length + 1);
        if (*result == NULL) {
            return ERROR_MEMORY;
        }
        strcpy(*result, incomplete_path);
        strcat(*result, extension);
    } else {
        *result = malloc(incomplete_path_length + 1);
        if (*result == NULL) {
            return ERROR_MEMORY;
        }
        strcpy(*result, incomplete_path);
    }

    return SUCCESS;
}

/**
 * Main function of the program.
 * Parses the command line arguments, the ini file and the width of the image.
 * Calculates the image size and allocates memory for the image data.
 * Builds the image and prints the progress.
 * Exports the image and prints the info.
 */
int main(int argc, char **argv) {
    if (argc != EXPECTED_ARG_COUNT) {
        print_usage(argv[0]);
        return ERROR;
    }

    char *config_path = argv[ARG_POS_CONFIG_PATH];
    char *str_width = argv[ARG_POS_WIDTH];
    char *incomplete_output_path = argv[ARG_POS_OUTPUT_PATH];

    // Parse ini file
    Configuration config;
    int status_parsing = parse_ini_file(config_path, &config);
    if (status_parsing < 0) {
        printf("Error: Could not parse ini file\n");
        return ERROR;
    }

    // Parse width fom command line parameter
    size_t image_width;
    int status_width_parse = parse_size_t(str_width, &image_width);
    if (status_width_parse < 0) {
        printf("Error: Could not parse width\n");
        return ERROR;
    }

    // Calculate image size
    ImageSize size;
    int status_size_calc = viewport_to_image_size(config.viewport, image_width, &size);
    if (status_size_calc < 0) {
        printf("Error: Could not calculate image size\n");
        return ERROR;
    }

    // Allocate memory for image
    unsigned char *p_image_data;
    int status_image_malloc = malloc_image_data(size, &p_image_data);
    if (status_image_malloc < 0) {
        printf("Error: Could not allocate memory for image. Please reduce image width. \n");
        return ERROR;
    }

    // Build image and print progress
    double build_time;
    int status_build = CPUTIME(build_image(size, config, p_image_data, &print_progress_bar), &build_time);
    if (status_build < 0) {
        printf("Error: Could not build image. \n");
        return ERROR;
    }

    // Export image
    char *output_path;
    int status_filename_generation = generate_valid_path(incomplete_output_path, EXTENSION, &output_path);
    int status_export = export_and_free(p_image_data, size, output_path);
    if (status_filename_generation < 0 || status_export < 0) {
        printf("Error: Could not save image\n");
        return ERROR;
    }
    printf("Image saved at \"%s\"\n", output_path);

    // Print info
    print_info(config_path, output_path, size, config, build_time);

    return SUCCESS;
}