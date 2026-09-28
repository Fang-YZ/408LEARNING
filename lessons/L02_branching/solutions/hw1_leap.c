/*
 * L02 HW1 reference solution - leap year
 *
 * Compile: gcc -Wall -Wextra hw1_leap.c -o hw1_leap.exe
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw1_leap.exe -Tests ..\..\..\tools\tests\hw1_leap.txt
 */

#include <stdio.h>

int main(void)
{
    int year = 0;

    printf("Enter a year: ");
    scanf("%d", &year);

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        printf("year %d is a leap year\n", year);
    }
    else
    {
        printf("year %d is not a leap year\n", year);
    }

    return 0;
}
