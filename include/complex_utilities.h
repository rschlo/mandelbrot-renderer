#ifndef COMPLEX_UTILITIES_H
#define COMPLEX_UTILITIES_H

#include <stddef.h>

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
 */
void multiply(Complex a, Complex b, Complex *p_result);

/**
 * Multiplies a complex number a with a scalar and stores the result in p_result.
 *
 * @param a The complex number.
 * @param scalar The scalar.
 * @param result A pointer to the complex number where the result should be stored.
 */
void multiply_scalar(Complex a, double scalar, Complex *p_result);

/**
 * Adds two complex numbers a and b and stores the result in p_result.
 *
 * @param a The first complex number.
 * @param b The second complex number.
 * @param p_result A pointer to the complex number where the result should be stored.
 */
void add(Complex a, Complex b, Complex *p_result);

/**
 * Calculates the magnitude of a complex number c and stores the result in p_result.
 *
 * @param c The complex number.
 * @param p_result A pointer to store the result.
 */
void magnitude(Complex c, double *p_result);

#endif  // COMPLEX_UTILITIES_H