#include <stdio.h>
long long fib(int);
long long factorial(int);
int main(){
    int n;
    scanf("%d", &n);
    long long fibs = fib(n);
    long long fac = factorial(n);
    printf("fib(%d) = %lld\n%d! = %lld", n, fibs, n, fac);
}

long long fib(int n){
    if(n <= 0) return 0;
    if(n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

long long factorial(int n){
    long long res = 1;

    if(n == 0 || n == 1) return 1;
    for(int i = 2; i <= n; ++i){
        res *= i;
    }
    return res;
}