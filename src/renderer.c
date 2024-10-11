#include "../include/renderer.h"

#include "../include/codes.h"
#include "../include/colors.h"
#include "../include/complex.h"
#include "../include/config.h"
#include "../include/image.h"

/**
 * The progress step for the image building process.
 * If the progress is greater than the last output plus the progress step, the progress is outputted through the progress_callback function.
 */
#define PROGRESS_STEP 0.05

/**
 * Processes the progress of the image building process.
 * If the progress is greater than the last output plus the progress step, the progress is outputted.
 *
 * @param progress The progress of the image building process. Must be between 0 and 1.
 * @param p_last_output The pointer to the last output progress.
 * @param progress_callback The callback function to output the progress.
 * @return Status code.
 */
void _process_progress(double progress, double *p_last_output, void (*progress_callback)(double)) {
    if ((progress - *p_last_output < PROGRESS_STEP) && p_last_output != 0 && progress != 1) {
        return;
    }
    progress_callback(progress);
    *p_last_output = progress;
}

int build_image(ImageSize size, Configuration config, unsigned char *p_image_data, void (*progress_callback)(double)) {
    Complex c = {0, 0};
    size_t number_iterations;
    uint32_t color;

    double progress = 0.0;
    double last_progress_output = 0.0;
    _process_progress(progress, &last_progress_output, progress_callback);
    int status = SUCCESS;

    for (int x = 0; x < size.width; x++) {
        for (int y = 0; y < size.height; y++) {
            status = pixelcoord_to_complex(x, y, config.viewport, size, &c);
            if (status < 0) {
                return status;
            }
            status = iterate_squence(c, config.iteration_depth, &number_iterations);
            if (status < 0) {
                return status;
            }
            status = calculate_color(number_iterations, config, &color);
            if (status < 0) {
                return status;
            }
            status = set_pixel_in_image_data(x, y, size, color, p_image_data);
            if (status < 0) {
                return status;
            }
            // Note: Division by zero is not possible here, because size.width is always greater than 0.
            // Otherwise the program would not reach this point.
            progress = (double)x / (double)size.width;
            _process_progress(progress, &last_progress_output, progress_callback);
        }
    }

    _process_progress(1.0, &last_progress_output, progress_callback);
    return SUCCESS;
}