/*
 * L03 HW2 reference solution - primality test with early break
 *
 * Compile: gcc -Wall -Wextra hw2_prime.c -o hw2_prime.exe
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw2_prime.exe -Tests ..\..\..\tools\tests\hw2_prime.txt
 */

#include <stdio.h>

int main(void)
{
    int n = 0;
    int divisor = 0;

    scanf("%d", &n);

    for (int d = 2; d <= n / 2; d++)
    {
        if (n % d == 0)
        {
            divisor = d;
            break;              /* a divisor proves it is not prime: stop */
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
