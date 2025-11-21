#include "../include/funcs.h"

#include <stdlib.h>
#include <string.h>

unsigned int add_one(unsigned int num) {
    unsigned int flag = 1;

    while (flag) {
        unsigned int carry = num & flag;
        num = num ^ flag;
        flag = carry << 1;
    }

    return num;
}

unsigned int subtract_one(unsigned int num) {
    return ~add_one(~num);
}

unsigned int abs_value(int num) {
    if (num >= 0) {
        return (unsigned int)num;
    }
    return add_one((unsigned int)(~num));
}

status_t num_to_base(const int num, const int r, char *result) {
    if (result == NULL) {
        return NULL_POINTER;
    }

    if (r < 1 || r > 5) {
        return INVALID_BASE;
    }

    if (num == 0) {
        result[0] = '0';
        result[1] = '\0';
        return SUCCESS;
    }
    
    unsigned int temp_number = abs_value(num);
    const unsigned int max_bits = 32;
    char temp_result[33];

    for (unsigned int i = 0; i < max_bits; i = add_one(i)) {
        temp_result[i] = '0';
    }
    temp_result[max_bits] = '\0';

    int index = subtract_one(max_bits);

    const unsigned int mask = subtract_one(1 << r);

    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

    unsigned int ostatok = 0;
    while (temp_number != 0) {
        ostatok = temp_number & mask;
        temp_result[index] = digits[ostatok]; 
        index = subtract_one(index);
        temp_number = temp_number >> r;
    }


    unsigned int start_pos = 0;
    while (temp_result[start_pos] == '0' && start_pos != subtract_one(max_bits)) {
        start_pos = add_one(start_pos);
    }


    unsigned int new_pos = 0;
    unsigned int curr_pos = start_pos;

    if (num < 0) {
        result[0] = '-';
        new_pos = 1;
    }
    
    while (temp_result[curr_pos] != '\0') {
        result[new_pos] = temp_result[curr_pos];
        new_pos = add_one(new_pos);
        curr_pos = add_one(curr_pos);
    }
    result[new_pos] = '\0';

    return SUCCESS;
}