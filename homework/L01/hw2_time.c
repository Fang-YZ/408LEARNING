#include <stdio.h>

int main(void)
{
    int total_seconds = 0;
    scanf("%d", &total_seconds);
    int h = total_seconds / 3600;
    int m = (total_seconds % 3600) / 60;
    int s = total_seconds % 60;
    printf("%d:%02d:%02d\n", h, m, s);
    return 0;
}