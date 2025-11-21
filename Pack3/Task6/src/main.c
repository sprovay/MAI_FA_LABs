#include "../include/funcs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char line[2048];

    printf("=======================================================================\n");
    printf("||         Программа для проверки расстановки скобок в строке        ||\n");
    printf("=======================================================================\n");
    printf("||                                                                   ||\n");
    printf("||         *** (Для выхода используйте Ctrl+D или Ctrl+Z) ***        ||\n");
    printf("||                                                                   ||\n");
    printf("=======================================================================\n\n");

    while (1) {
        printf("Введите строку: ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nЗавершение программы...\n");
                break;
            } else {
                printf("Ошибка при чтении ввода.\n");
                return 1;
            }
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            printf("Была введена пустая строка.\n");
            continue;
        }

        int result = check_brackets(line);

        switch (result) {
        case 0:
            printf("Скобки расставлены верно.\n");
            break;
        case 1:
            printf("Скобки расставлены неверно.\n");
            break;
        case -1:
            printf("Ошибка: некорректные входные данные.\n");
            break;
        }
        
        printf("\n");
    }

    return 0;
}