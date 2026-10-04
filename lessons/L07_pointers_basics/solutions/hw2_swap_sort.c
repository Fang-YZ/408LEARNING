/* L7 HW2 reference: swap through pointers, then sort with that swap. */
#include <stdio.h>

#define MAX_COUNT 1005

void swap(int *a, int *b);
void selection_sort(int values[], int count);
void print_values(const int values[], int count);

int main(void)
{
    int a;
    int b;
    int values[MAX_COUNT];
    int count;

    scanf("%d %d", &a, &b);

    swap(&a, &b);                   /* pass addresses: the caller's a and b really change */
    printf("swap: a = %d, b = %d\n", a, b);

    scanf("%d", &count);

    for (int index = 0; index < count; index++)
    {
        scanf("%d", &values[index]);
    }

    selection_sort(values, count);  /* an array argument is a pointer: the caller's data changes */

    printf("after:");
    print_values(values, count);

    return 0;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(int values[], int count)
{
    for (int start = 0; start < count - 1; start++)
    {
        int min_index = start;

        for (int scan = start + 1; scan < count; scan++)
        {
            if (values[scan] < values[min_index])
            {
                min_index = scan;
            }
        }

        if (min_index != start)
        {
            swap(&values[start], &values[min_index]);   /* one swap per round, not many */
        }
    }
}

void print_values(const int values[], int count)
{
    for (int index = 0; index < count; index++)
    {
        printf(" %d", values[index]);
    }

    printf("\n");
}
