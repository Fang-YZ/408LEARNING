/* L5 example 1: the three rules of recursion live inside one function. */
#include <stdio.h>

long long factorial(int n);

int main(void)
{
    for (int n = 0; n <= 5; n++)
    {
        printf("factorial(%d) = %lld\n", n, factorial(n));
    }

    return 0;
}

long long factorial(int n)
{
    if (n <= 1)                     /* rule 1: base case -- answer it directly */
    {
        return 1;
    }

    return n * factorial(n - 1);    /* rule 2: smaller input; rule 3: call myself */
}
