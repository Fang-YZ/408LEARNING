/* L7 HW4 reference (challenge): let the callee change the CALLER'S POINTER - int **. */
#include <stdio.h>

#define MAX_COUNT 1005

void find_max(int values[], int count, int **result);

int main(void)
{
    int values[MAX_COUNT];
    int count;
    int *best = NULL;

    scanf("%d", &count);

    for (int index = 0; index < count; index++)
    {
        scanf("%d", &values[index]);
    }

    find_max(values, count, &best);     /* pass the ADDRESS OF THE POINTER */

    if (best == NULL)
    {
        printf("empty array\n");
    }
    else
    {
        printf("max = %d, index = %td\n", *best, best - values);
    }

    return 0;
}

void find_max(int values[], int count, int **result)
{
    *result = NULL;                     /* wide exit: assume there is nothing to point at */

    if (count <= 0)
    {
        return;
    }

    int best_index = 0;

    for (int index = 1; index < count; index++)
    {
        if (values[index] > values[best_index])
        {
            best_index = index;
        }
    }

    *result = &values[best_index];      /* write the address into the CALLER'S pointer */
}
