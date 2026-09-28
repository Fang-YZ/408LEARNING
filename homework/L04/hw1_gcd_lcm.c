#include <stdio.h>
void swap(int* a, int* b);
int gcd(int a, int b);
long long lcm(int a, int b);
int main(){
    int a, b;
    scanf("%d %d",&a, &b);
    int flag = 0;
    if(a < b){
        flag = 1;
        swap(&a, &b);
    }
    int g = gcd(a, b);
    long long m = lcm(a, b);
    if(flag){
        swap(&a, &b);
    }
    printf("gcd(%d, %d) = %d\n", a, b, g);
    printf("lcm(%d, %d) = %lld", a, b, m);
    return 0;
}


int gcd (int a, int b){
    return b != 0? gcd(b, a % b) : a;
}

long long lcm(int a, int b){
    return (long long)a * (long long)b / gcd(a, b);
}

void swap(int* a, int* b){
    int temp = *a;
    *a =  *b;
    *b =  temp;
}