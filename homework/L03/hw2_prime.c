#include<stdio.h>
int main(){
    int n;
    scanf("%d", &n);
    int flag = 1;
    for (int i = 2; i * i <= n; ++i){
        if(n % i == 0){
            printf("%d is not prime, smallest divisor = %d", n, i);
            flag = 0;
            break;
        }
    }
    if(flag){
        printf("%d is prime", n);
    }
    return 0;
}



