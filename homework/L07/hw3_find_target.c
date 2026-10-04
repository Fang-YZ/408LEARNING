#include <stdio.h>
int* find_first(int values[], int count, int target){
    for(int i = 0; i < count; ++i){
        if(values[i] == target){
            return &values[i];
        }
    }
    return NULL;
}
int main(){
    int n;
    scanf("%d", &n);
    int values[1005];
    for(int i = 0; i < n; ++i){
        scanf("%d", &values[i]);
    }
    int target;
    scanf("%d", &target);
    int *s = find_first(values, n, target);

    if(s == NULL) printf("not found");
else printf("found %d at index %d", target, s - values);
return 0;
}