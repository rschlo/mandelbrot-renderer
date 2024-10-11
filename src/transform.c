#include "../include/transform.h"

#include <math.h>
#include <stdio.h>

int pixelcoord_to_complex(size_t x, size_t y, Viewport viewport, ImageSize size, Complex *p_c) {
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

int viewport_to_image_size(Viewport viewport, size_t image_width, ImageSize *p_image_size) {
    if (viewport.upper_right.real == viewport.lower_left.real ||
        viewport.upper_right.imag == viewport.lower_left.imag) {
        return ERROR_INVALID_VIEWPORT;
    }

    p_image_size->width = image_width;
    double viewport_width = fabs(viewport.upper_right.real - viewport.lower_left.real);
    double viewport_height = fabs(viewport.upper_right.imag - viewport.lower_left.imag);

    if (viewport_width < 1 && viewport_height > SIZE_MAX * viewport_width) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    double aspect_ratio = viewport_height / viewport_width;

    if (p_image_size->width > SIZE_MAX / aspect_ratio) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    p_image_size->height = (size_t)(image_width * aspect_ratio);

    if (p_image_size->width == 0 || p_image_size->height == 0) {
        return ERROR_IMAGE_SIZE_0;
    }

    return SUCCESS;
}