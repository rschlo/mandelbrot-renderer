#include "..\include\parser.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_VALUE_SEPARATOR_STR "="
#define NUM_COMMENT_CHARS 2
#define COMMENT_CHARS {'#', ';'}
#define ARRAY_SEPARATOR_STR ","
#define STR_TERMINATOR '\0'
#define EQUAL 0
// Note that this error code is only for internal use. It will not be returned to by any function defined in the header file.
#define ERROR_PARSING -1

/**
 * Parses a string to a size_t value. 
 * 
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 */
int parse_size_t(const char *str, size_t *p_value) {
    char *endptr;
    *p_value = (size_t)strtoull(str, &endptr, 10);
    if (*endptr != STR_TERMINATOR) {
        return ERROR_PARSING;
    }
}

/**
 * Parses a string to a double value.
 * 
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 */
int parse_double(const char *str, double *p_value) {
    char *endptr;
    *p_value = strtod(str, &endptr);
    if (*endptr != STR_TERMINATOR) {
        return ERROR_PARSING;
    }
    return SUCCESS;
}

/**
 * Parses a string as a hexadecimal int value.
 * 
 * @param str The string to parse.
 * @param p_value The pointer to store the parsed value.
 */
int parse_hex(const char *str, uint32_t *p_value) {
    char *endptr;
    *p_value = (uint32_t)strtoul(str, &endptr, 16);
    if (*endptr != STR_TERMINATOR) {
        return ERROR_PARSING;
    }
    return SUCCESS;
}

int parse_image_width(const char *str, size_t *p_value) {
    int status = parse_size_t(str, p_value);
    if (status != SUCCESS || *p_value == 0) {
        return ERROR_INVALID_IMAGE_WIDTH;
    }
    return SUCCESS;
}

/**
 * Removes all spaces from a string. The function modifies the input string.
 *
 * @param str The string from which the spaces should be removed.
 * @return Status code.
 */
int _remove_spaces(char *str) {
    char *dest = str;
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            *dest++ = *str;
        }
        str++;
    }
    *dest = STR_TERMINATOR;  // Set the null terminator
    return SUCCESS;
}

/**
 * Sets the value for the given key in the configuration struct. Returns an error code if the key is unknown or the value is invalid.
 *
 * @param key The key for which the value should be set.
 * @param value The value to set as string.
 * @param p_settings Pointer to the configuration struct that should be modified.
 * @return Status code.
 */
int _set_value(char *key, char *value, Configuration *p_settings) {
    int status = SUCCESS;
    if (strcmp(key, "iteration_depth") == EQUAL) {
        status = parse_size_t(value, &p_settings->iteration_depth);
        if (status != SUCCESS) {
            return ERROR_INVALID_ITERATION_DEPTH;
        }
    } else if (strcmp(key, "lower_left_real") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.lower_left.real);
        if (status != SUCCESS) {
            return ERROR_INVALID_VIEWPORT;
        }
    } else if (strcmp(key, "lower_left_imag") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.lower_left.imag);
        if (status != SUCCESS) {
            return ERROR_INVALID_VIEWPORT;
        }
    } else if (strcmp(key, "upper_right_real") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.upper_right.real);
        if (status != SUCCESS) {
            return ERROR_INVALID_VIEWPORT;
        }
    } else if (strcmp(key, "upper_right_imag") == EQUAL) {
        status = parse_double(value, &p_settings->viewport.upper_right.imag);
        if (status != SUCCESS) {
            return ERROR_INVALID_VIEWPORT;
        }
    } else if (strcmp(key, "inner_color") == EQUAL) {
        status = parse_hex(value, &p_settings->inner_color);
        if (status != SUCCESS) {
            return ERROR_INVALID_INNER_COLOR;
        }
    } else if (strcmp(key, "outer_colors") == EQUAL) {
        // Parse the outer colors as an array
        char *token = strtok(value, ARRAY_SEPARATOR_STR);
        size_t index = 0;
        while (status == SUCCESS && token != NULL && index < MAX_NUM_COLORS) {
            status = parse_hex(token, &p_settings->outer_colors[index]);
            token = strtok(NULL, ARRAY_SEPARATOR_STR);
            index++;
        }
        if (status != SUCCESS) {
            return ERROR_INVALID_OUTER_COLORS;
        }
        p_settings->num_outer_colors = index;  // Set the actual number of outer colors
    } else {
        return ERROR_INVALID_CONFIG_KEY;
    }
}

/**
 * Checks if the given line is a comment line. A comment line starts with a char from the COMMENT_CHARS array.
 *
 * @param line The line to check.
 * @return True if the line is a comment line, false otherwise.
 */
bool _is_comment_line(char *line) {
    int comment_chars[NUM_COMMENT_CHARS] = COMMENT_CHARS;
    for (int i = 0; i < NUM_COMMENT_CHARS; i++) {
        if (line[0] == comment_chars[i]) {
            return true;
        }
    }
    return false;
}

int parse_ini_file(const char *path, Configuration *p_config) {
    FILE *file = fopen(path, "r");
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

        // Note that strchr expects a char but strtok expects a *char (string).
        if (strchr(line, KEY_VALUE_SEPARATOR_STR[0])) {
            // Seperate the line into key and value
            char *key = strtok(line, KEY_VALUE_SEPARATOR_STR);
            char *value = strtok(NULL, KEY_VALUE_SEPARATOR_STR);

            if (key && value) {
                int status = _set_value(key, value, p_config);
                if (status < 0) return status;
            }
        }
    }

    fclose(file);
    return SUCCESS;
}