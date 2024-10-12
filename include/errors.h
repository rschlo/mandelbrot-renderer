#ifndef CODES_H
#define CODES_H


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

#define TRY(x) do { int _status = (x); if (_status < 0) return _status; } while (0)

char* get_status_message(int status_code);

#endif // CODES_H