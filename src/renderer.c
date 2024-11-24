#include "../include/renderer.h"

#include <math.h>

#include "../include/color_utilities.h"
#include "../include/config.h"
#include "../include/image_manager.h"
#include "../include/status_manager.h"

/**
 * The progress step for the image building process.
 * If the progress is greater than the last output plus the progress step, the progress is outputted through the progress_callback function.
 */
#define PROGRESS_STEP 0.05

/**
 * The escape radius for the Mandelbrot function.
 * If the magnitude of a term of the mandelbrot sequence is greater than the escape radius, the sequence is considered to be unbounded.
 * It is proven that an escape radius of 2 is sufficient.
 */
#define ESCAPE_RADIUS 2

/**
 * Processes the progress of the image building process.
 * If the progress is greater than the previous output plus the progress step, the progress is outputted.
 *
 * @param progress The progress of the image building process. Must be between 0 and 1.
 * @param p_prev_progress The pointer to the previous output progress.
 * @param progress_callback The callback function to output the progress.
 * @return Status code.
 */
void _process_progress(double progress, double *p_prev_progress, void (*progress_callback)(double)) {
    if ((progress - *p_prev_progress < PROGRESS_STEP) && p_prev_progress != 0 && progress != 1) {
        return;
    }
    progress_callback(progress);
    *p_prev_progress = progress;
}

/**
 * Iterates the Mandelbrot function for a given complex number c.
 * z_0 = 0, z_1 = z_0^2 + c = c, z_2 = z_1^2 + c, ...
 * The function stores the number of iterations for which the Mandelbrot function remained within the ESCAPE_RADIUS in p_iterations.
 * For example, if |z_5| <= 2 but |z_6| > ESCAPE_RADIUS, p_iterations will be set to 5. Because of z_0 = 0, the minimum value for p_iterations is 0.
 * If the algorithm reaches z_{iteration_depth} and |z_{iteration_depth}| <= ESCAPE_RADIUS, p_iterations will be set to iteration_depth and the function will stop.
 *
 * @param c The complex number for which the Mandelbrot function should be iterated.
 * @param iteration_depth The maximum number of iterations. Must be greater than 0.
 * @param p_iterations A pointer to store the number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 * @return Status code.
 */
int _iteration_count(Complex c, size_t iteration_depth, size_t *p_iterations) {
    // The maximum number of iterations must be greater than 0. Otherwise the sequence would not be iterated.
    if (iteration_depth == 0) return ERROR_INVALID_ITERATION_DEPTH;

    Complex z = {0.0, 0.0};
    double magnitude_z;
    size_t iteration_count = 0;

    while (iteration_count < iteration_depth) {
        multiply(z, z, &z);
        add(z, c, &z);
        magnitude(z, &magnitude_z);
        if (magnitude_z > ESCAPE_RADIUS) {
            break;
        }
        iteration_count++;
    }

    *p_iterations = iteration_count;
    return SUCCESS;
}

/**
 * Maps the pixel coordinates (x, y) to the complex plane.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param viewport The viewport in the complex plane.
 * @param image_size The size of the image in pixels.
 * @param p_complex A pointer to store the complex number.
 * @return Status code.
 */
int _map_to_complex_number(size_t x, size_t y, Viewport viewport, ImageSize size, Complex *p_c) {
    if (size.width == 0) {
        return GENERIC_ERROR;
    }
    double viewport_width = fabs(viewport.upper_right.real - viewport.lower_left.real);
    double s = viewport_width / size.width;
    p_c->real = (double)x;
    p_c->imag = -(double)y;
    multiply_scalar(*p_c, s, p_c);
    Complex upper_left = {viewport.lower_left.real, viewport.upper_right.imag};
    add(upper_left, *p_c, p_c);
    return SUCCESS;
}

/**
 * Calculates the color for a given number of iterations.
 * The color is determined by the number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 *
 * @param num_iterations The number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 * @param config The configuration struct.
 * @param p_result A pointer to store the calculated color.
 * @return Status code.
 */
int _choose_color(size_t num_iterations, Configuration config, uint32_t *result) {
    // If the number of iterations is greater than the maximum number of iterations, return an error
    if (num_iterations > config.iteration_depth) return ERROR_INVALID_NUM_ITERATIONS;
    // If there are no outer colors, return an error
    if (config.num_outer_colors < 1) return ERROR_NO_OUTER_COLORS;
    // If the maximum number of iterations is 0, return an error
    if (config.iteration_depth == 0) return ERROR_INVALID_ITERATION_DEPTH;
    // there are not enough different values for num_iterations to map to the outer colors. We don't know which color to dismiss?
    if (config.num_outer_colors > config.iteration_depth) return ERROR_TOO_MANY_OUTER_COLORS;

    // If number of iterations equals iteration_depth, assume the point is in the Mandelbrot set and return the inner color.
    if (num_iterations == config.iteration_depth) {
        *result = config.inner_color;
        return SUCCESS;
    }
    // If number of iterations is less than iteration_depth and there is only one outer color, return the outer color
    if (num_iterations < config.iteration_depth && config.num_outer_colors == 1) {
        *result = config.outer_colors[0];
        return SUCCESS;
    }

    // Calculate segment size and segment index. The segment size is the size of the color interval in the number of iterations.
    // Example: I have 3 outer colors and iteration_depth = 4. Then num_iterations can be 0, 1, 2, 3 (4 is mapped to inner color, see clause above).
    // We have 2=num_outer_colors-1 color intervals [c1, c2], [c2, c3]. Then we map these intervals to the [0, 1.5], [1.5, 3] in the number of iterations, where 1.5 = (iteration_depth-1)/(num_outer_colors-1).
    double segment_size = (config.iteration_depth - 1) / (double)(config.num_outer_colors - 1);
    if (isnan(segment_size) || isinf(segment_size)) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    size_t segment_index = num_iterations / segment_size;
    // Calculate the progress within the segment
    double t = num_iterations / segment_size - segment_index;

    uint32_t start_color = config.outer_colors[segment_index];
    uint32_t end_color = config.outer_colors[segment_index + 1];

    interpolate_color(start_color, end_color, t, result);
    return SUCCESS;
}

/**
 * Calculates the color for a given pixel. The color is determined by the number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 * The color is then stored in p_color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param config The configuration struct.
 * @param size The size of the image in pixels.
 * @param p_color A pointer to store the calculated color.
 * @return Status code.
 */
int _get_color_for_pixel(size_t x, size_t y, Configuration config, ImageSize size, uint32_t *p_color) {
    Complex c = {0, 0};
    size_t number_iterations;
    int status = _map_to_complex_number(x, y, config.viewport, size, &c);
    if (status < 0) return status;

    status = _iteration_count(c, config.iteration_depth, &number_iterations);
    if (status < 0) return status;

    status = _choose_color(number_iterations, config, p_color);
    if (status < 0) return status;

    return SUCCESS;
}

int render_to_image(Configuration config, ImageData *p_image_data, void (*progress_callback)(double)) {
    Complex c = {0, 0};
    size_t number_iterations;
    uint32_t color;

    double progress = 0.0;
    double prev_progress = 0.0;
    _process_progress(progress, &prev_progress, progress_callback);
    int status = SUCCESS;

    for (int x = 0; x < p_image_data->size.width; x++) {
        for (int y = 0; y < p_image_data->size.height; y++) {
            status = _get_color_for_pixel(x, y, config, p_image_data->size, &color);
            if (status < 0) return status;
            status = set_pixel_in_image_data(x, y, color, p_image_data);
            if (status < 0) return status;

            // Note: Division by zero is not possible here, because size.width is always greater than 0.
            // Otherwise the program would not reach this point.
            progress = (double)x / (double)p_image_data->size.width;
            _process_progress(progress, &prev_progress, progress_callback);
        }
    }

    _process_progress(1.0, &prev_progress, progress_callback);
    return SUCCESS;
}
