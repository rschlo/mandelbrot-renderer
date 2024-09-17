#include "..\include\viewport.h"

#include <math.h>
#include <stdio.h>

#include "..\include\bitmap.h"
#include "..\include\codes.h"

int pixelcoord_to_complex(size_t x, size_t y, Viewport viewport, ImageSize size, Complex *p_c) {
    if (size.width == 0) {
        return ERROR;
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