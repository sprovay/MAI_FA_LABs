#include "../include/funcs.h"

#include <stdlib.h>

int check_brackets(const char *str) {
    if (str == NULL) {
        return -1;
    }

    size_t size = strlen(str);
    if (size == 0) {
        return 0;
    }

    char *line = (char *)malloc(size * sizeof(char));
    if (line == NULL) {
        return -1;
    }

    int result = 0;

    char openings[] = "([{<";
    char endings[] = ")]}<";

    int top = -1;

    for (int curr_pos = 0; str[curr_pos] != '\0'; curr_pos++) {
        char c = str[curr_pos];

        if (strchr(openings, c) != NULL) {
            line[++top] = c;

        } else if (strchr(endings, c) != NULL) {
            if (top == -1) {
                result = 1;
                break;
            }

            char* curr_end = strchr(endings, c);
            int index = curr_end - endings;

            if (line[top] != openings[index]) {
                result = 1;
                break;
            }

            top--;
        }
    }

    if (result == 0 && top != -1) {
        result = 1;
    }
    
    free(line);
    return result;
}