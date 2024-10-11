#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "..\include\colors.h"
#include "..\include\complex.h"
#include "..\include\errors.h"
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
            return ERROR_MEMORY_ALLOC;
        }
        strcpy(*result, incomplete_path);
        strcat(*result, extension);
    } else {
        *result = malloc(incomplete_path_length + 1);
        if (*result == NULL) {
            return ERROR_MEMORY_ALLOC;
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
    // TODO: free memory also in case of errors

    if (strcmp(argv[ARG_POS_CONFIG_PATH], "-h") == 0 || strcmp(argv[ARG_POS_CONFIG_PATH], "--help") == 0) {
        print_help(argv[0]);
        return SUCCESS;
    }

    char *config_path = argv[ARG_POS_CONFIG_PATH];
    char *str_width = argv[ARG_POS_WIDTH];
    char *incomplete_output_path = argv[ARG_POS_OUTPUT_PATH];

    int status;

    // Parse ini file
    Configuration config;
    status = parse_ini_file(config_path, &config);
    if (status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }

    // Parse width fom command line parameter
    size_t image_width;
    status = parse_image_width(str_width, &image_width);
    if (status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }

    // Calculate image size
    ImageSize size;
    status = viewport_to_image_size(config.viewport, image_width, &size);
    if (status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }

    // Allocate memory for image
    unsigned char *p_image_data;
    status = malloc_image_data(size, &p_image_data);
    if (status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }

    // Build image and print progress
    double build_time;
    status = CPUTIME(build_image(size, config, p_image_data, &print_progress_bar), &build_time);
    if (status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }

    // Export image
    char *output_path;
    status = generate_valid_path(incomplete_output_path, EXTENSION, &output_path);
    if (status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }
    status = export_and_free(p_image_data, size, output_path);
    if (status != SUCCESS || status != SUCCESS) {
        printf("%s", get_status_message(status));
        return status;
    }
    printf("\n");

    // Print info
    print_info(config_path, output_path, size, config, build_time);

    return SUCCESS;
}