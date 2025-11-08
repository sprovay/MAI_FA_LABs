#include "../include/funcs.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>

status_t int_to_roman(int value, char *buffer, size_t buffer_size) {
    if (buffer == NULL) {
        return NULL_POINTER;
    }

    if (value <= 0 || value > 3999) {
        return INVALID_ROMAN;
    }
    
    const char *thousands[] = {"", "M", "MM", "MMM"};
    const char *hundreds[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    const char *tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    const char *ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    
    int thousand = value / 1000;
    int hundred = (value % 1000) / 100;
    int ten = (value % 100) / 10;
    int one = value % 10;
    
    size_t needed_size = strlen(thousands[thousand]) + strlen(hundreds[hundred]) + 
                        strlen(tens[ten]) + strlen(ones[one]) + 1;
    
    if (needed_size > buffer_size) {
        return BUFFER_OVERFLOW;
    }
    
    strcpy(buffer, thousands[thousand]);
    strcat(buffer, hundreds[hundred]);
    strcat(buffer, tens[ten]);
    strcat(buffer, ones[one]);
    
    return SUCCESS;
}

status_t uint_to_zeckendorf(unsigned int num, char *buffer, size_t buffer_size) {
    if (buffer == NULL) {
        return NULL_POINTER;
    }
    
    if (num == 0) {
        if (buffer_size < 3) {
            return BUFFER_OVERFLOW;
        }
        strcpy(buffer, "01");
        return SUCCESS;
    }

    if (num == 1) {
        if (buffer_size < 3) {
            return BUFFER_OVERFLOW;
        }
        strcpy(buffer, "11");
        return SUCCESS;
    }
    
    unsigned int fib[47];
    fib[0] = 1;
    fib[1] = 2;
    
    int fib_count = 2;
    
    while (fib_count < 47) {
        unsigned int next_fib = fib[fib_count - 1] + fib[fib_count - 2];
        if (next_fib > num) {
            break;
        }
        fib[fib_count] = next_fib;
        fib_count++;
    }
    
    if (buffer_size < (size_t)(fib_count + 2)) {
        return BUFFER_OVERFLOW;
    }
    
    char *current = buffer;
    unsigned int remaining = num;
    
    for (int i = 0; i < fib_count; i++) {
        *current++ = '0';
    }
    
    for (int i = fib_count - 1; i >= 0; i--) {
        if (remaining >= fib[i]) {
            buffer[i] = '1';
            remaining -= fib[i];
            if (i > 0) {
                i--;
            }
        }
    }
    
    *current++ = '1';
    *current = '\0';
    
    return SUCCESS;
}

status_t int_to_base(int value, int base, int uppercase, char *buffer, size_t buffer_size) {
    if (buffer == NULL) {
        return NULL_POINTER;
    }

    if (base < 2 || base > 36) {
        base = 10;
    }
    
    if (buffer_size < 2) {
        return BUFFER_OVERFLOW;
    }
    
    if (base == 10) {
        sprintf(buffer, "%d", value);
        return SUCCESS;
    }

    char temp_buffer[1024];
    int temp_index = 0;
    unsigned int unsigned_value;
    int is_negative = 0;
    
    if (value < 0) {
        is_negative = 1;
        unsigned_value = (unsigned int)(-value);
    } else {
        unsigned_value = (unsigned int)value;
    }
    
    do {
        unsigned int digit = unsigned_value % (unsigned int)base;
        char c;
        if (digit < 10) {
            c = '0' + digit;
        } else {
            c = (uppercase ? 'A' : 'a') + (digit - 10);
        }
        temp_buffer[temp_index++] = c;
        unsigned_value /= (unsigned int)base;
    } while (unsigned_value > 0 && temp_index < 1023);
    
    if (temp_index >= 1023) {
        return BUFFER_OVERFLOW;
    }
    
    if (is_negative) {
        temp_buffer[temp_index++] = '-';
    }
    
    if (temp_index + 1 > buffer_size) {
        return BUFFER_OVERFLOW;
    }
    
    for (int i = 0; i < temp_index; i++) {
        buffer[i] = temp_buffer[temp_index - 1 - i];
    }
    buffer[temp_index] = '\0';
    
    return SUCCESS;
}

status_t str_to_int_base(const char *str, int base, int uppercase, int *result) {
    if (result == NULL) {
        return NULL_POINTER;
    }

    if (str == NULL) {
        return NULL_POINTER;
    }

    if (base < 2 || base > 36) {
        base = 10;
    }
    
    if (str[0] == '\0') {
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
    
    while (*ptr != '\0') {
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

static status_t memory_dump_binary(const void *data, size_t size, char *buffer, size_t buffer_size) {
    const unsigned char *bytes = (const unsigned char *)data;
    size_t needed_size = size * 9 - 1;
    if (needed_size > buffer_size) {
        return BUFFER_OVERFLOW;
    }
    
    size_t buffer_index = 0;
    for (size_t i = 0; i < size; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            buffer[buffer_index++] = (bytes[i] & (1 << bit)) ? '1' : '0';
        }
        if (i < size - 1) {
            buffer[buffer_index++] = ' ';
        }
    }
    buffer[buffer_index] = '\0';
    
    return SUCCESS;
}

status_t memory_dump_int(int value, char *buffer, size_t buffer_size) {
    return memory_dump_binary(&value, sizeof(int), buffer, buffer_size);
}

status_t memory_dump_uint(unsigned int value, char *buffer, size_t buffer_size) {
    return memory_dump_binary(&value, sizeof(unsigned int), buffer, buffer_size);
}

status_t memory_dump_double(double value, char *buffer, size_t buffer_size) {
    return memory_dump_binary(&value, sizeof(double), buffer, buffer_size);
}

status_t memory_dump_float(float value, char *buffer, size_t buffer_size) {
    return memory_dump_binary(&value, sizeof(float), buffer, buffer_size);
}

int overfprintf(FILE *stream, const char *format, ...) {
    if (stream == NULL || format == NULL) return -1;
    
    va_list args;
    va_start(args, format);
    
    char buffer[1024];
    int total_chars = 0;
    const char *fmt_ptr = format;
    
    while (*fmt_ptr != '\0') {
        if (*fmt_ptr != '%') {
            if (fputc(*fmt_ptr, stream) == EOF) {
                va_end(args);
                return -1;
            }
            total_chars++;
            fmt_ptr++;
            continue;
        }
        
        fmt_ptr++;
        
        if (strncmp(fmt_ptr, "Ro", 2) == 0) {
            int value = va_arg(args, int);
            if (int_to_roman(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Zr", 2) == 0) {
            unsigned int value = va_arg(args, unsigned int);
            if (uint_to_zeckendorf(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Cv", 2) == 0) {
            int value = va_arg(args, int);
            int base = va_arg(args, int);
            if (int_to_base(value, base, 0, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "CV", 2) == 0) {
            int value = va_arg(args, int);
            int base = va_arg(args, int);
            if (int_to_base(value, base, 1, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "to", 2) == 0) {
            const char *str_val = va_arg(args, const char *);
            int base = va_arg(args, int);
            int result;
            if (str_to_int_base(str_val, base, 0, &result) == SUCCESS) {
                int len = fprintf(stream, "%d", result);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "TO", 2) == 0) {
            const char *str_val = va_arg(args, const char *);
            int base = va_arg(args, int);
            int result;
            if (str_to_int_base(str_val, base, 1, &result) == SUCCESS) {
                int len = fprintf(stream, "%d", result);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "mi", 2) == 0) {
            int value = va_arg(args, int);
            if (memory_dump_int(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "mu", 2) == 0) {
            unsigned int value = va_arg(args, unsigned int);
            if (memory_dump_uint(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "md", 2) == 0) {
            double value = va_arg(args, double);
            if (memory_dump_double(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "mf", 2) == 0) {
            float value = (float)va_arg(args, double);
            if (memory_dump_float(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = fprintf(stream, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else {
            const char *spec_start = fmt_ptr;
            
            while (*fmt_ptr != '\0' && 
                   *fmt_ptr != 'd' && *fmt_ptr != 'i' && *fmt_ptr != 'u' &&
                   *fmt_ptr != 'o' && *fmt_ptr != 'x' && *fmt_ptr != 'X' &&
                   *fmt_ptr != 'f' && *fmt_ptr != 'F' && *fmt_ptr != 'e' &&
                   *fmt_ptr != 'E' && *fmt_ptr != 'g' && *fmt_ptr != 'G' &&
                   *fmt_ptr != 'a' && *fmt_ptr != 'A' && *fmt_ptr != 'c' &&
                   *fmt_ptr != 's' && *fmt_ptr != 'p' && *fmt_ptr != 'n' &&
                   *fmt_ptr != '%') {
                fmt_ptr++;
            }
            
            if (*fmt_ptr == '\0') {
                va_end(args);
                return -1;
            }
            
            fmt_ptr++;
            
            size_t spec_len = fmt_ptr - spec_start;
            
            if (spec_len < 100) {
                char specifier[102] = "%";
                strncat(specifier, spec_start, spec_len);
                
                int len = vfprintf(stream, specifier, args);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                total_chars += len;
            } else {
                va_end(args);
                return -1;
            }
        }
    }
    va_end(args);
    return total_chars;
}

int oversprintf(char *str, const char *format, ...) {
    if (str == NULL || format == NULL) return -1;
    
    va_list args;
    va_start(args, format);
    
    char buffer[1024];
    int total_chars = 0;
    const char *fmt_ptr = format;
    
    while (*fmt_ptr != '\0') {
        if (*fmt_ptr != '%') {
            *str++ = *fmt_ptr++;
            total_chars++;
            continue;
        }
        
        fmt_ptr++;
        
        if (strncmp(fmt_ptr, "Ro", 2) == 0) {
            int value = va_arg(args, int);
            if (int_to_roman(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Zr", 2) == 0) {
            unsigned int value = va_arg(args, unsigned int);
            if (uint_to_zeckendorf(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "Cv", 2) == 0) {
            int value = va_arg(args, int);
            int base = va_arg(args, int);
            if (int_to_base(value, base, 0, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "CV", 2) == 0) {
            int value = va_arg(args, int);
            int base = va_arg(args, int);
            if (int_to_base(value, base, 1, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "to", 2) == 0) {
            const char *str_val = va_arg(args, const char *);
            int base = va_arg(args, int);
            int result;
            if (str_to_int_base(str_val, base, 0, &result) == SUCCESS) {
                int len = sprintf(str, "%d", result);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "TO", 2) == 0) {
            const char *str_val = va_arg(args, const char *);
            int base = va_arg(args, int);
            int result;
            if (str_to_int_base(str_val, base, 1, &result) == SUCCESS) {
                int len = sprintf(str, "%d", result);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "mi", 2) == 0) {
            int value = va_arg(args, int);
            if (memory_dump_int(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "mu", 2) == 0) {
            unsigned int value = va_arg(args, unsigned int);
            if (memory_dump_uint(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "md", 2) == 0) {
            double value = va_arg(args, double);
            if (memory_dump_double(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else if (strncmp(fmt_ptr, "mf", 2) == 0) {
            float value = (float)va_arg(args, double);
            if (memory_dump_float(value, buffer, sizeof(buffer)) == SUCCESS) {
                int len = sprintf(str, "%s", buffer);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            }
            fmt_ptr += 2;
            
        } else {
            const char *spec_start = fmt_ptr;
            
            while (*fmt_ptr != '\0' && 
                   *fmt_ptr != 'd' && *fmt_ptr != 'i' && *fmt_ptr != 'u' &&
                   *fmt_ptr != 'o' && *fmt_ptr != 'x' && *fmt_ptr != 'X' &&
                   *fmt_ptr != 'f' && *fmt_ptr != 'F' && *fmt_ptr != 'e' &&
                   *fmt_ptr != 'E' && *fmt_ptr != 'g' && *fmt_ptr != 'G' &&
                   *fmt_ptr != 'a' && *fmt_ptr != 'A' && *fmt_ptr != 'c' &&
                   *fmt_ptr != 's' && *fmt_ptr != 'p' && *fmt_ptr != 'n' &&
                   *fmt_ptr != '%') {
                fmt_ptr++;
            }
            
            if (*fmt_ptr == '\0') {
                va_end(args);
                return -1;
            }
            
            fmt_ptr++;
            size_t spec_len = fmt_ptr - spec_start;
            
            if (spec_len < 100) {
                char specifier[102] = "%";
                strncat(specifier, spec_start, spec_len);
                
                int len = vsprintf(str, specifier, args);
                if (len < 0) {
                    va_end(args);
                    return -1;
                }
                str += len;
                total_chars += len;
            } else {
                va_end(args);
                return -1;
            }
        }
    }
    
    *str = '\0';
    va_end(args);
    
    return total_chars;
}