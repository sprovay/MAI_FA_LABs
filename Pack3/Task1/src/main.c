#include "../include/funcs.h"

int main() {
    char result[34];
    int number = -2098;

    printf("number = %d\n", number);

    printf("r = 1: ");
    num_to_base(number, 1, result);
    printf("%s\n", result);

    printf("r = 2: ");
    num_to_base(number, 2, result);
    printf("%s\n", result);
    
    printf("r = 3: ");
    num_to_base(number, 3, result);
    printf("%s\n", result);
    
    printf("r = 4: ");
    num_to_base(number, 4, result);
    printf("%s\n", result);
    
    printf("r = 5: ");
    num_to_base(number, 5, result);
    printf("%s\n", result);

    return 0;
}