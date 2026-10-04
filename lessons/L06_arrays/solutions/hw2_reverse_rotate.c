/* L6 HW2 reference: reverse in place, then rotate right by k positions. */
#include <stdio.h>

#define MAX_COUNT 1000

void print_values(const int values[], int count);

int main(void)
{
    int values[MAX_COUNT];
    int rotated[MAX_COUNT];
    int count;
    int shift;

    scanf("%d", &count);

    for (int index = 0; index < count; index++)
    {
        scanf("%d", &values[index]);
    }

    scanf("%d", &shift);

    int step = shift % count;           /* k bigger than count must wrap around */

    for (int index = 0; index < count; index++)
    {
        rotated[(index + step) % count] = values[index];
    }

    for (int left = 0, right = count - 1; left < right; left++, right--)
    {
        int temp = values[left];        /* reverse the ORIGINAL array in place */
        values[left] = values[right];
        values[right] = temp;
    }

    printf("reversed:");
    print_values(values, count);

    printf("rotated:");
    print_values(rotated, count);

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
