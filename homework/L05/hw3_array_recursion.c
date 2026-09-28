#include <stdio.h>

#define MAX_COUNT 1000
int sum_range(const int values[], int count);
int max_range(const int values[], int count);

int main(void){
    int values[MAX_COUNT];
    int count;
    scanf("%d", &count);
    for (int index = 0; index < count; index++){
        scanf("%d", &values[index]);
    }
    printf("sum = %d, max = %d\n", sum_range(values, count), max_range(values, count));
    return 0;
}
int sum_range(const int values[], int count){
    if (count == 0)                 /* empty range: the sum of nothing is 0 */
    {
        return 0;
    }
    return values[count - 1] + sum_range(values, count - 1);
}

int max_range(const int values[], int count){
    if (count == 1) {
        return values[0];
    }
    int rest_max = max_range(values, count - 1);
    if (values[count - 1] > rest_max) {
        return values[count - 1];
    }
    return rest_max;
}
