
#ifndef RENDERER_H
#define RENDERER_H

#include "config.h"
#include "image.h"

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
int build_image(ImageSize size, Configuration config, unsigned char *p_image_data, void (*progress_callback)(double));

#endif  // RENDERER_H