#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "codes.h"
#include "complex.h"
#include "image.h"

#define ERROR_VIEWPORT_INVALID_VIEWPORT (ERROR_ILLEGAL_ARGUMENT - 300)

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
int pixelcoord_to_complex(size_t x, size_t y, Viewport viewport, ImageSize image_size, Complex *p_complex);

/**
 * Maps the viewport size to the image size. The aspect ratio is kept.
 * Calculates the image size based on the width and the viewport.
 * It keeps the aspect ratio and calculates the height.
 *
 * @param viewport The viewport of the complex plane
 * @param image_width The width of the image in pixels
 * @param p_image_size The pointer to store the calculated image size
 * @return Status code
 */
int viewport_to_image_size(Viewport viewport, size_t image_width, ImageSize *p_image_size);

#endif  // VIEWPORT_H