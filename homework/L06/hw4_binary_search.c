#include <stdio.h>
int main(){
    int n;
    scanf("%d", &n);
    int a[1005];
    for(int i = 0; i < n; ++i){
        scanf("%d", &a[i]);
    }
    int k;
    scanf("%d", &k);
    int index = -1;
    int comparisons = 0;
    int left = 0;
    int right = n - 1;
    while(left <= right){
        comparisons++;
        int mid = (left + right) / 2;
        if(a[mid] == k){
            index = mid;
            break;
        } 
        if(a[mid] < k){
            left = mid + 1;
        }
        if(a[mid] > k){
            right = mid - 1;
        }
    }
    if(index == -1){
    printf("not found, comparisons = %d",comparisons);
    }
    else{
        printf("index = %d, comparisons = %d", index, comparisons);
    }
    return 0;
}