/**
 * @file complex.h
 * @brief This file contains functions to work with complex numbers.
 *
 * It includes functions to multiply, add and calculate the magnitude of complex numbers.
 * The file also includes a function to iterate the Mandelbrot function for a given complex number.
 *
 * @author Robert Schlosshan
 */

#ifndef COMPLEX_H
#define COMPLEX_H

#include <stddef.h>

#define ESCAPE_RADIUS 2
#define ERROR_ITERATE_N_MAX_0 (ERROR_ILLEGAL_ARGUMENT - 500)

/**
 * Represents a complex number.
 */
typedef struct
{
    double real;
    double imag;
} Complex;

/**
 * Represents a viewport in the complex plane. Both corners are complex numbers.
 */
typedef struct
{
    Complex lower_left;
    Complex upper_right;
} Viewport;

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
 * z_0 = 0, z_1 = z_0^2 + c = c, z_2 = z_1^2 + c, ...
 * The function stores the number of iterations for which the Mandelbrot function remained within the ESCAPE_RADIUS in p_iterations.
 * For example, if |z_5| <= 2 but |z_6| > ESCAPE_RADIUS, p_iterations will be set to 5. Because of z_0 = 0, the minimum value for p_iterations is 0.
 * If the algorithm reaches z_{n_max} and |z_{n_max}| <= ESCAPE_RADIUS, p_iterations will be set to n_max and the function will stop.
 *
 * @param c The complex number for which the Mandelbrot function should be iterated.
 * @param n_max The maximum number of iterations. Must be greater than 0.
 * @param p_iterations A pointer to store the number of iterations for which the mandelbrot function remained within the ESCAPE_RADIUS.
 * @return Status code.
 */
int iterate_squence(Complex c, size_t n_max, size_t *p_iterations);

#endif  // COMPLEX_H