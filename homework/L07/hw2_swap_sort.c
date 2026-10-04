#include<stdio.h>
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void SelectionSort(int values[], int n){
    for(int i = 0; i < n - 1; ++i){
        int mini = i;
        for(int j = i + 1; j < n; ++j){
            if(values[mini] > values[j]){
                mini = j;
            }
        }
        if(mini != i){
            swap(&values[i], &values[mini]);
        }
    }
}
int main(){
    int a,b;
    int values[1005];
    scanf("%d %d", &a, &b);
    swap(&a,&b);
    printf("swap: a = %d, b = %d\n", a, b);
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        scanf("%d", &values[i]);
    }
    SelectionSort(values, n);
    printf("after:");
    for(int i = 0; i < n; ++i){
        printf(" %d", values[i]);
    }
    return 0;
}