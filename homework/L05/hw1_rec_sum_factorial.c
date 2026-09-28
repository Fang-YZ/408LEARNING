#include <stdio.h>
long long sum_to(int n);
long long factorial(int n);

int main(void){
    int n;
    scanf("%d", &n);
    long long sumto =sum_to(n);
    long long fact = factorial(n);
    printf("sum(1..%d) = %lld\n", n, sumto);
    printf("%d! = %lld", n, fact);

    return 0;
}

long long sum_to(int n){
    if(n <= 0) return 0;
    return n + sum_to(n - 1);
}

long long factorial(int n){
    if (n <= 1)                     /* rule 1: base case -- answer it directly */
    {
        return 1;
    }

    return n * factorial(n - 1);    /* rule 2: smaller input; rule 3: call myself */
}
