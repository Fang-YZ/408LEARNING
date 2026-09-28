/*
 * L02 Example 2 - logical operators && || and short-circuit evaluation
 *
 * Compile: gcc -Wall -Wextra ex2_range_shortcircuit.c -o ex2_range_shortcircuit.exe
 * Run:     ./ex2_range_shortcircuit.exe
 *
 * What you learn here:
 *   1. A range like 10 <= x <= 20 must be written as (x >= 10 && x <= 20).
 *   2. && and || evaluate left to right and STOP as soon as the result is known.
 *   3. Short-circuit protects you: (b != 0 && a / b > 1) never divides by zero.
 */

#include <stdio.h>

int main(void)
{
    int x = 0;
    int a = 0;
    int b = 0;

    printf("Enter an integer x: ");
    scanf("%d", &x);

    if (x >= 10 && x <= 20)
    {
        printf("%d is inside [10, 20]\n", x);
    }
    else
    {
        printf("%d is outside [10, 20]\n", x);
    }

    printf("Enter two integers a b (a will be divided by b): ");
    scanf("%d %d", &a, &b);

    if (b != 0 && a / b > 1)
    {
        printf("a / b = %d, which is greater than 1\n", a / b);
    }
    else
    {
        printf("No comparison made: b == 0, or a / b <= 1\n");
    }

    return 0;
}
