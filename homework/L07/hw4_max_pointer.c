#include <stdio.h>
void find_max(int values[], int count, int **result){
    *result = NULL;
    if(count <= 0){
        return;
    }
    int bi = 0;
    for(int i = 1; i < count; ++i){
        if(values[bi] < values[i]){
            bi = i;
        }
    }
    *result = &values[bi];
}

int main(){
    int values[1005];
    int n;
    int *res = NULL;
    scanf("%d", &n);
    for(int i = 0; i < n; ++i){
        scanf("%d", &values[i]);
    }
    find_max(values, n, &res);
    if(res == NULL){
        printf("empty array");
    }else   printf("max = %d, index = %d", *res, res - values);
    return 0;
}