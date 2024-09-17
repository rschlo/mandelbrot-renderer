#ifndef PRINTER_H
#define PRINTER_H

#include "bitmap.h"
#include "viewport.h"
#include "parser.h"

#define PROGRESS_BAR_WIDTH 20
#define PROGRESS_STEP 0.05

/**
 * Prints information about the image building process to the console. 
 * That includes the ini file, the output file, the image size, the maximum number of iterations, the viewport, the inner color, the gradient, the gradient length and the build time.
 */
void print_info(const char *ini_file, const char *output_file, ImageSize size, Configuration config, double build_time);

/**
 * Prints how the program should be used to the console. 
 */
void print_usage(const char *program_name);

/**
 * Processes the progress of the image building process. 
 * If the progress is greater than the last output plus the progress step, the progress is printed to the console. 
 */
void process_progress(double progress, double *p_last_output);

#endif // PRINTER_H