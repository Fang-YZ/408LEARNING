/* L4 example 2: pass by value - the callee only gets COPIES, so swap cannot touch main's variables. */
#include <stdio.h>

void swap_by_value(int left, int right);

int main(void)
{
    int a = 1;
    int b = 2;

    printf("before: a = %d, b = %d\n", a, b);
    swap_by_value(a, b);
    printf("after : a = %d, b = %d\n", a, b);

    return 0;
}

void swap_by_value(int left, int right)
{
    int temp = left;

    left = right;
    right = temp;

    printf("inside: left = %d, right = %d\n", left, right);
}
