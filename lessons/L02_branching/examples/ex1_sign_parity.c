/*
 * L02 Example 1 - if / else if / else: sign and parity
 *
 * Compile: gcc -Wall -Wextra ex1_sign_parity.c -o ex1_sign_parity.exe
 * Run:     ./ex1_sign_parity.exe
 *
 * What you learn here:
 *   1. if / else if / else chains; braces are never omitted.
 *   2. n % 2 can be -1, 0, or 1: never test (n % 2 == 1) for oddness.
 */

#include <stdio.h>

int main(void)
{
    int n = 0;

    printf("Enter an integer: ");
    scanf("%d", &n);

    if (n > 0)
    {
        printf("%d is positive\n", n);
    }
    else if (n < 0)
    {
        printf("%d is negative\n", n);
    }
    else
    {
        printf("%d is zero\n", n);
    }

    if (n % 2 == 0)
    {
        printf("%d is even\n", n);
    }
    else
    {
        printf("%d is odd\n", n);
    }

    return 0;
}
