#include "task_funcs.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Ошибка: программа не принимает аргументов.\n");
        return 1;
    }

    int base = 3;
    size_t num_numbers = 1;

    Result* results = (Result*)malloc(num_numbers * sizeof(Result));
    
    if (results == NULL) {
        printf("Ошибка: не удалось выделить память.\n");
        return 1;
    }
    
    ErrorCode status = all_finite(base, 1e-14, results, num_numbers, (1.0/3.0) * (1.0/3.0) * (1.0/3.0));

    switch (status) {
        case SUCCESS:
            printf("Результаты проверки (основание %d):\n", base);
            for (size_t i = 0; i < num_numbers; i++) {
                printf("Число %.14f - %s\n", results[i].number, results[i].is_finite ? "конечно" : "бесконечно");
                    }
            break;
        case INVALID_BASE:
            printf("Ошибка: введено некорректное основание системы счисления.\n");
            break;
        case INVALID_NUMBER:
            printf("Ошибка: введено неверное значение числа. Числа должны находиться в интервале (0, 1).\n");
            break;
        case INVALID_ARGUMENT:
            printf("Ошибка: введено некорректное значение переменной количества чисел.\n");
            break;
        default:
            printf("Неизвестная ошибка.\n");
            break;
    }
    free(results);

    return 0;
}