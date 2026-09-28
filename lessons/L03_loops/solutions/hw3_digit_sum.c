/*
 * L03 HW3 reference solution - digits and digit sum
 *
 * Compile: gcc -Wall -Wextra hw3_digit_sum.c -o hw3_digit_sum.exe
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw3_digit_sum.exe -Tests ..\..\..\tools\tests\hw3_digit_sum.txt
 */

#include <stdio.h>

int main(void)
{
    int n = 0;
    int digits = 0;
    int digit_sum = 0;

    scanf("%d", &n);

    if (n == 0)
    {
        digits = 1;             /* the number 0 has exactly one digit */
        digit_sum = 0;
    }
    else
    {
        while (n > 0)
        {
            digit_sum += n % 10;   /* take the last digit */
            n /= 10;               /* drop the last digit */
            digits++;
        }
    }

    printf("digits = %d, digit sum = %d\n", digits, digit_sum);

    return 0;
}
