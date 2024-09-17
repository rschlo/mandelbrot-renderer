#include "../include/colors.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/codes.h"
#include "../include/parser.h"

uint8_t get_red(uint32_t color) {
    return (color & 0xFF0000) >> 16;  // Rot
}

uint8_t get_green(uint32_t color) {
    return (color & 0xFF00) >> 8;  // Grün
}

uint8_t get_blue(uint32_t color) {
    return (color & 0xFF);  // Blau
}

uint32_t rgb_to_uint32(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = red;
    color = (color << 8) + green;
    color = (color << 8) + blue;
    return color;
}

int calculate_color(size_t num_iterations, Configuration settings, uint32_t *result) {
    uint32_t inner_color = settings.inner_color;
    uint32_t *gradient = settings.outer_colors;
    size_t gradient_length = settings.num_outer_colors;
    size_t n_max = settings.n_max;

    // Überprüfe, ob der Index im gültigen Bereich liegt
    if (num_iterations >= n_max) {
        *result = inner_color;
        return SUCCESS;  // Rückgabe der inneren Farbe, wenn der Index ungültig ist
    }

    // Wenn es nur eine Farbe im Gradient gibt, gib diese Farbe für alle Indizes außer dem letzten zurück
    if (gradient_length == 1) {
        if (num_iterations == n_max - 1) {
            *result = inner_color;
            return SUCCESS;
        } else {
            *result = gradient[0];
            return SUCCESS;
        }
    }

    // Berechne die Anzahl der Segmente
    size_t segmentSize = n_max / gradient_length;
    size_t segmentIndex = num_iterations / segmentSize;

    if (segmentIndex >= gradient_length - 1) {
        *result = inner_color;  // Gib die innere Farbe zurück, wenn wir am Ende des Farbverlaufs sind
        return SUCCESS;
    }

    uint32_t start = gradient[segmentIndex];
    uint32_t end = gradient[segmentIndex + 1];

    // Berechne den lokalen Index innerhalb des Segments
    size_t j = num_iterations % segmentSize;

    // Berechne den Fortschritt innerhalb des Segments
    double t = (double)j / (segmentSize - 1);

    // Interpoliere die Farben
    uint8_t red = (uint8_t)(get_red(start) + t * (get_red(end) - get_red(start)));
    uint8_t green = (uint8_t)(get_green(start) + t * (get_green(end) - get_green(start)));
    uint8_t blue = (uint8_t)(get_blue(start) + t * (get_blue(end) - get_blue(start)));

    // Erstelle uint32_t Farbe aus den interpolierten RGB-Werten
    uint32_t color = rgb_to_uint32(red, green, blue);

    *result = color;
    return SUCCESS;
}