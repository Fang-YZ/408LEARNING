/* L5 example 2: naive Fibonacci is short, but the number of calls explodes. */
#include <stdio.h>

long long fib(int n);
long long fib_take_calls(void);

static long long call_count = 0;    /* counts how many times fib() runs */

int main(void){
    int n;
    scanf("%d", &n);
    long long value = fib(n);
    printf("fib(%d) = %lld, calls = %lld\n", n, value, fib_take_calls());
    return 0;
}

long long fib(int n){
    call_count++;
    if (n <= 1){
        return n;                   /* fib(0) = 0, fib(1) = 1 */
    }
    return fib(n - 1) + fib(n - 2); 
}

long long fib_take_calls(){
    long long value = call_count;
    call_count = 0;       
    return value;
}
