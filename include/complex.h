#ifndef COMPLEX_H
#define COMPLEX_H

#include <stddef.h>

#define ESCAPE_RADIUS 2


/**
 * Represents a complex number. 
 */
typedef struct
{
    double real;
    double imag;
} Complex;


/**
 * Multiplies two complex numbers a and b and stores the result in result. 
 */
void multiply(Complex a, Complex b, Complex *result);

/**
 * Multiplies a complex number a with a scalar and stores the result in result. 
 */
void multiply_scalar(Complex a, double scalar, Complex *result);

/**
 * Adds two complex numbers a and b and stores the result in result. 
 */
void add(Complex a, Complex b, Complex *result);

/**
 * Returns the magnitude of the complex number c. 
 */
double magnitude(Complex c);

/**
 * Iterates the Mandelbrot function for a given complex number c.
 * The function returns the number of iterations needed to escape the ESCAPE_RADIUS or n_max if the number of iterations exceeds n_max.
 */
int iterate_squence(Complex c, size_t n_max, size_t *p_iterations);

#endif // COMPLEX_H