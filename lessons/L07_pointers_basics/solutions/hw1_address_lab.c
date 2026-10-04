/* L7 HW1 reference: watch an address, a value and a dereference - then write through the pointer. */
#include <stdio.h>

int main(void)
{
    int x;

    scanf("%d", &x);

    int *p = &x;                    /* p holds the address of x */
    int *q = p;                     /* q holds the same address */

    printf("*p = %d\n", *p);
    printf("address match: %s\n", (p == &x) ? "yes" : "no");
    printf("q == p: %s\n", (q == p) ? "yes" : "no");

    *p = *p * 2;                    /* write through the pointer: x itself changes */

    printf("after *p = *p * 2: x = %d\n", x);

    return 0;
}
