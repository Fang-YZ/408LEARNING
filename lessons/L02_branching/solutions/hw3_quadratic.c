/*
 * L02 HW3 reference solution - quadratic equation real roots
 *
 * Compile: gcc -Wall -Wextra hw3_quadratic.c -o hw3_quadratic.exe
 *          (on Linux add -lm)
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw3_quadratic.exe -Tests ..\..\..\tools\tests\hw3_quadratic.txt
 */

#include <math.h>
#include <stdio.h>

int main(void)
{
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    double d = 0.0;

    printf("Enter a b c: ");
    scanf("%lf %lf %lf", &a, &b, &c);

    d = b * b - 4.0 * a * c;

    if (fabs(d) < 1e-9)
    {
        printf("One real root: x = %.2f\n", -b / (2.0 * a));
    }
    else if (d > 0)
    {
        printf("x1 = %.2f, x2 = %.2f\n",
               (-b + sqrt(d)) / (2.0 * a),
               (-b - sqrt(d)) / (2.0 * a));
    }
    else
    {
        printf("No real roots\n");
    }

    return 0;
}
