/*
 * L01 Example 1 - Hello program (printf basics)
 *
 * Compile: gcc -Wall -Wextra ex1_hello.c -o ex1_hello.exe
 * Run:     ./ex1_hello.exe
 *
 * What you learn here:
 *   1. main() is the entry point of every C program.
 *   2. printf() prints text; \n moves to a new line.
 *   3. Every statement ends with a semicolon ;.
 */

#include <stdio.h>

int main(void)
{
    printf("Hello, 408!\n");
    printf("My name is Zain Fong.\n");   /* TODO: change this to your own name */
    printf("I am learning C for Data Structures.\n");
    printf("Line 1\nLine 2\nLine 3\n");

    return 0;
}
