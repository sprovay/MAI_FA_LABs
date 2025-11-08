#include "../include/funcs.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

void test_roman_numerals() {
    printf("=== ТЕСТ РИМСКИХ ЦИФР (%%Ro) ===\n");
    
    char buffer[64];
    int values[] = {1, 4, 9, 49, 99, 444, 999, 1999, 2024, 3999};
    int count = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < count; i++) {
        status_t status = int_to_roman(values[i], buffer, sizeof(buffer));
        printf("%4d → %-12s (статус: %s)\n", 
               values[i], buffer, 
               status == SUCCESS ? "SUCCESS" : "ERROR");
    }
    
    printf("\nГраничные случаи:\n");
    status_t status = int_to_roman(0, buffer, sizeof(buffer));
    printf("%4d → %-12s (статус: %s)\n", 0, buffer, status == INVALID_ROMAN ? "INVALID_ROMAN" : "ERROR");
    
    status = int_to_roman(4000, buffer, sizeof(buffer));
    printf("%4d → %-12s (статус: %s)\n", 4000, buffer, status == INVALID_ROMAN ? "INVALID_ROMAN" : "ERROR");
    
    printf("\n");
}

void test_zeckendorf_correct_order() {
    printf("=== ПРОВЕРКА ЦЕКЕНДОРФА ===\n");
    
    struct {
        unsigned int value;
        const char *expected;
        const char *explanation;
    } test_cases[] = {
        {0, "01", "0 = 0"},
        {1, "11", "1 = 1"},
        {2, "011", "2 = 2"},
        {3, "0011", "3 = 3"},
        {4, "1011", "4 = 3 + 1"},
        {5, "00011", "5 = 5"},
        {6, "10011", "6 = 5 + 1"},
        {7, "01011", "7 = 5 + 2"},
        {8, "000011", "8 = 8"},
        {13, "0000011", "13 = 13"},
        {20, "0101011", "20 = 13 + 5 + 2"}
    };
    
    int count = sizeof(test_cases) / sizeof(test_cases[0]);
    char buffer[64];
    
    for (int i = 0; i < count; i++) {
        status_t status = uint_to_zeckendorf(test_cases[i].value, buffer, sizeof(buffer));
        int correct = (strcmp(buffer, test_cases[i].expected) == 0);
        printf("%2u → %-8s (ожидалось: %-8s) %s %s\n", 
               test_cases[i].value, buffer, test_cases[i].expected,
               correct ? "✓" : "✗",
               correct ? "" : test_cases[i].explanation);
    }
    printf("\n");
}

void test_base_conversion() {
    printf("=== ТЕСТ СИСТЕМ СЧИСЛЕНИЯ (%%Cv/%%CV) ===\n");
    
    char buffer[64];
    
    printf("Преобразование 255 в разные системы:\n");
    int bases[] = {2, 8, 10, 16, 36};
    int base_count = sizeof(bases) / sizeof(bases[0]);
    
    for (int i = 0; i < base_count; i++) {
        status_t status = int_to_base(255, bases[i], 0, buffer, sizeof(buffer));
        printf("255 в %2d-чной: %-12s (статус: %s)\n", 
               bases[i], buffer, 
               status == SUCCESS ? "SUCCESS" : "ERROR");
    }
    
    printf("\nВерхний регистр:\n");
    int_to_base(255, 16, 1, buffer, sizeof(buffer));
    printf("255 в 16-чной: %s\n", buffer);
    
    int_to_base(1002, 36, 1, buffer, sizeof(buffer));
    printf("1002 в 36-чной: %s\n", buffer);
    
    printf("\nОтрицательные числа:\n");
    int_to_base(-42, 10, 0, buffer, sizeof(buffer));
    printf("-42 в 10-чной: %s\n", buffer);
    
    int_to_base(-255, 16, 0, buffer, sizeof(buffer));
    printf("-255 в 16-чной: %s\n", buffer);
    
    printf("\n");
}

void test_string_to_int() {
    printf("=== ТЕСТ ПРЕОБРАЗОВАНИЯ СТРОК (%%to/%%TO) ===\n");
    
    int result;
    
    struct {
        const char *str;
        int base;
        int uppercase;
        int expected;
    } test_cases[] = {
        {"1010", 2, 0, 10},
        {"ff", 16, 0, 255},
        {"777", 8, 0, 511},
        {"z", 36, 0, 35},
        {"FF", 16, 1, 255},
        {"ABC", 16, 1, 2748},
        {"RU", 36, 1, 1002},
        {"-1010", 2, 0, -10},
        {"-ff", 16, 0, -255}
    };
    
    int count = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < count; i++) {
        status_t status = str_to_int_base(test_cases[i].str, test_cases[i].base, 
                                         test_cases[i].uppercase, &result);
        printf("\"%s\" из %2d-чной → %5d (ожидалось: %5d) %s\n",
               test_cases[i].str, test_cases[i].base, result, test_cases[i].expected,
               status == SUCCESS ? "SUCCESS" : "ERROR");
    }
    printf("\n");
}

void test_memory_dump() {
    printf("=== ТЕСТ ДАМПА ПАМЯТИ (%%mi/%%mu/%%mf/%%md) ===\n");
    
    char buffer[256];
    
    printf("Целые числа:\n");
    memory_dump_int(1, buffer, sizeof(buffer));
    printf("int 1: %s\n", buffer);
    
    memory_dump_int(-42, buffer, sizeof(buffer));
    printf("int -42: %s\n", buffer);
    
    memory_dump_uint(1234567890, buffer, sizeof(buffer));
    printf("unsigned int 1234567890: %s\n", buffer);
    
    printf("\nВещественные числа:\n");
    memory_dump_float(1.0f, buffer, sizeof(buffer));
    printf("float 1.0: %s\n", buffer);
    
    memory_dump_float(-3.14159f, buffer, sizeof(buffer));
    printf("float -3.14159: %s\n", buffer);
    
    memory_dump_double(123.456789, buffer, sizeof(buffer));
    printf("double 123.456789: %s\n", buffer);
    
    printf("\nГраничные значения:\n");
    memory_dump_int(INT_MAX, buffer, sizeof(buffer));
    printf("INT_MAX: %s\n", buffer);
    
    memory_dump_int(INT_MIN, buffer, sizeof(buffer));
    printf("INT_MIN: %s\n", buffer);
    
    memory_dump_uint(UINT_MAX, buffer, sizeof(buffer));
    printf("UINT_MAX: %s\n", buffer);
    
    printf("\n");
}

void test_overfprintf() {
    printf("=== ТЕСТ OVERFPRINTF (запись в файл) ===\n");
    
    FILE *f = fopen("test_output.txt", "w");
    if (f == NULL) {
        printf("Ошибка открытия файла!\n");
        return;
    }
    
    overfprintf(f, "=== ТЕСТОВЫЙ ВЫВОД OVERFPRINTF ===\n\n");
    
    overfprintf(f, "1. Римские цифры:\n");
    overfprintf(f, "   1999 = %Ro\n", 1999);
    overfprintf(f, "   2024 = %Ro\n", 2024);
    overfprintf(f, "   49   = %Ro\n\n", 49);
    
    overfprintf(f, "2. Цекендорфово представление:\n");
    overfprintf(f, "   0  = %Zr\n", 0);
    overfprintf(f, "   1  = %Zr\n", 1);
    overfprintf(f, "   4  = %Zr\n", 4);
    overfprintf(f, "   13 = %Zr\n\n", 13);
    
    overfprintf(f, "3. Системы счисления:\n");
    overfprintf(f, "   255 в 16-чной (нижний): %Cv\n", 255, 16);
    overfprintf(f, "   255 в 16-чной (верхний): %CV\n", 255, 16);
    overfprintf(f, "   -42 в 10-чной: %Cv\n\n", -42, 10);
    
    overfprintf(f, "4. Преобразование из строки:\n");
    overfprintf(f, "   \"ff\" из 16-чной = %to\n", "ff", 16);
    overfprintf(f, "   \"1010\" из 2-чной = %to\n", "1010", 2);
    overfprintf(f, "   \"ABC\" из 16-чной = %TO\n\n", "ABC", 16);
    
    overfprintf(f, "5. Дамп памяти:\n");
    overfprintf(f, "   int 42: %mi\n", 42);
    overfprintf(f, "   unsigned int 123: %mu\n", 123);
    overfprintf(f, "   float 3.14: %mf\n", 3.14f);
    overfprintf(f, "   double 123.456: %md\n\n", 123.456);
    
    overfprintf(f, "6. Комбинированные тесты:\n");
    overfprintf(f, "   Число 20: римское=%Ro, цекендорф=%Zr\n", 20, 20);
    overfprintf(f, "   Число 255: в 2-чной=%Cv, в 16-чной=%CV\n", 255, 2, 255, 16);
    
    fclose(f);
    printf("Результаты записаны в файл 'test_output.txt'\n\n");
}

void test_oversprintf() {
    printf("=== ТЕСТ OVERSPRINTF (вывод в строку) ===\n");
    
    char buffer[512];
    int len;
    
    len = oversprintf(buffer, "Римские: %Ro", 49);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Цекендорф 4: %Zr", 4);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "255 в 16-чной: %Cv", 255, 16);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "\"ff\" из 16-чной: %to", "ff", 16);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Комбо: %Ro, %Zr, %Cv", 20, 20, 255, 16);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Смешанный: число=%d, римское=%Ro, hex=0x%x", 100, 100, 100);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Дамп int 42: %mi", 42);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Дамп float 1.0: %mf", 1.0f);
    printf("%s (длина: %d)\n", buffer, len);
    
    printf("\n");
}

void test_edge_cases() {
    printf("=== ТЕСТ ГРАНИЧНЫХ СЛУЧАЕВ И ОШИБОК ===\n");
    
    char buffer[64];
    int result;
    
    printf("NULL указатели:\n");
    status_t status = int_to_roman(10, NULL, sizeof(buffer));
    printf("int_to_roman с NULL buffer: %s\n", status == NULL_POINTER ? "NULL_POINTER" : "ERROR");
    
    status = str_to_int_base("123", 10, 0, NULL);
    printf("str_to_int_base с NULL result: %s\n", status == NULL_POINTER ? "NULL_POINTER" : "ERROR");
    
    printf("\nНевалидные основания:\n");
    int_to_base(123, 1, 0, buffer, sizeof(buffer));
    printf("Основание 1: %s (должно быть 10)\n", buffer);
    
    int_to_base(123, 37, 0, buffer, sizeof(buffer));
    printf("Основание 37: %s (должно быть 10)\n", buffer);
    
    printf("\nПереполнение буфера:\n");
    char small_buffer[1];
    status = int_to_roman(10, small_buffer, sizeof(small_buffer));
    printf("Маленький буфер для римских: %s\n", status == BUFFER_OVERFLOW ? "BUFFER_OVERFLOW" : "ERROR");
    
    status = uint_to_zeckendorf(10, small_buffer, sizeof(small_buffer));
    printf("Маленький буфер для цекендорфа: %s\n", status == BUFFER_OVERFLOW ? "BUFFER_OVERFLOW" : "ERROR");
    
    printf("\nНевалидные строки:\n");
    status = str_to_int_base("", 10, 0, &result);
    printf("Пустая строка: %s\n", status == INVALID_NUMBER ? "INVALID_NUMBER" : "ERROR");
    
    status = str_to_int_base("12G", 10, 0, &result);
    printf("Невалидные символы: %s\n", status == INVALID_NUMBER ? "INVALID_NUMBER" : "ERROR");
    
    status = str_to_int_base("FF", 10, 0, &result);
    printf("Символы вне основания: %s\n", status == INVALID_NUMBER ? "INVALID_NUMBER" : "ERROR");
    
    printf("\n");
}

void test_extended_format_specifiers() {
    printf("=== ТЕСТ РАСШИРЕННЫХ СПЕЦИФИКАТОРОВ ФОРМАТА ===\n");
    
    char buffer[512];
    int len;
    
    printf("1. Спецификаторы с шириной и точностью:\n");
    len = oversprintf(buffer, "Число: |%8d|", 42);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Число: |%-8d|", 42);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Число: |%08d|", 42);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Дробное: |%8.2f|", 3.14159);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Строка: |%-10s|", "test");
    printf("%s (длина: %d)\n", buffer, len);
    
    printf("\n2. Спецификаторы с модификаторами типа:\n");
    len = oversprintf(buffer, "Long: %ld", 123456789L);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Long long: %lld", 123456789012LL);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Size_t: %zu", (size_t)1000);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Pointer: %p", buffer);
    printf("%s (длина: %d)\n", buffer, len);
    
    printf("\n3. Комбинации с пользовательскими спецификаторами:\n");
    len = oversprintf(buffer, "Римское с шириной: |%Ro| и hex: |%08X|", 49, 255);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Цекендорф: %Zr, число: %+d, дробное: %.2f", 13, 42, 3.14);
    printf("%s (длина: %d)\n", buffer, len);
    
    printf("\n4. Сложные спецификаторы:\n");
    len = oversprintf(buffer, "Научная нотация: %e", 1234567.89);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Шестнадцатеричный float: %a", 3.14);
    printf("%s (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Процент: 100%% завершено");
    printf("%s (длина: %d)\n", buffer, len);
    
    printf("\n");
}

void test_file_extended_formats() {
    printf("=== ТЕСТ РАСШИРЕННЫХ ФОРМАТОВ В ФАЙЛ ===\n");
    
    FILE *f = fopen("extended_test_output.txt", "w");
    if (f == NULL) {
        printf("Ошибка открытия файла!\n");
        return;
    }
    
    overfprintf(f, "=== ТЕСТ РАСШИРЕННЫХ СПЕЦИФИКАТОРОВ ===\n\n");
    
    overfprintf(f, "1. Простые спецификаторы:\n");
    overfprintf(f, "   Число: %d\n", 123);
    overfprintf(f, "   Строка: %s\n", "test");
    overfprintf(f, "   Дробное: %f\n", 3.14);
    
    overfprintf(f, "\n2. Спецификаторы с шириной:\n");
    overfprintf(f, "   |%5d|\n", 123);
    overfprintf(f, "   |%-5d|\n", 123);
    
    overfprintf(f, "\n3. С пользовательскими спецификаторами:\n");
    overfprintf(f, "   Римское: %Ro\n", 49);
    overfprintf(f, "   Цекендорф: %Zr\n", 20);
    overfprintf(f, "   Число: %d, система: %Cv\n", 42, 255, 16);
    
    overfprintf(f, "\n4. Смешанные форматы:\n");
    overfprintf(f, "   Комбинация: %Ro | %Zr | %d | %s\n", 
               2024, 13, 123, "text");
    
    fclose(f);
    printf("Результаты записаны в файл 'extended_test_output.txt'\n\n");
}

void test_edge_format_cases() {
    printf("=== ТЕСТ ГРАНИЧНЫХ СЛУЧАЕВ ФОРМАТА ===\n");
    
    char buffer[256];
    int len;
    
    printf("1. Пустые и специальные спецификаторы:\n");
    len = oversprintf(buffer, "Простой текст");
    printf("Без спецификаторов: '%s' (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Знак процента: 100%%");
    printf("Знак процента: '%s' (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Двойной процент: 100%%%%");
    printf("Двойной процент: '%s' (длина: %d)\n", buffer, len);
    
    printf("\n2. Спецификаторы на границах:\n");
    len = oversprintf(buffer, "Начало: %d текст", 123);
    printf("Спецификатор в начале: '%s' (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Текст в конце: %d", 456);
    printf("Спецификатор в конце: '%s' (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "%d", 789);
    printf("Только спецификатор: '%s' (длина: %d)\n", buffer, len);
    
    printf("\n3. Множественные спецификаторы:\n");
    len = oversprintf(buffer, "%d %s %f %Ro %Zr", 42, "test", 3.14, 49, 13);
    printf("Множественные: '%s' (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "%d%d%d%d", 1, 2, 3, 4);
    printf("Подряд: '%s' (длина: %d)\n", buffer, len);
    
    printf("\n4. Спецификаторы с разными разделителями:\n");
    len = oversprintf(buffer, "Число:%d\tСтрока:%s\nНовая строка", 123, "text");
    printf("С разделителями: '%s' (длина: %d)\n", buffer, len);
    
    printf("\n5. Длинные сложные спецификаторы:\n");
    len = oversprintf(buffer, "Формат: %+20.10f", 1234567.8912345);
    printf("Сложный float: '%s' (длина: %d)\n", buffer, len);
    
    len = oversprintf(buffer, "Широкий: %30s", "short");
    printf("Широкая строка: '%s' (длина: %d)\n", buffer, len);
    
    printf("\n");
}

int main() {
    printf("=== КОМПЛЕКСНОЕ ТЕСТИРОВАНИЕ ФУНКЦИЙ ===\n\n"); 
    test_roman_numerals();
    test_zeckendorf_correct_order();
    test_base_conversion();
    test_string_to_int();
    test_memory_dump();
    
    test_overfprintf();
    test_oversprintf();
    test_edge_cases();
    
    test_extended_format_specifiers();
    test_file_extended_formats();
    test_edge_format_cases();

    
    printf("=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===\n");
    
    return 0;
}