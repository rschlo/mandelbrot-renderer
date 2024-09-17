#include "../include/printer.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../include/bitmap.h"
#include "../include/viewport.h"
#include "../include/parser.h"

void print_info(const char *ini_file, const char *output_file, ImageSize size, Configuration p_config, double build_time) {
    printf("\n");
    printf("> output file: %s\n", output_file);
    printf("> image size: %d x %d\n", size.width, size.height);
    printf("> configurations (%s):\n", ini_file);
    printf("  - max iterations: %d\n", p_config.n_max);
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

void print_usage(const char *program_name) {
    printf("Usage: %s <ini file> <width in pixels> <output_file>\n",
           program_name);
}

void process_progress(double progress, double *p_last_output) {
    if ((progress - *p_last_output < PROGRESS_STEP) && p_last_output != 0 && progress != 1) {
        return;
    }

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
    *p_last_output = progress;
}