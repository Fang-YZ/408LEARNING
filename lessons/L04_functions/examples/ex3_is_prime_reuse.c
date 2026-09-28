/* L4 example 3: one function does one job - is_prime() is REUSED by count_primes(). */
#include <stdio.h>

int is_prime(int number);
int count_primes(int low, int high);

int main(void)
{
    printf("is_prime(97) = %d\n", is_prime(97));
    printf("is_prime(1)  = %d\n", is_prime(1));
    printf("primes in [2, 100] = %d\n", count_primes(2, 100));

    return 0;
}

int is_prime(int number)
{
    if (number < 2)
    {
        return 0;                       /* 0, 1 and negatives are not prime */
    }

    for (int divisor = 2; divisor * divisor <= number; divisor++)
    {
        if (number % divisor == 0)
        {
            return 0;                   /* early return: one divisor is enough */
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
            count++;                    /* reuse the test instead of rewriting it */
        }
    }

    return count;
}
