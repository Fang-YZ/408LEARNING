/* L5 example 3: watch the call stack - every call prints on the way in and again on the way out. */
#include <stdio.h>

void print_indent(int depth);
void countdown(int n, int depth);

int main(void)
{
    countdown(3, 0);

    return 0;
}

void print_indent(int depth)
{
    for (int step = 0; step < depth; step++)
    {
        printf("..");
    }
}

void countdown(int n, int depth)
{
    print_indent(depth);
    printf("enter countdown(%d)\n", n);

    if (n > 0)
    {
        countdown(n - 1, depth + 1);    /* push: go one level deeper */
    }
    else
    {
        print_indent(depth);
        printf("base case: stop here\n");  /* bottom of the stack */
    }

    print_indent(depth);
    printf("leave countdown(%d)\n", n);     /* pop: come back up, one level at a time */
}
