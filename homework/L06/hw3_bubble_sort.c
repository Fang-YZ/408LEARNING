#include <stdio.h>
const int maxn = 1005;
int main(){
    int n;
    scanf("%d", &n);
    int a[maxn];
    for(int i = 0 ; i < n; ++i){
        scanf("%d", &a[i]);
    }
    printf("before:");
    for(int i = 0; i < n; ++i){
        printf(" %d",a[i]);
    }
    printf("\n");
    for(int i = 0; i < n; ++i){
        int flag = 0;
        for(int j = i + 1; j < n; ++j){
            if(a[i] > a[j]){
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
                flag = 1;
            }
}
            if(flag == 0) break;
        
    }
    printf("after:");
    for(int i = 0; i < n; ++i){
        printf(" %d", a[i]);
    }
    return 0;
}