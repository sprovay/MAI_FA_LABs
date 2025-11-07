#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

void test_overfscanf() {
    printf("=== Testing overfscanf ===\n");
    
    FILE *test_file = fopen("test_input.txt", "w+");
    if (!test_file) {
        printf("Failed to create test file\n");
        return;
    }
    
    fprintf(test_file, "XIV 42 1011 1A Hello\n");
    fprintf(test_file, "XLII 100 10011 ff World\n");
    
    rewind(test_file);
    
    int roman_val, int_val, base_val1, base_val2;
    unsigned int zeck_val;
    char str_val[20];
    
    int count1 = overfscanf(test_file, "%Ro %d %Zr %Cv %s", 
                           &roman_val, &int_val, &zeck_val, &base_val1, 16, str_val);
    
    printf("Read %d values:\n", count1);
    printf("  Roman: %d (expected: 14)\n", roman_val);
    printf("  Integer: %d (expected: 42)\n", int_val);
    printf("  Zeckendorf: %u (expected: 4)\n", zeck_val);
    printf("  Base16: %d (expected: 26)\n", base_val1);
    printf("  String: %s (expected: Hello)\n", str_val);
    
    int count2 = overfscanf(test_file, "%Ro %d %Zr %Cv %s", 
                           &roman_val, &int_val, &zeck_val, &base_val2, 16, str_val);
    
    printf("\nRead %d values:\n", count2);
    printf("  Roman: %d (expected: 42)\n", roman_val);
    printf("  Integer: %d (expected: 100)\n", int_val);
    printf("  Zeckendorf: %u (expected: 6)\n", zeck_val);
    printf("  Base16: %d (expected: 255)\n", base_val2);
    printf("  String: %s (expected: World)\n", str_val);
    
    fclose(test_file);
}

void test_oversscanf() {
    printf("\n=== Testing oversscanf ===\n");
    
    // Тест 1: Смешанные кастомные и стандартные спецификаторы
    printf("Test 1: Mixed custom and standard specifiers\n");
    const char *test1 = "XIV 42 10101 1A Hello";
    int a, b, c;
    unsigned int d;
    char str[20];
    
    int count = oversscanf(test1, "%Ro %d %Zr %Cv %s", 
                          &a, &b, &d, &c, 16, str);
    
    printf("Input: \"%s\"\n", test1);
    printf("Format: %%Ro %%d %%Zr %%Cv %%s\n");
    printf("Result: count=%d, Roman=%d, int=%d, Zeckendorf=%u, Base16=%d, str=%s\n",
           count, a, b, d, c, str);
    printf("Expected: count=5, Roman=14, int=42, Zeckendorf=3, Base16=26, str=Hello\n\n");
    
    // Тест 2: Только кастомные спецификаторы
    printf("Test 2: Only custom specifiers\n");
    const char *test2 = "XLII 1001 FF";
    int count2 = oversscanf(test2, "%Ro %Zr %CV", &a, &d, &c, 16);
    
    printf("Input: \"%s\"\n", test2);
    printf("Format: %%Ro %%Zr %%CV\n");
    printf("Result: count=%d, Roman=%d, Zeckendorf=%u, Base16=%d\n",
           count2, a, d, c);
    printf("Expected: count=3, Roman=42, Zeckendorf=5, Base16=255\n\n");
    
    // Тест 3: С пробелами
    printf("Test 3: With whitespace\n");
    const char *test3 = "   CML   101001  2f  ";
    int count3 = oversscanf(test3, "%Ro %Zr %Cv", &a, &d, &c, 16);
    
    printf("Input: \"%s\"\n", test3);
    printf("Format: %%Ro %%Zr %%Cv\n");
    printf("Result: count=%d, Roman=%d, Zeckendorf=%u, Base16=%d\n",
           count3, a, d, c);
    printf("Expected: count=3, Roman=950, Zeckendorf=8, Base16=47\n");
}

void test_parsing_functions() {
    printf("\n=== Testing Parsing Functions ===\n");
    
    // Тестирование roman_to_int
    int roman_result;
    status_t status = roman_to_int("XIV", &roman_result);
    printf("Roman 'XIV': %d, status: %s\n", roman_result, 
           status == SUCCESS ? "SUCCESS" : "FAIL");
    
    // Тестирование zeckendorf_to_uint
    unsigned int zeck_result;
    status = zeckendorf_to_uint("1011", &zeck_result);
    printf("Zeckendorf '1011': %u, status: %s\n", zeck_result,
           status == SUCCESS ? "SUCCESS" : "FAIL");
    
    // Тестирование str_to_int_base
    int base_result;
    status = str_to_int_base("1A", 16, 0, &base_result);
    printf("Base16 '1A': %d, status: %s\n", base_result,
           status == SUCCESS ? "SUCCESS" : "FAIL");
}

int main() {
    test_overfscanf();
    test_oversscanf();
    test_parsing_functions();
    return 0;
}