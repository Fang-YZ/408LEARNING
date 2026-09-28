/*
 * L03 Example 2 - nested loops: right triangle of stars
 *
 * Compile: gcc -Wall -Wextra ex2_stars_triangle.c -o ex2_stars_triangle.exe
 * Run:     ./ex2_stars_triangle.exe
 *
 * What you learn here:
 *   1. Outer loop controls the ROW, inner loop controls the COLUMN.
 *   2. For height h, row i prints exactly i stars.
 */

#include <stdio.h>

int main(void)
{
    int h = 0;

    printf("Enter height: ");
    scanf("%d", &h);

    for (int i = 1; i <= h; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
