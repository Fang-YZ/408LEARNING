#include <stdio.h>
int main(){
    int n;
    scanf("%d", &n);
    int digits = 0;
    int digit_sum = 0;
    if(n == 0){
        digits = 1;
    }
    while(n > 0){
        digits++;
        digit_sum += n % 10;
        n = n / 10;
    }
    
    printf("digits = %d, digit sum = %d", digits, digit_sum);
    return 0;
}