#include <stdio.h>
int main(){
    int x;
    scanf("%d", &x);
    int *p = &x;
    printf("*p = %d\n", *p);
    int *q = p;
    printf("address match: %s\n", (p == &x) ? "yes":"no");
    printf("q == p: %s\n",(q == p)?"yes":"no");
    * p = *p * 2;
    printf("after *p = *p * 2: x = %d", *p);
    return 0;
}