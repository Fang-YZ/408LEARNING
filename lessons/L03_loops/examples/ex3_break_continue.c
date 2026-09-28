/*
 * L03 Example 3 - break vs continue, and a first look at primality
 *
 * Compile: gcc -Wall -Wextra ex3_break_continue.c -o ex3_break_continue.exe
 * Run:     ./ex3_break_continue.exe
 *
 * What you learn here:
 *   1. continue = skip the rest of THIS round, go to the next round.
 *   2. break = leave the whole loop immediately.
 *   3. n is prime if no divisor from 2 to n/2 divides it.
 */

#include <stdio.h>

int main(void)
{
    int n = 0;
    int divisor = 0;

    printf("Enter n (>= 2): ");
    scanf("%d", &n);

    printf("Multiples of 3 skipped:");
    for (int i = 1; i <= n; i++)
    {
        if (i % 3 == 0)
        {
            continue;                 /* skip the rest of this round */
        }
        printf(" %d", i);
    }
    printf("\n");

    for (int d = 2; d <= n / 2; d++)
    {
        if (n % d == 0)
        {
            divisor = d;
            break;                    /* found a divisor: stop early */
        }
    }

    if (divisor == 0)
    {
        printf("%d is prime\n", n);
    }
    else
    {
        printf("%d is not prime, smallest divisor = %d\n", n, divisor);
    }

    return 0;
}
