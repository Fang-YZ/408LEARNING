/* L6 HW4 reference (challenge): binary search on a sorted array, with a comparison counter. */
#include <stdio.h>

#define MAX_COUNT 1000

static int comparison_count = 0;

int binary_search(const int values[], int count, int target);

int main(void)
{
    int values[MAX_COUNT];
    int count;
    int target;

    scanf("%d", &count);

    for (int index = 0; index < count; index++)
    {
        scanf("%d", &values[index]);
    }

    scanf("%d", &target);

    int found = binary_search(values, count, target);

    if (found >= 0)
    {
        printf("index = %d, comparisons = %d\n", found, comparison_count);
    }
    else
    {
        printf("not found, comparisons = %d\n", comparison_count);
    }

    return 0;
}

int binary_search(const int values[], int count, int target)
{
    int low = 0;
    int high = count - 1;

    while (low <= high)                 /* the search range is [low, high] */
    {
        int middle = low + (high - low) / 2;   /* safe middle: (low + high) / 2 can overflow */

        comparison_count++;

        if (values[middle] == target)
        {
            return middle;              /* found: report the position */
        }

        if (values[middle] < target)
        {
            low = middle + 1;           /* throw away the left half */
        }
        else
        {
            high = middle - 1;          /* throw away the right half */
        }
    }

    return -1;                          /* -1 means "not found" */
}
