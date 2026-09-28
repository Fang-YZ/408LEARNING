#include <stdio.h>
int digit_sum(int);
long long reverse_number(int);
int main(){
    int n;
    scanf("%d", &n);
    int digitsum = digit_sum(n); 
    long long rn = reverse_number(n);
    printf("n = %d: digit sum = %d, reverse = %lld", n, digitsum, rn);
    return 0;
}

int digit_sum(int n){
    int digitsum = 0;
    while(n > 0){
        digitsum += n % 10;
        n = n / 10;
    }
    return digitsum;
}

long long reverse_number(int n){
    long long res = 0;
    while(n > 0){
        res = res * 10;
        res += n % 10;
        n = n / 10;
    }
    return res;
}