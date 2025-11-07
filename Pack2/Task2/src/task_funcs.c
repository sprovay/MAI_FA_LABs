#include "task_funcs.h"

void* memchr(const void* str, int c, size_t n) {
    if (str == NULL) {
        return NULL;
    }
    
    const unsigned char* ptr = (const unsigned char*)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; ++i) {
        if (ptr[i] == uc) {
            return (void*)(ptr + i);
        }
    }
    return NULL;
}

int memcmp(const void* str1, const void* str2, size_t n) {
    if (str1 == NULL && str2 == NULL) {
        return 0;
    }

    if (str1 == NULL || str2 == NULL) {
        return (str1 == NULL) ? -1 : 1;
    }
    
    const unsigned char* ptr1 = (const unsigned char*)str1;
    const unsigned char* ptr2 = (const unsigned char*)str2;
    
    for (size_t i = 0; i < n; ++i) {
        if (ptr1[i] != ptr2[i]) {
            return ptr1[i] < ptr2[i] ? -1 : 1;
        }
    }
    
    return 0;
}

void* memcpy(void* dest, const void* src, size_t n) {
    if (dest == NULL || src == NULL) {
        return dest;
    }
    
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    
    return dest;
}

void* memset(void* str, int c, size_t n) {
    if (str == NULL) {
        return NULL;
    }
    
    unsigned char* ptr = (unsigned char*)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; ++i) {
        ptr[i] = uc;
    }

    return str;
}

char* strncat(char* dest, const char* src, size_t n) {
    if (dest == NULL || src == NULL) {
        return dest;
    }
    
    char* dest_end = dest;
    while (*dest_end != '\0') {
        dest_end++;
    }
    
    size_t i = 0;
    while (i < n && src[i] != '\0') {
        dest_end[i] = src[i];
        i++;
    }

    dest_end[i] = '\0';
    
    return dest;
}

char* strchr(const char* str, int c) {
    if (str == NULL) {
        return NULL;
    }
    
    unsigned char uc = (unsigned char)c;
    
    while (*str != '\0') {
        if (*str == uc) {
            return (char*)str;
        }
        str++;
    }
    
    if (uc == '\0') {
        return (char*)str;
    }
    
    return NULL;
}

int strncmp(const char* str1, const char* str2, size_t n) {
    if (str1 == NULL && str2 == NULL) {
        return 0;
    }

    if (str1 == NULL || str2 == NULL) {
        return (str1 == NULL) ? -1 : 1;
    }
    
    for (size_t i = 0; i < n; ++i) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] < (unsigned char)str2[i] ? -1 : 1;
        }
        if (str1[i] == '\0') {
            break;
        }
    }
    
    return 0;
}

char* strncpy(char* dest, const char* src, size_t n) {
    if (dest == NULL || src == NULL) {
        return dest;
    }
    
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }
    
    for (; i < n; ++i) {
        dest[i] = '\0';
    }
    
    return dest;
}

size_t strcspn(const char* str1, const char* str2) {
    if (str1 == NULL) {
        return 0;
    }
    
    if (str2 == NULL) {
        return strlen(str1);
    }
    
    size_t count = 0;
    
    while (str1[count] != '\0') {
        const char* stop_char = str2;
        while (*stop_char != '\0') {
            if (str1[count] == *stop_char) {
                return count;
            }
            stop_char++;
        }
        count++;
    }
    
    return count;
}

size_t strlen(const char* str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    
    return len;
}

char* strpbrk(const char* str1, const char* str2) {
    if (str1 == NULL || str2 == NULL) {
        return NULL;
    }
    
    while (*str1 != '\0') {
        const char* good_char = str2;
        while (*good_char != '\0') {
            if (*str1 == *good_char) {
                return (char*)str1;
            }
            good_char++;
        }
        str1++;
    }
    
    return NULL;
}

char* strrchr(const char* str, int c) {
    if (str == NULL) {
        return NULL;
    }
    
    unsigned char uc = (unsigned char)c;
    const char* lo = NULL;
    
    while (*str != '\0') {
        if (*str == uc) {
            lo = str;
        }
        str++;
    }
    
    if (uc == '\0') {
        return (char*)str;
    }
    
    return (char*)lo;
}

char* strstr(const char* haystack, const char* needle) {
    if (haystack == NULL || needle == NULL) {
        return NULL;
    }
    
    if (*needle == '\0') {
        return (char*)haystack;
    }
    
    for (size_t i = 0; haystack[i] != '\0'; ++i) {
        size_t j = 0;
        while (needle[j] != '\0' && haystack[i + j] == needle[j]) {
            j++;
        }
        if (needle[j] == '\0') {
            return (char*)(haystack + i);
        }
    }
    
    return NULL;
}

char* strtok(char* str, const char* delim) {
    static char* save_ptr = NULL;

    if (delim == NULL) {
        return NULL;
    }
    
    if (str != NULL) {
        save_ptr = str;
    } else if (save_ptr == NULL) {
        return NULL;
    }
    
    char* current = save_ptr;
    
    while (*current != '\0') {
        const char* d = delim;
        int is_delim = 0;
        while (*d != '\0') {
            if (*current == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (!is_delim) break;
        current++;
    }
    
    if (*current == '\0') {
        save_ptr = NULL;
        return NULL;
    }
    
    char* token_start = current;
    
    while (*current != '\0') {
        const char* d = delim;
        int is_delim = 0;
        while (*d != '\0') {
            if (*current == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (is_delim) {
            *current = '\0';
            save_ptr = current + 1;
            return token_start;
        }
        current++;
    }
    
    save_ptr = NULL;
    return token_start;
}