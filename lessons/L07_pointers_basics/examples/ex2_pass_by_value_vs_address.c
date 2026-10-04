/* L7 example 2: pass by value changes only the copy; pass by address changes the original. */
#include <stdio.h>

void swap_wrong(int a, int b);       /* a and b are copies */
void swap_right(int *a, int *b);     /* a and b are addresses */

int main(void)
{
    int x = 1;
    int y = 2;

    swap_wrong(x, y);
    printf("after swap_wrong: x = %d, y = %d\n", x, y);

    swap_right(&x, &y);
    printf("after swap_right: x = %d, y = %d\n", x, y);

    return 0;
}

void swap_wrong(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_right(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
