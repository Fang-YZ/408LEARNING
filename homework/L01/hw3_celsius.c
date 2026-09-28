#include <stdio.h>

int main(void)
{
    double fahrenheit = 0.0;
    scanf("%lf", &fahrenheit);
    double celsius = (fahrenheit - 32) * 5 / 9;
    printf("%.2f F -> %.2f C\n", fahrenheit, celsius);
    return 0;
}