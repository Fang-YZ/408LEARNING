/* L4 HW3 reference: split "digit sum" and "reverse number" into two small functions. */
#include <stdio.h>

int digit_sum(int number);
long long reverse_number(int number);

int main(void)
{
    int n;

    scanf("%d", &n);

    printf("n = %d: digit sum = %d, reverse = %lld\n", n, digit_sum(n), reverse_number(n));

    return 0;
}

int digit_sum(int number)
{
    int sum = 0;

    while (number > 0)
    {
        sum += number % 10;
        number /= 10;
    }

    return sum;
}

long long reverse_number(int number)
{
    long long reversed = 0;

    while (number > 0)
    {
        reversed = reversed * 10 + number % 10;
        number /= 10;
    }

    return reversed;
}
