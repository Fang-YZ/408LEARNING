/*
 * L01 Example 2 - Variables, basic types, sizeof()
 *
 * Compile: gcc -Wall -Wextra ex2_types.c -o ex2_types.exe
 * Run:     ./ex2_types.exe
 *
 * What you learn here:
 *   1. A variable = type + name; declare it before you use it.
 *   2. Initialize a variable when you declare it (never leave it dirty).
 *   3. sizeof(type) returns how many bytes the type occupies.
 */

#include <stdio.h>

int main(void)
{
    int age = 21;              /* integer number */
    double height = 1.75;      /* real number with decimals */
    char grade = 'A';          /* one single character, single quotes */

    printf("age    = %d\n", age);
    printf("height = %.2f\n", height);
    printf("grade  = %c\n", grade);

    printf("\nSize of each type in bytes (this machine):\n");
    printf("char      : %d\n", (int)sizeof(char));
    printf("short     : %d\n", (int)sizeof(short));
    printf("int       : %d\n", (int)sizeof(int));
    printf("long      : %d\n", (int)sizeof(long));
    printf("long long : %d\n", (int)sizeof(long long));
    printf("float     : %d\n", (int)sizeof(float));
    printf("double    : %d\n", (int)sizeof(double));

    return 0;
}
