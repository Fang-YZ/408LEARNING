/* L4 HW4 reference (challenge): the loop versions of fib and factorial, wrapped as functions. */
#include <stdio.h>

long long factorial(int n);
long long fib(int n);

int main(void)
{
    int n;

    scanf("%d", &n);

    printf("fib(%d) = %lld\n", n, fib(n));
    printf("%d! = %lld\n", n, factorial(n));

    return 0;
}

long long factorial(int n)
{
    long long product = 1;               /* 0! = 1 falls out of this start value */

    for (int factor = 2; factor <= n; factor++)
    {
        product *= factor;
    }

    return product;
}

long long fib(int n)
{
    long long previous = 0;              /* fib(0) */
    long long current = 1;               /* fib(1) */

    for (int step = 0; step < n; step++)
    {
        long long next = previous + current;

        previous = current;
        current = next;
    }

    return previous;
}
