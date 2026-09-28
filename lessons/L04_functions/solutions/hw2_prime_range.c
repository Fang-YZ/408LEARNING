/* L4 HW2 reference: is_prime() answers one question, count_primes() reuses it. */
#include <stdio.h>

int is_prime(int number);
int count_primes(int low, int high);

int main(void)
{
    int low;
    int high;

    scanf("%d %d", &low, &high);

    printf("primes in [%d, %d] = %d\n", low, high, count_primes(low, high));

    return 0;
}

int is_prime(int number)
{
    if (number < 2)
    {
        return 0;                       /* guard clause: 0 and 1 are not prime */
    }

    for (int divisor = 2; divisor * divisor <= number; divisor++)
    {
        if (number % divisor == 0)
        {
            return 0;
        }
    }

    return 1;
}

int count_primes(int low, int high)
{
    int count = 0;

    for (int value = low; value <= high; value++)
    {
        if (is_prime(value))
        {
            count++;
        }
    }

    return count;
}
