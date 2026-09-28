/* L4 example 1: the three pieces of a function - prototype, definition, call. */
#include <stdio.h>

int gcd(int first, int second);         /* prototype: every call is checked against it */

int main(void)
{
    int a = 1071;
    int b = 462;
    int result = gcd(a, b);             /* call: arguments are COPIED into the parameters */

    printf("gcd(%d, %d) = %d\n", a, b, result);

    return 0;
}

int gcd(int first, int second)          /* definition: the real body lives here */
{
    while (second != 0)
    {
        int remainder = first % second;

        first = second;
        second = remainder;
    }

    return first;
}
