/*
 * L02 HW4 reference solution (challenge) - simple calculator
 *
 * Compile: gcc -Wall -Wextra hw4_calculator.c -o hw4_calculator.exe
 * Test:    ..\..\..\tools\check.ps1 -Exe .\hw4_calculator.exe -Tests ..\..\..\tools\tests\hw4_calculator.txt
 */

#include <stdio.h>

int main(void)
{
    double a = 0.0;
    double b = 0.0;
    double result = 0.0;
    char op = '?';

    printf("Enter expression (a op b): ");
    scanf("%lf %c %lf", &a, &op, &b);

    switch (op)
    {
        case '+':
            result = a + b;
            printf("%.2f %c %.2f = %.2f\n", a, op, b, result);
            break;
        case '-':
            result = a - b;
            printf("%.2f %c %.2f = %.2f\n", a, op, b, result);
            break;
        case '*':
            result = a * b;
            printf("%.2f %c %.2f = %.2f\n", a, op, b, result);
            break;
        case '/':
            if (b == 0.0)
            {
                printf("Division by zero.\n");
            }
            else
            {
                result = a / b;
                printf("%.2f %c %.2f = %.2f\n", a, op, b, result);
            }
            break;
        default:
            printf("Unknown operator.\n");
            break;
    }

    return 0;
}
