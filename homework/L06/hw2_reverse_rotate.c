#include <stdio.h>
const int maxn = 1e3 + 5;
int main(){

    int n;
    int a[maxn];
    int b[maxn];
    int c[maxn];
    int k;
    scanf("%d", &n);
    for(int i = 0; i < n; ++i){
        scanf("%d",&a[i]);
        b[i] = a[i];
    }
    scanf("%d", &k);
    int m = n - 1;
    for(int j = 0; j < n / 2; ++j){
            int temp = b[j];
            b[j] = b[m];
            b[m] = temp;
            --m;
    }
    printf("reversed:");
    for(int i = 0; i < n; ++i){
        printf(" %d", b[i]);
    }
    printf("\n");
    for(int i = 0 ; i < n; ++i){
        c[i] = a[(i + (n- (k % n)))% n];
    }
    printf("rotated:");
    for(int i = 0; i < n; ++i){
        printf(" %d", c[i]);
    }
    return 0;
}