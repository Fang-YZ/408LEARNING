#include <stdio.h>
int is_prime(int n);
int count_primes(int low, int high);
int main(){
    int low, high;
    scanf("%d %d", &low, &high);
    int res = count_primes(low, high);
    printf("primes in [%d, %d] = %d", low, high, res);
    return 0;
}

int is_prime(int n){
    if(n == 0 || n == 1){
        return 0;
    }
    for(int i = 2 ; i * i <= n; ++i){//除数从2开始
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}
int count_primes(int low, int high){
    int cnt = 0;
    for(int j = low; j <= high; ++j){
        cnt += is_prime(j);
    }
    return cnt;
}