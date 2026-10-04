/* L7 HW3 reference: return an address when found, NULL when not - and check before use. */
#include <stdio.h>

#define MAX_COUNT 1005

int *find_first(int values[], int count, int target);

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

    int *found = find_first(values, count, target);

    if (found == NULL)
    {
        printf("not found\n");
    }
    else
    {
        printf("found %d at index %td\n", *found, found - values);   /* pointer subtraction */
    }

    return 0;
}

int *find_first(int values[], int count, int target)
{
    for (int index = 0; index < count; index++)
    {
        if (values[index] == target)
        {
            return &values[index];      /* an address inside the caller's array */
        }
    }

    return NULL;                        /* nothing found: the caller must check this */
}
