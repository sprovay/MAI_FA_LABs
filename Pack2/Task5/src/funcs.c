#include "../include/funcs.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define TARGET_LENGTH 80

status_t format_text_file(FILE* input_file, FILE* output_file) {
    if (input_file == NULL || output_file == NULL) {
        return NULL_POINTER;
    }

    status_t status = SUCCESS;
    char* line = NULL;
    size_t line_length = 0;

    while ((status = read_line(input_file, &line, &line_length)) == SUCCESS) {
        if (line_length == 0) {
            fprintf(output_file, "\n");
            free(line);
            line = NULL;
            continue;
        }

        char** formatted_lines = NULL;
        size_t formatted_count = 0;
        
        status = format_line(line, &formatted_lines, &formatted_count);
        
        if (status == SUCCESS) {
            status = write_formatted_lines(output_file, formatted_lines, formatted_count);
        }
        
        if (formatted_lines != NULL) {
            free_lines_array(formatted_lines, formatted_count);
        }
        if (line != NULL) {
            free(line);
            line = NULL;
        }
        
        if (status != SUCCESS) {
            break;
        }
    }

    if (line != NULL) {
        free(line);
        line = NULL;
    }

    return (status == EMPTY_LINE) ? SUCCESS : status;
}

status_t read_line(FILE* file, char** line, size_t* length) {
    if (file == NULL || line == NULL || length == NULL) {
        return NULL_POINTER;
    }

    size_t capacity = 128;
    *line = (char*)malloc(capacity * sizeof(char));
    if (*line == NULL) {
        return MEMORY_ALLOCATION;
    }

    *length = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        if (*length >= capacity - 1) {
            if (capacity >= MAX_LINE_LENGTH) {
                free(*line);
                *line = NULL;
                return LINE_TOO_LONG;
            }
            capacity *= 2;
            char* temp = (char*)realloc(*line, capacity * sizeof(char));
            if (temp == NULL) {
                free(*line);
                *line = NULL;
                return MEMORY_ALLOCATION;
            }
            *line = temp;
        }
        (*line)[(*length)++] = (char)ch;
    }

    if (*length == 0 && ch == EOF) {
        free(*line);
        *line = NULL;
        return EMPTY_LINE;
    }

    (*line)[*length] = '\0';
    return SUCCESS;
}

status_t format_line(const char* line, char*** formatted_lines, size_t* line_count) {
    if (line == NULL || formatted_lines == NULL || line_count == NULL) {
        return NULL_POINTER;
    }

    const size_t len = strlen(line);
    if (len == 0) {
        return EMPTY_LINE;
    }

    if (len <= TARGET_LENGTH) {
        char** lines_array = (char**)malloc(sizeof(char*));
        if (lines_array == NULL) {
            return MEMORY_ALLOCATION;
        }
        
        lines_array[0] = (char*)malloc((len + 1) * sizeof(char));
        if (lines_array[0] == NULL) {
            free(lines_array);
            return MEMORY_ALLOCATION;
        }
        
        strcpy(lines_array[0], line);
        *formatted_lines = lines_array;
        *line_count = 1;
        return SUCCESS;
    }

    size_t max_lines = (len / TARGET_LENGTH) + 2;
    char** lines_array = allocate_lines_array(max_lines);
    if (lines_array == NULL) {
        return MEMORY_ALLOCATION;
    }

    size_t current_line = 0;
    size_t pos = 0;

    while (pos < len) {
        size_t segment_end = pos + TARGET_LENGTH;
        
        if (segment_end >= len) {
            size_t remaining = len - pos;
            lines_array[current_line] = (char*)malloc((remaining + 1) * sizeof(char));
            if (lines_array[current_line] == NULL) {
                free_lines_array(lines_array, max_lines);
                return MEMORY_ALLOCATION;
            }
            strncpy(lines_array[current_line], line + pos, remaining);
            lines_array[current_line][remaining] = '\0';
            current_line++;
            break;
        }

        size_t break_pos = segment_end;
        while (break_pos > pos && !isspace((unsigned char)line[break_pos])) {
            break_pos--;
        }

        if (break_pos > pos && isspace((unsigned char)line[break_pos])) {
            segment_end = break_pos;
        } else if (segment_end < len - 1) {
            break_pos = segment_end;
            while (break_pos < len && !isspace((unsigned char)line[break_pos])) {
                break_pos++;
            }
            if (break_pos < len && isspace((unsigned char)line[break_pos])) {
                segment_end = break_pos;
            }
        } else {
            segment_end = pos + TARGET_LENGTH;
        }
        

        size_t segment_length = segment_end - pos;
        char* segment = (char*)malloc((segment_length + 1) * sizeof(char));
        if (segment == NULL) {
            free_lines_array(lines_array, max_lines);
            return MEMORY_ALLOCATION;
        }
        
        strncpy(segment, line + pos, segment_length);
        segment[segment_length] = '\0';

        if (segment_length < TARGET_LENGTH && segment_end < len) {
            size_t effective_length = segment_length;

            while (effective_length > 0 && isspace((unsigned char)segment[effective_length - 1])) {
                effective_length--;
            }

            size_t word_count = 0;
            for (size_t i = 0; i < effective_length; i++) {
                if (!isspace((unsigned char)segment[i]) && 
                    (i == 0 || isspace((unsigned char)segment[i-1]))) {
                    word_count++;
                }
            }

            if (word_count > 1) {
                size_t spaces_needed = TARGET_LENGTH - segment_length;
                size_t gaps = word_count - 1;
                
                if (gaps > 0) {
                    char* clean_segment = (char*)malloc((effective_length + 1) * sizeof(char));
                    if (clean_segment == NULL) {
                        free(segment);
                        free_lines_array(lines_array, max_lines);
                        return MEMORY_ALLOCATION;
                    }
                    
                    strncpy(clean_segment, segment, effective_length);
                    clean_segment[effective_length] = '\0';
                    
                    char* formatted_segment = (char*)malloc((TARGET_LENGTH + 1) * sizeof(char));
                    if (formatted_segment == NULL) {
                        free(clean_segment);
                        free(segment);
                        free_lines_array(lines_array, max_lines);
                        return MEMORY_ALLOCATION;
                    }
                    
                    size_t base_spaces = spaces_needed / gaps;
                    size_t extra_spaces = spaces_needed % gaps;
                    size_t formatted_pos = 0;
                    size_t space_count = 0;
                    
                    for (size_t i = 0; i < effective_length; i++) {
                        formatted_segment[formatted_pos++] = clean_segment[i];
                        
                        if (isspace((unsigned char)segment[i]) && !isspace((unsigned char)segment[i+1])) {
                            space_count++;
                            size_t current_spaces = base_spaces + (space_count <= extra_spaces ? 1 : 0);
                            
                            for (size_t s = 0; s < current_spaces; s++) {
                                formatted_segment[formatted_pos++] = ' ';
                            }
                        }
                    }
                    while (formatted_pos < TARGET_LENGTH) {
                        formatted_segment[formatted_pos++] = ' ';
                    }

                    formatted_segment[TARGET_LENGTH] = '\0';
                    free(clean_segment);
                    free(segment);
                    segment = formatted_segment;
                }
            }
        }

        lines_array[current_line] = segment;
        current_line++;
        pos = segment_end;
        
        while (pos < len && isspace((unsigned char)line[pos])) {
            pos++;
        }
    }

    if (current_line > 0) {
        char** temp = (char**)realloc(lines_array, current_line * sizeof(char*));
        if (temp == NULL) {
            free_lines_array(lines_array, max_lines);
            return MEMORY_ALLOCATION;
        }
        *formatted_lines = temp;
    } else {
        free(lines_array);
        *formatted_lines = NULL;
    }
    
    *line_count = current_line;
    return SUCCESS;
}

status_t write_formatted_lines(FILE* file, char** formatted_lines, size_t line_count) {
    if (file == NULL || formatted_lines == NULL) {
        return NULL_POINTER;
    }

    for (size_t i = 0; i < line_count; i++) {
        if (formatted_lines[i] != NULL) {
            char* line_start = formatted_lines[i];
            while (*line_start != '\0' && isspace((unsigned char)*line_start)) {
                line_start++;
            }
            if (*line_start != '\0') {
                char* end = line_start + strlen(line_start) - 1;
                while (end > line_start && isspace((unsigned char)*end)) {
                    end--;
                }
                *(end + 1) = '\0';
                
                fprintf(file, "%s\n", line_start);
            } else {
                fprintf(file, "\n");
            }
        }
    }
    
    return SUCCESS;
}


char** allocate_lines_array(size_t count) {
    if (count == 0) {
        return NULL;
    }
    return (char**)malloc(count * sizeof(char*));
}

void free_lines_array(char** lines, size_t count) {
    if (lines == NULL) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (lines[i] != NULL) {
            free(lines[i]);
        }
    }
    free(lines);
}