/*
 * L02 HW2 reference solution - grade
 *
 * Compile: gcc -Wall -Wextra hw2_grade.c -o hw2_grade.exe
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw2_grade.exe -Tests ..\..\..\tools\tests\hw2_grade.txt
 */

#include <stdio.h>

int main(void)
{
    int score = 0;
    char grade = 'E';

    printf("Enter a score (0-100): ");
    scanf("%d", &score);

    if (score >= 90)
    {
        grade = 'A';
    }
    else if (score >= 80)
    {
        grade = 'B';
    }
    else if (score >= 70)
    {
        grade = 'C';
    }
    else if (score >= 60)
    {
        grade = 'D';
    }
    else
    {
        grade = 'E';
    }

    printf("%d -> %c\n", score, grade);

    return 0;
}
