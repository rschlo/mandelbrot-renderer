#include "..\include\complex.h"

#include <math.h>
#include <stdio.h>

#include "..\include\codes.h"

int multiply(Complex a, Complex b, Complex *p_result) {
    // TODO: Check overflow?
    p_result->real = a.real * b.real - a.imag * b.imag;
    p_result->imag = a.real * b.imag + a.imag * b.real;
    return SUCCESS;
}

int multiply_scalar(Complex a, double scalar, Complex *p_result) {
    // TODO: Check overflow?
    p_result->real = a.real * scalar;
    p_result->imag = a.imag * scalar;
    return SUCCESS;
}

int add(Complex a, Complex b, Complex *p_result) {
    // TODO: Check overflow?
    p_result->real = a.real + b.real;
    p_result->imag = a.imag + b.imag;
    return SUCCESS;
}

// TODO: Check overflow?
int magnitude(Complex c, double *p_result) {
    *p_result = sqrt(c.real * c.real + c.imag * c.imag);
    return SUCCESS;
}

/**
 * Note that there can't be an overflow because of the ESCAPE_RADIUS.
 */
int iterate_squence(Complex c, size_t n_max, size_t *p_iterations) {
    Complex z = {0.0, 0.0};
    double magnitude_z;
    // n is the number of iterations for which the sequence remains within the ESCAPE_RADIUS.
    // z_{n+1} is the first term of the sequence that exceeds the ESCAPE_RADIUS.
    size_t n = 0;

    while (n < n_max) {
        multiply(z, z, &z);
        add(z, c, &z);
        magnitude(z, &magnitude_z);
        if (magnitude_z > ESCAPE_RADIUS) {
            break;
        }
        n++;
    }

    *p_iterations = n;
    return SUCCESS;
}