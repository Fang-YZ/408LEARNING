/* L6 HW1 reference: read an array, then report sum / average / min / max in one pass. */
#include <stdio.h>

#define MAX_COUNT 1000

int main(void)
{
    int values[MAX_COUNT];
    int count;

    scanf("%d", &count);

    for (int index = 0; index < count; index++)
    {
        scanf("%d", &values[index]);
    }

    long long total = 0;                /* the sum of many big ints needs a wider type */
    int minimum = values[0];            /* start from a real element, never from 0 */
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

    printf("sum = %lld, average = %.2f\n", total, (double)total / count);
    printf("min = %d, max = %d\n", minimum, maximum);

    return 0;
}
