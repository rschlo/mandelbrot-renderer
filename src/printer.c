#include "../include/printer.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


void print_info(const char *config_path, const char *output_path, ImageSize size, Configuration p_config, double build_time) {
    printf("\n\n");
    printf("> output file: %s\n", output_path);
    printf("> image size: %d x %d\n", size.width, size.height);
    printf("> configurations (%s):\n", config_path);
    printf("  - iteration depth: %d\n", p_config.iteration_depth);
    printf("  - lower left: %lf + (%lf)i\n", p_config.viewport.lower_left.real, p_config.viewport.lower_left.imag);
    printf("  - upper right: %lf + (%lf)i\n", p_config.viewport.upper_right.real, p_config.viewport.upper_right.imag);
    printf("  - inner color: %x\n", p_config.inner_color);
    printf("  - outer colors: ");
    for (size_t i = 0; i < p_config.num_outer_colors; i++) {
        printf("%x ", p_config.outer_colors[i]);
    }
    printf("\n");
    printf("> build information \n");
    printf("  - build time: %.6f seconds\n", build_time);
}

void print_help(const char *program_name) {
    printf("\n");
    printf("Usage: %s [OPTIONS] <config_file> <image_width> <output_file>\n", program_name);
    printf("\n");
     printf("Description:\n");
    printf("  This program generates a Mandelbrot set image based on the given configuration file.\n");
    printf("  The image is saved as a bmp file.\n");
    printf("\n");
    printf("Arguments: \n");
    printf("  <config_file>   Path to the .ini configuration file that defines viewport, colors, etc.\n");
    printf("  <image_width>   Width of the output image in pixels (height is auto-calculated to preserve aspect ratio).\n");
    printf("  <output_file>   Path to the output file (must end with .bmp).\n");
   

    /*
    printf("Format of config file:\n");
    printf("  iteration_depth = <number>\n");
     printf("  lower_left_real = <real>\n");
    printf("  lower_left_imag = <real>\n");
    printf("  upper_right_real = <real>\n");
    printf("  upper_right_imag = <real>\n");
    printf("  inner_color = <hex>\n");
    printf("  outer_colors = <hex>, <hex> ...\n");
    */
}

void print_error_message(int status) {
    printf("Error: %s\n", get_status_message(status));
}


void print_progress_bar(double progress) {
    printf("\r|");
    int i = 0;
    while (i < PROGRESS_BAR_WIDTH * progress) {
        printf("#");
        i++;
    }
    while (i < PROGRESS_BAR_WIDTH) {
        printf(" ");
        i++;
    }
    printf("| %.2f%%", progress * 100);
    fflush(stdout);
}
