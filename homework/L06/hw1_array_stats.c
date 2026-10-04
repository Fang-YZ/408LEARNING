#include <stdio.h>
const long long maxn = 0x3f3f3f3f3f3f3f3f;
const int NUM = 1e3 + 5;
int main(){
    long long a[NUM];
    int n;
    scanf("%d", &n);
    long long maxm = -maxn;
    long long minm = maxn;
    long long sum = 0;
    double average = 0.00;
    for(int i = 0; i < n; ++i){
        scanf("%lld", &a[i]);
        if(a[i] > maxm){
            maxm = a[i];
        }
        if(a[i] < minm){
            minm = a[i];
        }
        sum += a[i];
    }
    average = (double)sum / n;
    printf("sum = %lld, average = %.2f\n", sum, average);
    printf("min = %lld, max = %lld", minm, maxm);
    return 0;

}