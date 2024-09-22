#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "..\include\bitmap.h"
#include "..\include\codes.h"
#include "..\include\colors.h"
#include "..\include\complex.h"
#include "..\include\parser.h"
#include "..\include\printer.h"
#include "..\include\viewport.h"

#define ERROR_WHILE_EXPORT -3
#define WARNING_IMAGE_SIZE_0 2
#define ARG_POS_CONFIG_PATH 1
#define ARG_POS_WIDTH 2
#define ARG_POS_OUTPUT_PATH 3

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
 * Mallocs memory for the image data.
 * Prints an error message if the memory allocation fails.
 * This function is free of overflow errors.
 *
 * @param size The size of the image in pixels.
 * @param p_p_image_data A pointer to the pointer where the image data should be stored.
 * @return Status code.
 */
int malloc_image_data(ImageSize size, unsigned char **p_p_image_data) {
    if (size.width == 0 || size.height == 0) {
        return WARNING_IMAGE_SIZE_0;
    }
    if (size.width > SIZE_MAX / 3 || 3 * size.width > SIZE_MAX / size.height) {
        return ERROR_OVERFLOW;
    }
    size_t malloc_size = size.width * size.height * 3;

    unsigned char *p_memory = (unsigned char *)malloc(malloc_size);
    if (p_memory == NULL) {
        return ERROR_MEM_ALLOC;
    }
    *p_p_image_data = p_memory;
    return SUCCESS;
}

/**
 * Processes the progress of the image building process.
 * If the progress is greater than the last output plus the progress step, the progress is printed to the console.
 *
 * @param progress The progress of the image building process. Must be between 0 and 1.
 * @param p_last_output The pointer to the last output progress.
 * @return Status code.
 */
void process_progress(double progress, double *p_last_output) {
    if ((progress - *p_last_output < PROGRESS_STEP) && p_last_output != 0 && progress != 1) {
        return;
    }
    print_progress_bar(progress);
    *p_last_output = progress;
}

/**
 * Builds the image data. The function iterates over all pixels and calculates
 * the color for each pixel. The color is determined by the number of iterations
 * needed to escape the ESCAPE_RADIUS. The color is then stored in the image data.
 * The memory for p_image_data must be allocated before calling this function. The function does not free the memory.
 *
 * @param size The size of the image in pixels.
 * @param config The configuration struct.
 * @param p_image_data A pointer to the image data where the colors should be stored.
 * @return Status code.
 */
int build_image_and_print_progress(ImageSize size, Configuration config, unsigned char *p_image_data) {
    Complex c = {0, 0};
    size_t number_iterations;
    uint32_t color;

    double progress = 0.0;
    double last_progress_output = 0.0;
    process_progress(progress, &last_progress_output);

    for (int x = 0; x < size.width; x++) {
        for (int y = 0; y < size.height; y++) {
            int status_pixel_conversion = pixelcoord_to_complex(x, y, config.viewport, size, &c);
            if (status_pixel_conversion < 0) {
                return ERROR;
            }
            iterate_squence(c, config.n_max, &number_iterations);
            calculate_color(number_iterations, config, &color);

            int status_set_pixel = set_pixel_in_image_data(x, y, size, color, p_image_data);
            if (status_set_pixel < 0) {
                return ERROR;
            }

            // Note: Division by zero is not possible here, because size.width is always greater than 0.
            // Otherwise the program would not reach this point.
            progress = (double)x / (double)size.width;
            process_progress(progress, &last_progress_output);
        }
    }

    process_progress(1.0, &last_progress_output);
    printf("\n");
    return SUCCESS;
}

/**
 * Generates a valid filename by appending the specified extension if it is not already present.
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
            return ERROR_MEM_ALLOC;
        }
        strcpy(*result, incomplete_path);
        strcat(*result, extension);
    } else {
        *result = malloc(incomplete_path_length + 1);
        if (*result == NULL) {
            return ERROR_MEM_ALLOC;
        }
        strcpy(*result, incomplete_path);
    }
}

/**
 * Saves the image data to a file and frees the memory.
 *
 * @param p_image_data The image data to save.
 * @param size The size of the image.
 * @param output_path The path to save the image to.
 */
int export_and_free(unsigned char *p_image_data, ImageSize size, const char *output_path) {
    int status_export = saveBMP(output_path, size, p_image_data);
    if (status_export < 0) {
        return ERROR_WHILE_EXPORT;
    }
    free(p_image_data);
    return SUCCESS;
}

/**
 * Calculates the image size based on the width and the viewport.
 * It keeps the aspect ratio and calculates the height.
 * Every possible overflow has been checked.
 *
 * @param width The width of the image in pixels
 * @param viewport The viewport of the complex plane
 * @param size The pointer to store the calculated image size
 * @return Status code
 */
int calc_image_size(size_t width, Viewport viewport, ImageSize *size) {
    if (viewport.upper_right.real == viewport.lower_left.real ||
        viewport.upper_right.imag == viewport.lower_left.imag) {
        return ERROR_INVALID_VIEWPORT;
    }

    size->width = width;
    double viewport_width = fabs(viewport.upper_right.real - viewport.lower_left.real);
    double viewport_height = fabs(viewport.upper_right.imag - viewport.lower_left.imag);

    if (viewport_width < 1 && viewport_height > SIZE_MAX * viewport_width) {
        return ERROR_OVERFLOW;
    }
    double aspect_ratio = viewport_height / viewport_width;

    if (size->width > SIZE_MAX / aspect_ratio) {
        return ERROR_OVERFLOW;
    }
    size->height = (size_t)(width * aspect_ratio);

    if (size->width == 0 || size->height == 0) {
        return WARNING_IMAGE_SIZE_0;
    }

    return SUCCESS;
}

const char EXTENSION[] = ".bmp";

/**
 * Main function of the program.
 * Parses the command line arguments, the ini file and the width of the image.
 * Calculates the image size and allocates memory for the image data.
 * Builds the image and prints the progress.
 * Exports the image and prints the info.
 */
int main(int argc, char **argv) {
    if (argc != 4) {
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
    int status_size_calc = calc_image_size(image_width, config.viewport, &size);
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
    int status_build = CPUTIME(build_image_and_print_progress(size, config, p_image_data), &build_time);
    if (status_build < 0) {
        printf("Error: Could not build image. Please reduce image width. \n");
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