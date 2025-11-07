#include "../include/funcs.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>

status_t roman_to_int(const char *str, int *result) {
    if (str == NULL || result == NULL) {
        return NULL_POINTER;
    }

    if (*str == '\0') {
        return INVALID_ROMAN;
    }

    int values[256] = {0};
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;
    int prev_value = 0;

    for (int i = strlen(str) - 1; i >= 0; i--) {
        char c = toupper(str[i]);
        int current = values[(unsigned char)c];
        
        if (current == 0) {
            return INVALID_ROMAN;
        }

        if (current < prev_value) {
            total -= current;
        } else {
            total += current;
        }
        prev_value = current;
    }

    if (total <= 0 || total > 3999) {
        return INVALID_ROMAN;
    }

    *result = total;
    return SUCCESS;
}

status_t zeckendorf_to_uint(const char *str, unsigned int *result) {
    if (str == NULL || result == NULL) {
        return NULL_POINTER;
    }

    size_t len = strlen(str);
    if (len < 2 || str[len - 1] != '1') {
        return INVALID_ZEKKENDORF;
    }

    unsigned int fib[47];
    fib[0] = 1;
    fib[1] = 2;
    
    int fib_count = 2;
    
    while (fib_count < 47) {
        unsigned int next_fib = fib[fib_count - 1] + fib[fib_count - 2];
        if (next_fib < fib[fib_count - 1]) {
            break;
        }
        fib[fib_count] = next_fib;
        fib_count++;
    }

    unsigned int total = 0;
    int str_index = 0;

    for (int i = 0; i < fib_count && str_index < (int)len - 1; i++) {
        char c = str[str_index++];
        if (c == '1') {
            total += fib[i];
        } else if (c != '0') {
            return INVALID_ZEKKENDORF;
        }
    }

    *result = total;
    return SUCCESS;
}

status_t str_to_int_base(const char *str, int base, int uppercase, int *result) {
    if (str == NULL || result == NULL) {
        return NULL_POINTER;
    }

    if (base < 2 || base > 36) {
        base = 10;
    }

    if (*str == '\0') {
        return INVALID_NUMBER;
    }

    int value = 0;
    int is_negative = 0;
    const char *ptr = str;

    if (*ptr == '-') {
        is_negative = 1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }

    while (*ptr == '0') {
        ptr++;
    }

    while (*ptr != '\0' && !isspace(*ptr)) {
        char c = *ptr;
        int digit;

        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            if (uppercase) {
                digit = toupper(c) - 'A' + 10;
            } else {
                digit = tolower(c) - 'a' + 10;
            }
        } else {
            return INVALID_NUMBER;
        }

        if (digit >= base) {
            return INVALID_NUMBER;
        }

        value = value * base + digit;
        ptr++;
    }

    *result = is_negative ? -value : value;
    return SUCCESS;
}


int read_token(FILE *stream, char *buffer, size_t buffer_size) {
    if (stream == NULL || buffer == NULL || buffer_size == 0) {
        return 0;
    }

    int c;
    size_t index = 0;
    
    while ((c = fgetc(stream)) != EOF && isspace(c)) {
    }
    
    if (c == EOF) {
        return 0;
    }
    
    do {
        if (index < buffer_size - 1) {
            buffer[index++] = (char)c;
        }
        c = fgetc(stream);
    } while (c != EOF && !isspace(c) && index < buffer_size - 1);
    
    if (c != EOF && !isspace(c)) {
        ungetc(c, stream);
    }
    
    buffer[index] = '\0';
    return 1;
}

int overfscanf(FILE *stream, const char *format, ...) {
    if (stream == NULL || format == NULL) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    int assigned_count = 0;
    const char *fmt_ptr = format;

    while (*fmt_ptr != '\0') {
        if (*fmt_ptr != '%') {
            int expected = *fmt_ptr;
            if (isspace(expected)) {
                int c;
                while ((c = fgetc(stream)) != EOF && isspace(c)) {
                }
                if (c != EOF) {
                    ungetc(c, stream);
                } else {
                    break;
                }
            } else {
                int actual = fgetc(stream);
                if (actual == EOF) break;
                if (actual != expected) {
                    ungetc(actual, stream);
                    break;
                }
            }
            fmt_ptr++;
            continue;
        }

        fmt_ptr++;

        if (strncmp(fmt_ptr, "Ro", 2) == 0) {
            int *result = va_arg(args, int*);
            char buffer[256];
            
            if (read_token(stream, buffer, sizeof(buffer))) {
                int value;
                status_t status = roman_to_int(buffer, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Zr", 2) == 0) {
            unsigned int *result = va_arg(args, unsigned int*);
            char buffer[256];
            
            if (read_token(stream, buffer, sizeof(buffer))) {
                unsigned int value;
                status_t status = zeckendorf_to_uint(buffer, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Cv", 2) == 0) {
            int *result = va_arg(args, int*);
            int base = va_arg(args, int);
            char buffer[256];
            
            if (read_token(stream, buffer, sizeof(buffer))) {
                int value;
                status_t status = str_to_int_base(buffer, base, 0, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "CV", 2) == 0) {
            int *result = va_arg(args, int*);
            int base = va_arg(args, int);
            char buffer[256];
            
            if (read_token(stream, buffer, sizeof(buffer))) {
                int value;
                status_t status = str_to_int_base(buffer, base, 1, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else {
            char specifier = *fmt_ptr;
            
            if (specifier != 'c') {
                int c;
                while ((c = fgetc(stream)) != EOF && isspace(c)) {
                }
                if (c != EOF) {
                    ungetc(c, stream);
                } else {
                    switch (specifier) {
                        case 'd': case 'i': case 'u': case 'o': case 'x': case 'X':
                            va_arg(args, int*);
                            break;
                        case 'f': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A':
                            va_arg(args, double*);
                            break;
                        case 's':
                            va_arg(args, char*);
                            break;
                        case 'p':
                            va_arg(args, void**);
                            break;
                    fmt_ptr++;
                    continue;
                    }
                }
            }
            
            switch (specifier) {
                case 'd': case 'i': {
                    int *result = va_arg(args, int*);
                    char buffer[256];
                    if (read_token(stream, buffer, sizeof(buffer))) {
                        char *endptr;
                        long value = strtol(buffer, &endptr, specifier == 'i' ? 0 : 10);
                        if (endptr != buffer) {
                            *result = (int)value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                case 'u': case 'o': case 'x': case 'X': {
                    unsigned int *result = va_arg(args, unsigned int*);
                    char buffer[256];
                    if (read_token(stream, buffer, sizeof(buffer))) {
                        char *endptr;
                        int base = 10;
                        if (specifier == 'o') base = 8;
                        else if (specifier == 'x' || specifier == 'X') base = 16;
                        unsigned long value = strtoul(buffer, &endptr, base);
                        if (endptr != buffer) {
                            *result = (unsigned int)value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                case 'f': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A': {
                    float *result = va_arg(args, float*);
                    char buffer[256];
                    if (read_token(stream, buffer, sizeof(buffer))) {
                        char *endptr;
                        float value = strtof(buffer, &endptr);
                        if (endptr != buffer) {
                            *result = value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                case 's': {
                    char *result = va_arg(args, char*);
                    char buffer[256];
                    if (read_token(stream, buffer, sizeof(buffer))) {
                        strcpy(result, buffer);
                        assigned_count++;
                    }
                    break;
                }
                case 'c': {
                    char *result = va_arg(args, char*);
                    int c = fgetc(stream);
                    if (c != EOF) {
                        *result = (char)c;
                        assigned_count++;
                    }
                    break;
                }
                case 'p': {
                    void **result = va_arg(args, void**);
                    char buffer[256];
                    if (read_token(stream, buffer, sizeof(buffer))) {
                        char *endptr;
                        unsigned long value = strtoul(buffer, &endptr, 16);
                        if (endptr != buffer) {
                            *result = (void*)(uintptr_t)value;
                            assigned_count++;
                        }
                    }
                    break;
                }

                default: {
                    break;
                }
            }
            fmt_ptr++;
        }
    }

    va_end(args);
    return assigned_count;
}

int read_token_from_string(const char **str_ptr, char *buffer, size_t buffer_size) {
    if (str_ptr == NULL || *str_ptr == NULL || buffer == NULL || buffer_size == 0) {
        return 0;
    }

    const char *ptr = *str_ptr;
    size_t index = 0;
    
    while (*ptr != '\0' && isspace(*ptr)) {
        ptr++;
    }
    
    if (*ptr == '\0') {
        return 0;
    }
    
    while (*ptr != '\0' && !isspace(*ptr) && index < buffer_size - 1) {
        buffer[index++] = *ptr++;
    }
    
    buffer[index] = '\0';
    *str_ptr = ptr;
    
    return index > 0;
}

int oversscanf(const char *str, const char *format, ...) {
    if (str == NULL || format == NULL) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    int assigned_count = 0;
    const char *str_ptr = str;
    const char *fmt_ptr = format;

    while (*fmt_ptr != '\0' && *str_ptr != '\0') {
        if (*fmt_ptr != '%') {
            if (isspace(*fmt_ptr)) {
                while (isspace(*fmt_ptr)) {
                    fmt_ptr++;
                }
                while (*str_ptr != '\0' && isspace(*str_ptr)) {
                    str_ptr++;
                }
            } else {
                if (*str_ptr != *fmt_ptr) {
                    break;
                }
                str_ptr++;
                fmt_ptr++;
            }
            continue;
        }

        fmt_ptr++;

        if (strncmp(fmt_ptr, "Ro", 2) == 0) {
            int *result = va_arg(args, int*);
            char buffer[256];
            
            if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                int value;
                status_t status = roman_to_int(buffer, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Zr", 2) == 0) {
            unsigned int *result = va_arg(args, unsigned int*);
            char buffer[256];
            
            if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                unsigned int value;
                status_t status = zeckendorf_to_uint(buffer, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Cv", 2) == 0) {
            int *result = va_arg(args, int*);
            int base = va_arg(args, int);
            char buffer[256];
            
            if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                int value;
                status_t status = str_to_int_base(buffer, base, 0, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "CV", 2) == 0) {
            int *result = va_arg(args, int*);
            int base = va_arg(args, int);
            char buffer[256];
            
            if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                int value;
                status_t status = str_to_int_base(buffer, base, 1, &value);
                if (status == SUCCESS) {
                    *result = value;
                    assigned_count++;
                }
            }
            fmt_ptr += 2;
            
        } else {
            char specifier = *fmt_ptr;
            
            if (specifier != 'c') {
                while (*str_ptr != '\0' && isspace(*str_ptr)) {
                    str_ptr++;
                }
            }
            
            if (*str_ptr == '\0') {
                switch (specifier) {
                    case 'd': case 'i': case 'u': case 'o': case 'x': case 'X':
                        va_arg(args, int*);
                        break;
                    case 'f': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A':
                        va_arg(args, double*);
                        break;
                    case 's':
                        va_arg(args, char*);
                        break;
                    case 'p':
                        va_arg(args, void**);
                        break;
                fmt_ptr++;
                continue;
                }
            }
            
            switch (specifier) {
                case 'd': case 'i': {
                    int *result = va_arg(args, int*);
                    char buffer[256];
                    if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                        char *endptr;
                        long value = strtol(buffer, &endptr, specifier == 'i' ? 0 : 10);
                        if (endptr != buffer) {
                            *result = (int)value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                case 'u': case 'o': case 'x': case 'X': {
                    unsigned int *result = va_arg(args, unsigned int*);
                    char buffer[256];
                    if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                        char *endptr;
                        int base = 10;
                        if (specifier == 'o') base = 8;
                        else if (specifier == 'x' || specifier == 'X') base = 16;
                        unsigned long value = strtoul(buffer, &endptr, base);
                        if (endptr != buffer) {
                            *result = (unsigned int)value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                case 'f': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A': {
                    float *result = va_arg(args, float*);
                    char buffer[256];
                    if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                        char *endptr;
                        float value = strtof(buffer, &endptr);
                        if (endptr != buffer) {
                            *result = value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                case 's': {
                    char *result = va_arg(args, char*);
                    char buffer[256];
                    if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                        strcpy(result, buffer);
                        assigned_count++;
                    }
                    break;
                }
                case 'c': {
                    char *result = va_arg(args, char*);
                    if (*str_ptr != '\0') {
                        *result = *str_ptr++;
                        assigned_count++;
                    }
                    break;
                }
                case 'p': {
                    void **result = va_arg(args, void**);
                    char buffer[256];
                    if (read_token_from_string(&str_ptr, buffer, sizeof(buffer))) {
                        char *endptr;
                        unsigned long value = strtoul(buffer, &endptr, 16);
                        if (endptr != buffer) {
                            *result = (void*)(uintptr_t)value;
                            assigned_count++;
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            fmt_ptr++;
        }
    }

    va_end(args);
    return assigned_count;
}