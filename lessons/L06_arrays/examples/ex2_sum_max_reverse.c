/* L6 example 2: one pass for sum / min / max / average, then reverse in place with two indexes. */
#include <stdio.h>

void print_values(const int values[], int count);

int main(void)
{
    int values[] = {12, 5, 27, 8, 19, 3};
    int count = (int)(sizeof(values) / sizeof(values[0]));
    int total = 0;
    int minimum = values[0];            /* start from a real element, not from 0 */
    int maximum = values[0];

    for (int index = 0; index < count; index++)
    {
        total += values[index];

        if (values[index] < minimum)
        {
            minimum = values[index];
        }

        if (values[index] > maximum)
        {
            maximum = values[index];
        }
    }

    printf("sum = %d, average = %.2f\n", total, (double)total / count);
    printf("min = %d, max = %d\n", minimum, maximum);

    for (int left = 0, right = count - 1; left < right; left++, right--)
    {
        int temp = values[left];        /* swap the two ends, then move inward */
        values[left] = values[right];
        values[right] = temp;
    }

    printf("reversed:");
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
