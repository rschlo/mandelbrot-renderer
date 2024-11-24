#ifndef STATUS_MANAGER_H
#define STATUS_MANAGER_H

#define SUCCESS 0
#define GENERIC_ERROR -1

#define ERROR_ARITHMETIC_OVERFLOW -2
#define ERROR_MEMORY_ALLOC -3
#define ERROR_INVALID_NUM_ITERATIONS -6
#define ERROR_INVALID_ITERATION_DEPTH -7
#define ERROR_FILE_ACCESS -8
#define ERROR_IMAGE_SIZE_0 -9
#define ERROR_FILE_NOT_FOUND -10
#define ERROR_INVALID_CONFIG_KEY -13

#define ERROR_INVALID_VIEWPORT -11

#define ERROR_INVALID_INNER_COLOR -14
#define ERROR_INVALID_OUTER_COLORS -15
#define ERROR_NO_OUTER_COLORS -4
#define ERROR_TOO_MANY_OUTER_COLORS -5

#define ERROR_INVALID_IMAGE_WIDTH -16
#define ERROR_INVALID_NUM_CL_ARG -17

/**
 * Returns the status message for a given status code.
 *
 * @param status_code The status code for which the message should be returned.
 * @return The status message.
 */
char* get_status_message(int status_code);

#endif  // STATUS_MANAGER_H