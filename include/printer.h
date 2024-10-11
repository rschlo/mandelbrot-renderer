#ifndef PRINTER_H
#define PRINTER_H

#include "parser.h"

#define PROGRESS_BAR_WIDTH 20
#define PROGRESS_STEP 0.05

/**
 * Prints information about the image building process to the console.
 * That includes the config path, the output path, the image size, the maximum number of iterations, the viewport, the inner color, the gradient, the gradient length and the build time.
 *
 * @param config_path The path to the configuration file.
 * @param output_path The path to the output file.
 * @param size The size of the image in pixels.
 * @param config The configuration struct.
 * @param build_time The time it took to build the image.
 */
void print_info(const char *config_path, const char *output_path, ImageSize size, Configuration config, double build_time);

/**
 * Prints a progress bar to the console. The progress bar is a horizontal bar that shows the progress of a process.
 *
 * @param progress The progress of the process. The progress must be between 0 and 1.
 */
void print_progress_bar(double progress);

/**
 * Prints the help message to the console.
 * The help message explains how to use the program and what the command line arguments are. 
 * 
 * @param program_name The name of the program.
 */
void print_help(const char *program_name);

#endif  // PRINTER_H