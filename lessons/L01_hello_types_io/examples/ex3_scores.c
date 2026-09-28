/*
 * L01 Example 3 - scanf() input, arithmetic, printf() formatting
 *
 * Compile: gcc -Wall -Wextra ex3_scores.c -o ex3_scores.exe
 * Run:     ./ex3_scores.exe
 *
 * What you learn here:
 *   1. scanf needs & (address of); printf does not.
 *   2. scanf reads a double with %lf, printf prints a double with %f.
 *   3. int / int truncates; keep one side double to keep decimals.
 */

#include <stdio.h>

int main(void)
{
    double math_score = 0.0;
    double english_score = 0.0;
    double c_score = 0.0;
    double total = 0.0;
    double average = 0.0;

    printf("Enter three scores separated by spaces: ");
    scanf("%lf %lf %lf", &math_score, &english_score, &c_score);

    total = math_score + english_score + c_score;
    average = total / 3.0;    /* total is double, but write 3.0 to stay obvious */

    printf("Total   = %.1f\n", total);
    printf("Average = %.1f\n", average);

    return 0;
}
