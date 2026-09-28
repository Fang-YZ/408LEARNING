/*
 * L03 HW1 reference solution - factorial n! (n: 1..20)
 *
 * Compile (MinGW needs the ANSI stdio recipe for %lld):
 *   gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 hw1_factorial.c -o hw1_factorial.exe
 * Test:
 *   ..\..\..\tools\check.ps1 -Exe .\hw1_factorial.exe -Tests ..\..\..\tools\tests\hw1_factorial.txt
 */

#include <stdio.h>

int main(void)
{
    int n = 0;
    long long fact = 1;

    scanf("%d", &n);

    for (int i = 1; i <= n; i++)
    {
        fact *= i;              /* fact starts at 1, not 0 */
    }

    printf("%d! = %lld\n", n, fact);

    return 0;
}
