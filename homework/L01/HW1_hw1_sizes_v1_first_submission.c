#include<stdio.h>

int main(void)
{
    printf("Type Size Report\n");
    printf("char        %d\n", (int)sizeof(char));
    printf("short       %d\n", (int)sizeof(short));
    printf("int         %d\n", (int)sizeof(int));
    printf("long        %d\n", (int)sizeof(long));
    printf("long long   %d\n", (int)sizeof(long long));
    printf("float       %d\n", (int)sizeof(float));
    printf("double      %d\n", (int)sizeof(double));
    printf("End of Report\n");
    return 0;
}
