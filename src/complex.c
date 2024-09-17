#include "..\include\complex.h"
#include <stdio.h>

#include <math.h>

#include "..\include\codes.h"

void multiply(Complex a, Complex b, Complex *result) {
    // TODO: Check overflow
    result->real = a.real * b.real - a.imag * b.imag;
    result->imag = a.real * b.imag + a.imag * b.real;
}

void multiply_scalar(Complex a, double scalar, Complex *result) {
    // TODO: Check overflow
    result->real = a.real * scalar;
    result->imag = a.imag * scalar;
}

void add(Complex a, Complex b, Complex *result) {
    // TODO: Check overflow
    result->real = a.real + b.real;
    result->imag = a.imag + b.imag;
}

// TODO: Check overflow
double magnitude(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}

/**
 * Note that there can't be an overflow because of the ESCAPE_RADIUS.
 */
int iterate_squence(Complex c, size_t n_max, size_t *p_iterations) {
    Complex z = {0.0, 0.0};
    for (int n = 1; n < n_max; n++) {
        multiply(z, z, &z);
        add(z, c, &z);
        if (magnitude(z) > ESCAPE_RADIUS) {
            *p_iterations = n;
            return SUCCESS;
        }
    }
    *p_iterations = n_max;
    return SUCCESS;
}