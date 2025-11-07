#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    NULL_POINTER,
    MEMORY_ALLOCATION,
    FILE_OPEN,
    INVALID_INPUT,
    EMPTY_LINE,
    LINE_TOO_LONG
} status_t;

status_t format_text_file(FILE* input_file, FILE* output_file);
status_t read_line(FILE* file, char** line, size_t* length);
status_t format_line(const char* line, char*** formatted_lines, size_t* line_count);
status_t write_formatted_lines(FILE* file, char** formatted_lines, size_t line_count);
char** allocate_lines_array(size_t count);
void free_lines_array(char** lines, size_t count);

#endif