/*
 * L02 Example 3 - switch / case / break / default
 *
 * Compile: gcc -Wall -Wextra ex3_switch_weekday.c -o ex3_switch_weekday.exe
 * Run:     ./ex3_switch_weekday.exe
 *
 * What you learn here:
 *   1. switch jumps to the matching case, then FALLS THROUGH without break.
 *   2. Grouping cases: case 6: case 7: share the same body.
 *   3. default handles everything else.
 */

#include <stdio.h>

int main(void)
{
    int day = 0;

    printf("Enter a day number (1-7): ");
    scanf("%d", &day);

    switch (day)
    {
        case 1:
            printf("Monday\n");
            break;
        case 2:
            printf("Tuesday\n");
            break;
        case 3:
            printf("Wednesday\n");
            break;
        case 4:
            printf("Thursday\n");
            break;
        case 5:
            printf("Friday\n");
            break;
        case 6:
        case 7:
            printf("Weekend!\n");
            break;
        default:
            printf("Invalid day number (must be 1-7)\n");
            break;
    }

    return 0;
}
