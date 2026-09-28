#include <stdio.h>
static int totalcount = 0;
void hanoi(int disks, char from, char to, char via);

int main(){
    int disks;
    scanf("%d", &disks);
    hanoi(disks, 'A', 'C', 'B');
    printf("total moves = %d", totalcount);
    return 0;
}

void hanoi(int disks, char from, char to, char via){
    if(disks <= 0) {
        return;
    }
    if(disks == 1){
        ++totalcount;
        printf("move disk 1: %c -> %c\n", from, to);
        return;
    }
    hanoi(disks - 1, from, via, to);
    ++totalcount;
    printf("move disk %d: %c -> %c\n", disks, from, to);
    hanoi(disks - 1, via, to, from);
}