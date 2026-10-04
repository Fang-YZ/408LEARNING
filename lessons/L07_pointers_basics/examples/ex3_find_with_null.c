/* L7 example 3: return NULL when nothing is found, and check before dereferencing. */
#include <stdio.h>

int *find_first_negative(int *values, int count);

int main(void)
{
    int values[5] = {3, 5, -7, 9, 2};
    int *found = find_first_negative(values, 5);
    int *missing = find_first_negative(values, 2);

    if (found != NULL)
    {
        printf("first negative = %d, at index %td\n", *found, found - values);
    }

    printf("first 2 elements: %s\n", (missing == NULL) ? "not found" : "found");

    return 0;
}

int *find_first_negative(int *values, int count)
{
    for (int index = 0; index < count; index++)
    {
        if (values[index] < 0)
        {
            return &values[index];   /* an address inside the caller's array */
        }
    }

    return NULL;
}
