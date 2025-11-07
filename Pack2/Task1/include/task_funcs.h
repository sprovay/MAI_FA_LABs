#ifndef TASK_FUNCS_H
#define TASK_FUNCS_H

#include <stddef.h>

typedef enum {
    SUCCESS = 0,
    INVALID_BASE = 1,
    INVALID_NUMBER = 2,
    INVALID_ARGUMENT = 3,
} ErrorCode;

typedef struct {
    double number;
    int is_finite;
} Result;

int check_denominator_and_base_divisors(const long long denominator, const int base);

ErrorCode all_finite(const int base, const double epsilon, Result* results, const size_t amount_numbers, ...);

#endif