#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    NULL_POINTER,
    INVALID_BASE
} status_t;

status_t num_to_base(const int number, const int r, char *result);

#endif