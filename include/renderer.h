
#ifndef RENDERER_H
#define RENDERER_H

#include "config.h"
#include "image_manager.h"

/**
 * Builds the image data. The function iterates over all pixels and calculates
 * the color for each pixel. The color is determined by the number of iterations
 * needed to escape the ESCAPE_RADIUS. The color is then stored in the image data.
 * The memory for p_image_data must be allocated before calling this function. The function does not free the memory.
 *
 * @param config The configuration struct.
 * @param p_image_data A pointer to the image data.
 * @param progress_callback A callback function to output the progress.
 * @return Status code.
 */
int render_to_image(Configuration config, ImageData* p_image_data, void (*progress_callback)(double));

#endif  // RENDERER_H