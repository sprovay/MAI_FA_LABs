#include <stdio.h>
#include <assert.h>
#include "task_funcs.h"

void test_memchr() {
    printf("Testing memchr... ");
    
    char str[] = "Hello, World!";
    
    assert(memchr(str, 'W', 13) == str + 7);
    assert(memchr(str, 'o', 13) == str + 4);
    assert(memchr(str, 'x', 13) == NULL);
    assert(memchr(str, 'H', 0) == NULL);
    assert(memchr(str, '\0', 14) == str + 13);
    
    printf("PASS\n");
}

void test_memcmp() {
    printf("Testing memcmp... ");
    
    char str1[] = "Hello";
    char str2[] = "Hello";
    char str3[] = "Hellp";
    
    assert(memcmp(str1, str2, 5) == 0);
    assert(memcmp(str1, str3, 5) == -1);
    assert(memcmp(str3, str1, 5) == 1);
    assert(memcmp(str1, str3, 0) == 0);
    
    printf("PASS\n");
}

void test_memcpy() {
    printf("Testing memcpy... ");
    
    char src[] = "Hello";
    char dest[10];
    
    assert(memcpy(dest, src, 6) == dest);
    assert(strncmp(dest, "Hello", 6) == 0);
    assert(memcpy(dest, src, 0) == dest);
    
    printf("PASS\n");
}

void test_memset() {
    printf("Testing memset... ");
    
    char buffer[10] = "Hello";
    
    assert(memset(buffer, 'x', 5) == buffer);
    assert(buffer[0] == 'x');
    assert(buffer[4] == 'x');
    assert(buffer[5] == '\0');
    assert(memset(buffer, 'y', 0) == buffer);
    
    printf("PASS\n");
}

void test_strncat() {
    printf("Testing strncat... ");
    
    char dest[20] = "Hello";
    
    assert(strncat(dest, " World", 7) == dest);
    assert(strncmp(dest, "Hello World", 12) == 0);
    
    char dest2[20] = "Hello";
    strncat(dest2, " World!", 3);
    assert(strncmp(dest2, "Hello Wo", 9) == 0);
    
    printf("PASS\n");
}

void test_strchr() {
    printf("Testing strchr... ");
    
    char str[] = "Hello, World!";
    
    assert(strchr(str, 'W') == str + 7);
    assert(strchr(str, 'o') == str + 4);
    assert(strchr(str, 'x') == NULL);
    assert(strchr(str, '\0') == str + 13);
    
    printf("PASS\n");
}

void test_strncmp() {
    printf("Testing strncmp... ");
    
    char str1[] = "Hello";
    char str2[] = "Hello";
    char str3[] = "Hellp";
    
    assert(strncmp(str1, str2, 5) == 0);
    assert(strncmp(str1, str3, 5) == -1);
    assert(strncmp(str3, str1, 5) == 1);
    assert(strncmp(str1, str3, 4) == 0);
    assert(strncmp(str1, str3, 0) == 0);
    
    printf("PASS\n");
}

void test_strncpy() {
    printf("Testing strncpy... ");
    
    char dest[10];
    
    assert(strncpy(dest, "Hello", 6) == dest);
    assert(strncmp(dest, "Hello", 6) == 0);
    
    char dest2[10];
    strncpy(dest2, "Hi", 5);
    assert(dest2[0] == 'H');
    assert(dest2[1] == 'i');
    assert(dest2[2] == '\0');
    assert(dest2[3] == '\0');
    assert(dest2[4] == '\0');
    
    printf("PASS\n");
}

void test_strcspn() {
    printf("Testing strcspn... ");
    
    char str[] = "Hello123";
    char reject[] = "0123456789";
    
    assert(strcspn(str, reject) == 5);
    assert(strcspn(str, "xyz") == 8);
    assert(strcspn(str, "H") == 0);
    
    printf("PASS\n");
}

void test_strlen() {
    printf("Testing strlen... ");
    
    assert(strlen("Hello") == 5);
    assert(strlen("") == 0);
    assert(strlen("a") == 1);
    
    printf("PASS\n");
}

void test_strpbrk() {
    printf("Testing strpbrk... ");
    
    char str[] = "Hello, World!";
    char accept[] = " ,!";
    
    assert(strpbrk(str, accept) == str + 5);
    assert(strpbrk(str, "xyz") == NULL);
    assert(strpbrk(str, "H") == str);
    
    printf("PASS\n");
}

void test_strrchr() {
    printf("Testing strrchr... ");
    
    char str[] = "Hello, World! Hello!";
    
    assert(strrchr(str, 'o') == str + 18);
    assert(strrchr(str, 'H') == str + 14);
    assert(strrchr(str, 'x') == NULL);
    assert(strrchr(str, '\0') == str + 20);
    
    printf("PASS\n");
}

void test_strstr() {
    printf("Testing strstr... ");
    
    char haystack[] = "Hello, World! Hello!";
    
    assert(strstr(haystack, "World") == haystack + 7);
    assert(strstr(haystack, "Hello") == haystack);
    assert(strstr(haystack, "Earth") == NULL);
    assert(strstr(haystack, "") == haystack);
    
    printf("PASS\n");
}

void test_strtok() {
    printf("Testing strtok... ");
    
    char str1[] = "  Hello, World!;This is   a test";
    
    char* token = strtok(str1, " ,!;");
    assert(strncmp(token, "Hello", 6) == 0);
    
    token = strtok(NULL, " ,!;");
    assert(strncmp(token, "World", 6) == 0);
    
    token = strtok(NULL, " ,!;");
    assert(strncmp(token, "This", 5) == 0);
    
    token = strtok(NULL, " ,!;");
    assert(strncmp(token, "is", 3) == 0);
    
    token = strtok(NULL, " ,!;");
    assert(strncmp(token, "a", 2) == 0);
    
    token = strtok(NULL, " ,!;");
    assert(strncmp(token, "test", 5) == 0);
    
    token = strtok(NULL, " ,!;");
    assert(token == NULL);
    
    printf("PASS\n");
}

int main() {
    printf("Running all string function tests...\n\n");
    
    test_memchr();
    test_memcmp();
    test_memcpy();
    test_memset();
    test_strncat();
    test_strchr();
    test_strncmp();
    test_strncpy();
    test_strcspn();
    test_strlen();
    test_strpbrk();
    test_strrchr();
    test_strstr();
    test_strtok();
    
    printf("\nAll tests passed successfully!\n");

    printf("%s \n", strtok("", " ,"));
    return 0;
}