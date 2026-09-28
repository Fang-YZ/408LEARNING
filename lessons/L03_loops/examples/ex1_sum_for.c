/*
 * L03 Example 1 - for loop: sum 1..n
 *
 * Compile: gcc -Wall -Wextra ex1_sum_for.c -o ex1_sum_for.exe
 * Run:     ./ex1_sum_for.exe
 *
 * What you learn here:
 *   1. A loop needs three parts: start value, stop condition, step.
 *   2. for (int i = 1; i <= n; i++) is the standard counting loop.
 *   3. Boundary check: n = 0 must give sum = 0.
 */

#include <stdio.h>

int main(void)
{
    int n = 0;
    int sum = 0;

    printf("Enter n (0-1000): ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }

    printf("sum 1..%d = %d\n", n, sum);

    return 0;
}
