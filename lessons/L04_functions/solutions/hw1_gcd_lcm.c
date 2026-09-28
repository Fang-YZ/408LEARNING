/* L4 HW1 reference: greatest common divisor and least common multiple, one job per function. */
#include <stdio.h>

int gcd(int first, int second);
long long lcm(int first, int second);

int main(void)
{
    int a;
    int b;

    scanf("%d %d", &a, &b);

    printf("gcd(%d, %d) = %d\n", a, b, gcd(a, b));
    printf("lcm(%d, %d) = %lld\n", a, b, lcm(a, b));

    return 0;
}

int gcd(int first, int second)
{
    while (second != 0)
    {
        int remainder = first % second;

        first = second;
        second = remainder;
    }

    return first;
}

long long lcm(int first, int second)
{
    /* Cast BEFORE the multiply: first * second can overflow int even when the lcm fits. */
    long long product = (long long)first * second;

    return product / gcd(first, second);
}
