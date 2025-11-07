#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    INVALID_ROMAN,
    INVALID_NUMBER,
    BUFFER_OVERFLOW,
    NULL_POINTER,
} status_t;

int overfprintf(FILE *stream, const char *format, ...);
int oversprintf(char *str, const char *format, ...);

status_t int_to_roman(int value, char *buffer, size_t buffer_size);
status_t uint_to_zeckendorf(unsigned int value, char *buffer, size_t buffer_size);
status_t int_to_base(int value, int base, int uppercase, char *buffer, size_t buffer_size);
status_t str_to_int_base(const char *str, int base, int uppercase, int *result);
status_t memory_dump_int(int value, char *buffer, size_t buffer_size);
status_t memory_dump_uint(unsigned int value, char *buffer, size_t buffer_size);
status_t memory_dump_double(double value, char *buffer, size_t buffer_size);
status_t memory_dump_float(float value, char *buffer, size_t buffer_size);

#endif