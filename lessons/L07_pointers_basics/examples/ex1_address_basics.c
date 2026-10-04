/* L7 example 1: one block of memory, two names -- and how wide a pointer is. */
#include <stdio.h>

int main(void)
{
    int score = 90;
    int *p = &score;                 /* p stores the address of score */
    int *q = p;                      /* q stores the same address */

    printf("&score = %p, p = %p, *p = %d\n", (void *)&score, (void *)p, *p);
    printf("p == q ? %s\n", (p == q) ? "yes" : "no");

    *p = 100;                        /* write through the pointer */
    printf("after *p = 100: score = %d, *q = %d\n", score, *q);

    printf("sizeof(int) = %zu, sizeof(int *) = %zu, sizeof(int[5]) = %zu\n",
           sizeof(int), sizeof(int *), sizeof(int[5]));

    return 0;
}
