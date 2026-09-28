/* L5 HW1 reference: recursion for the sum 1..n and for n!. */
#include <stdio.h>

long long sum_to(int n);
long long factorial(int n);

int main(void)
{
    int n;

    scanf("%d", &n);

    printf("sum(1..%d) = %lld\n", n, sum_to(n));
    printf("%d! = %lld\n", n, factorial(n));

    return 0;
}

long long sum_to(int n)
{
    if (n <= 0)                     /* the empty sum: also catches negatives */
    {
        return 0;
    }

    return n + sum_to(n - 1);
}

long long factorial(int n)
{
    if (n <= 1)                     /* 0! = 1 and 1! = 1 share this base case */
    {
        return 1;
    }

    return n * factorial(n - 1);
}
