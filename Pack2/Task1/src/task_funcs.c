#include <math.h>
#include <stdarg.h>
#include "task_funcs.h"

int is_prime(const int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

long long find_gcd(const long long a, const long long b) {
    long long temp_a = a;
    long long temp_b = b;
    long long temp;
    
    while (temp_b != 0) {
        temp = temp_b;
        temp_b = temp_a % temp_b;
        temp_a = temp;
    }
    return temp_a;
}

int check_denominator_and_base_divisors(const long long denominator, const int base) {
    long long temp_denominator = denominator;
    
    if (temp_denominator == 1) return 1;
    
    int temp_base = base;
    for (int i = 2; i <= temp_base; i++) {
        if (is_prime(i) && temp_base % i == 0) {
            while (temp_base % i == 0) {
                temp_base /= i;
            }
            while (temp_denominator % i == 0) {
                temp_denominator /= i;
            }
        }
    }
    return temp_denominator == 1;
}

ErrorCode all_finite(const int base, const double epsilon, Result* results, size_t amount_numbers, ...) {
    if (results == NULL) {
        return INVALID_ARGUMENT;
    }

    if (base < 2 || base > 64) {
        return INVALID_BASE;
    }
    
    if (epsilon <= 0.0) {
        return INVALID_ARGUMENT;
    }
    
    if (amount_numbers < 1) {
        return INVALID_ARGUMENT;
    }
    
    va_list args;
    va_start(args, amount_numbers);
    
    for (size_t i = 0; i < amount_numbers; i++) {
        const double number = va_arg(args, double);
        
        if (number <= 0.0 || number >= 1.0) {
            va_end(args);
            return INVALID_NUMBER;
        }
        
        results[i].number = number;

        double temp = number;
        long long denominator = 1;
        
        while (fabs(temp - round(temp)) > epsilon) {
            denominator *= base;
            temp = number * denominator;
        }
        
        if (fabs(temp - round(temp)) > epsilon) {
            results[i].is_finite = 0;
            continue;
        }
        
        const long long numerator = llround(temp);
        const long long gcd = find_gcd(numerator, denominator);
        const long long simplified_denominator = denominator / gcd;
        
        results[i].is_finite = check_denominator_and_base_divisors(simplified_denominator, base);
    }
    va_end(args);
    return SUCCESS;
}