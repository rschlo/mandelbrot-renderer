#include "..\include\parser.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\include\codes.h"

#define KEY_VALUE_SEPARATOR_STR "="
#define NUM_COMMENT_CHARS 2
#define COMMENT_CHARS {'#', ';'}
#define ARRAY_SEPERATOR_STR ","
#define STR_TERMINATOR '\0'
#define EQUAL 0

int parse_size_t(const char *str, size_t *p_value) {
    char *endptr;
    *p_value = (size_t)strtoull(str, &endptr, 10);
    if (*endptr != STR_TERMINATOR) {
        return ERROR;
    }
}

int parse_double(const char *str, double *p_value) {
    char *endptr;
    *p_value = strtod(str, &endptr);
    if (*endptr != STR_TERMINATOR) {
        return ERROR;
    }
    return SUCCESS;
}

int parse_hex(const char *str, uint32_t *p_value) {
    char *endptr;
    *p_value = (uint32_t)strtoul(str, &endptr, 16);
    if (*endptr != STR_TERMINATOR) {
        return ERROR;
    }
    return SUCCESS;
}

/**
 * Removes all spaces from a string.
 */
void _remove_spaces(char *str) {
    char *dest = str;
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            *dest++ = *str;
        }
        str++;
    }
    *dest = STR_TERMINATOR;  // Set the null terminator
}

int _set_value(char *key, char *value, Configuration *p_settings) {
    int status = SUCCESS;
    if (strcmp(key, "n_max") == EQUAL) {
        status = parse_size_t(value, &p_settings->n_max);
    } else if (strcmp(key, "lower_left_real") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.lower_left.real);
    } else if (strcmp(key, "lower_left_imag") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.lower_left.imag);
    } else if (strcmp(key, "upper_right_real") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.upper_right.real);
    } else if (strcmp(key, "upper_right_imag") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.upper_right.imag);
    } else if (strcmp(key, "inner_color") == EQUAL) {
        status = parse_hex(value, &p_settings->inner_color);
    } else if (strcmp(key, "outer_colors") == EQUAL) {
        // Teile die Werte durch Kommas
        char *token = strtok(value, ARRAY_SEPERATOR_STR);
        size_t index = 0;
        while (status == SUCCESS && token != NULL && index < MAX_ARRAY_SIZE) {
            status = parse_hex(token, &p_settings->outer_colors[index]);
            token = strtok(NULL, ARRAY_SEPERATOR_STR);
            index++;
        }
        p_settings->num_outer_colors = index;  // Setze die tatsächliche Größe des Arrays
    }
    if (status < 0) {
        return ERROR;
    }
}

bool _is_comment_line(char *line) {
    int comment_chars[NUM_COMMENT_CHARS] = COMMENT_CHARS;
    for (int i = 0; i < NUM_COMMENT_CHARS; i++) {
        if (line[0] == comment_chars[i]) {
            return true;
        }
    }
    return false;
}

int parse_ini_file(const char *filename, Configuration *p_config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        // Remove newline characters
        line[strcspn(line, "\r\n")] = 0;
        _remove_spaces(line);

        if (_is_comment_line(line)) {
            // Comment line, skip
            continue;
        }

        // strchr expects a char but strtok expects a *char (string).
        if (strchr(line, KEY_VALUE_SEPARATOR_STR[0])) {
            // Seperate the line into key and value
            char *key = strtok(line, KEY_VALUE_SEPARATOR_STR);
            char *value = strtok(NULL, KEY_VALUE_SEPARATOR_STR);

            if (key && value) {
                int status = _set_value(key, value, p_config);
                if (status < 0) {
                    return ERROR;
                }
            }
        }
    }

    fclose(file);
    return SUCCESS;
}