/* L6 example 1: define, initialize, walk an array - and let sizeof count the elements. */
#include <stdio.h>

int main(void)
{
    int scores[5] = {90, 85, 77, 60, 98};                   /* full initialization */
    int zeros[4] = {0};                                     /* every element becomes 0 */
    int partial[5] = {1, 2, 3};                             /* the tail is filled with 0 */
    int count = (int)(sizeof(scores) / sizeof(scores[0]));  /* 5: computed, not hard-coded */

    printf("count = %d, first = %d, last = %d\n", count, scores[0], scores[count - 1]);

    printf("zeros  :");
    for (int index = 0; index < 4; index++)
    {
        printf(" %d", zeros[index]);
    }
    printf("\n");

    printf("partial:");
    for (int index = 0; index < 5; index++)
    {
        printf(" %d", partial[index]);
    }
    printf("\n");

    return 0;
}
