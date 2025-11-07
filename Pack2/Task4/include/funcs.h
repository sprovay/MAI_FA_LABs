#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    INVALID_ROMAN,
    INVALID_ZEKKENDORF,
    INVALID_NUMBER,
    NULL_POINTER,
} status_t;

int overfscanf(FILE *stream, const char *format, ...);
int oversscanf(const char *str, const char *format, ...);

status_t roman_to_int(const char *str, int *result);
status_t zeckendorf_to_uint(const char *str, unsigned int *result);
status_t str_to_int_base(const char *str, int base, int uppercase, int *result);

#endif