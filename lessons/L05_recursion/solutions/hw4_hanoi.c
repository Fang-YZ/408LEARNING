/* L5 HW4 reference (challenge): print every move of the Tower of Hanoi. */
#include <stdio.h>

static int move_count = 0;

void hanoi(int disks, char from_peg, char to_peg, char via_peg);

int main(void)
{
    int disks;

    scanf("%d", &disks);

    hanoi(disks, 'A', 'C', 'B');

    printf("total moves = %d\n", move_count);

    return 0;
}

void hanoi(int disks, char from_peg, char to_peg, char via_peg)
{
    if (disks <= 0)                 /* wide base case: n = 0 must not recurse forever */
    {
        return;
    }

    if (disks == 1)
    {
        move_count++;
        printf("move disk 1: %c -> %c\n", from_peg, to_peg);
        return;                     /* base case: one disk left */
    }

    hanoi(disks - 1, from_peg, via_peg, to_peg);    /* step 1: clear the small pile */
    move_count++;
    printf("move disk %d: %c -> %c\n", disks, from_peg, to_peg);
    hanoi(disks - 1, via_peg, to_peg, from_peg);    /* step 3: pile it onto the target */
}
