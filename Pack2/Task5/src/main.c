#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 256

int get_path(char* buffer, size_t size, const char* prompt) {
    if (buffer == NULL || size == 0) {
        return 0;
    }

    printf("%s", prompt);
    
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    if (strlen(buffer) == 0) {
        return 0;
    }

    return 1;
}

int main(void) {
    char input_path[MAX_PATH_LENGTH];
    char output_path[MAX_PATH_LENGTH];

    if (!get_path(input_path, sizeof(input_path), "Введите путь к файлу для чтения: ")) {
        printf("Error: Недопустимый путь к файлу для чтения.\n");
        return EXIT_FAILURE;
    }

    if (!get_path(output_path, sizeof(output_path), "Введите путь к файлу для записи: ")) {
        printf("Error: Недопустимый путь к файлу для записи.\n");
        return EXIT_FAILURE;
    }

    FILE* input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("Error: Не получилось открыть файл для чтения '%s'\n", input_path);
        return EXIT_FAILURE;
    }

    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        printf("Error: Не получилось открыть файл для записи '%s'\n", output_path);
        fclose(input_file);
        return EXIT_FAILURE;
    }

    status_t result = format_text_file(input_file, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    if (result == SUCCESS) {
        printf("\nОбработка выполнена успешно\n");
    } else {
        printf("%d \n", result);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}