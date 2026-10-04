/* L6 HW3 reference: hand-written bubble sort, printing the array before and after. */
#include <stdio.h>

#define MAX_COUNT 1000

void print_values(const int values[], int count);
void bubble_sort(int values[], int count);

int main(void)
{
    int values[MAX_COUNT];
    int count;

    scanf("%d", &count);

    for (int index = 0; index < count; index++)
    {
        scanf("%d", &values[index]);
    }

    printf("before:");
    print_values(values, count);

    bubble_sort(values, count);         /* an array argument is a pointer: the caller's data changes */

    printf("after:");
    print_values(values, count);

    return 0;
}

void print_values(const int values[], int count)
{
    for (int index = 0; index < count; index++)
    {
        printf(" %d", values[index]);
    }

    printf("\n");
}

void bubble_sort(int values[], int count)
{
    for (int pass = 0; pass < count - 1; pass++)
    {
        int swapped = 0;

        for (int index = 0; index < count - 1 - pass; index++)
        {
            if (values[index] > values[index + 1])
            {
                int temp = values[index];
                values[index] = values[index + 1];
                values[index + 1] = temp;
                swapped = 1;
            }
        }

        if (swapped == 0)               /* a clean pass means it is already sorted */
        {
            break;
        }
    }
}
