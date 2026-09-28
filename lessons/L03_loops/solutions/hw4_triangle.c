/*
 * L03 HW4 reference solution (challenge) - isosceles triangle
 *
 * Row i (1..h): (h - i) spaces, then (2 * i - 1) stars.
 *
 * Compile: gcc -Wall -Wextra hw4_triangle.c -o hw4_triangle.exe
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw4_triangle.exe -Tests ..\..\..\tools\tests\hw4_triangle.txt
 */

#include <stdio.h>

int main(void)
{
    int h = 0;

    scanf("%d", &h);

    for (int row = 1; row <= h; row++)
    {
        for (int space = 1; space <= h - row; space++)
        {
            printf(" ");
        }
        for (int star = 1; star <= 2 * row - 1; star++)
        {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
