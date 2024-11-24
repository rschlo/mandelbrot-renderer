#include "..\include\complex_utilities.h"

#include <math.h>

void multiply(Complex a, Complex b, Complex *p_result) {
    p_result->real = a.real * b.real - a.imag * b.imag;
    p_result->imag = a.real * b.imag + a.imag * b.real;
}

void multiply_scalar(Complex a, double scalar, Complex *p_result) {
    p_result->real = a.real * scalar;
    p_result->imag = a.imag * scalar;
}

void add(Complex a, Complex b, Complex *p_result) {
    p_result->real = a.real + b.real;
    p_result->imag = a.imag + b.imag;
}

void magnitude(Complex c, double *p_result) {
    *p_result = sqrt(c.real * c.real + c.imag * c.imag);
}