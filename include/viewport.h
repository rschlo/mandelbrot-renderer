#include "bitmap.h"
#include "complex.h"

#ifndef VIEWPORT_H
#define VIEWPORT_H

#define ERROR_INVALID_VIEWPORT -4

/**
 * Represents a viewport in the complex plane. Both corners are complex numbers.
 */
typedef struct
{
    Complex lower_left;
    Complex upper_right;
} Viewport;

/**
 * Maps the pixel coordinates (x, y) to the complex plane.
 */
int pixelcoord_to_complex(size_t x, size_t y, Viewport viewport, ImageSize size, Complex *p_c);

#endif  // VIEWPORT_H