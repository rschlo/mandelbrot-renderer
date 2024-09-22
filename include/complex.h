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
 * Multiplies two complex numbers a and b and stores the result in p_result.
 *
 * @param a The first complex number.
 * @param b The second complex number.
 * @param p_result A pointer to the complex number where the result should be stored.
 * @return Status code.
 */
int multiply(Complex a, Complex b, Complex *p_result);

/**
 * Multiplies a complex number a with a scalar and stores the result in p_result.
 *
 * @param a The complex number.
 * @param scalar The scalar.
 * @param result A pointer to the complex number where the result should be stored.
 * @return Status code.
 */
int multiply_scalar(Complex a, double scalar, Complex *p_result);

/**
 * Adds two complex numbers a and b and stores the result in p_result.
 *
 * @param a The first complex number.
 * @param b The second complex number.
 * @param p_result A pointer to the complex number where the result should be stored.
 * @return Status code.
 */
int add(Complex a, Complex b, Complex *p_result);

/**
 * Calculates the magnitude of a complex number c and stores the result in p_result.
 *
 * @param c The complex number.
 * @param p_result A pointer to store the result.
 * @return Status code.
 */
int magnitude(Complex c, double *p_result);

/**
 * Iterates the Mandelbrot function for a given complex number c.
 * The function returns the number of iterations needed to escape the ESCAPE_RADIUS or n_max if the number of iterations exceeds n_max.
 *
 * @param c The complex number for which the Mandelbrot function should be iterated.
 * @param n_max The maximum number of iterations.
 * @param p_iterations A pointer to store the number of iterations needed to escape the ESCAPE_RADIUS.
 * @return Status code.
 */
int iterate_squence(Complex c, size_t n_max, size_t *p_iterations);

#endif  // COMPLEX_H