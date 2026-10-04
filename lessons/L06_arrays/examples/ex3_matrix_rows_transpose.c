/* L6 example 3: a 2D array is rows of rows - walk it by row, then transpose by swapping the loops. */
#include <stdio.h>

int main(void)
{
    int grid[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    for (int row = 0; row < 3; row++)           /* walk row by row */
    {
        int row_sum = 0;

        for (int col = 0; col < 4; col++)
        {
            row_sum += grid[row][col];
        }

        printf("row %d sum = %d\n", row, row_sum);
    }

    printf("transpose:\n");
    for (int col = 0; col < 4; col++)           /* transpose: just swap the two loops */
    {
        for (int row = 0; row < 3; row++)
        {
            printf("%4d", grid[row][col]);
        }
        printf("\n");
    }

    return 0;
}
