# L7 复习笔记 — 指针（上）：内存与地址、`&` 与 `*`、传址调用

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- 内存是一排带编号的格子，**编号叫地址**；`&变量` 取出它的门牌号，`*指针` 顺着地址走到那块内存。
- `int *p = &score;` 读作"p 是指向 int 的指针，初值 = score 的地址"；`*p` 叫**解引用**。
- `*` 有两个身份：**声明里**表示"这是指针"（`int *p`），**表达式里**表示"取这个地址上的值"（`*p = 100`）。
- `&` 与 `*` 互为逆运算：`*&x` 就是 `x`，`&*p` 就是 `p`（前提 p 有效）。
- 指针的类型信息是"指向什么类型"：`int *` 与 `char *` 都占 8 字节，但 `*p` 读几个字节、`p+1` 走多远完全不同（L8 细讲）。
- **未初始化的指针是野指针**：里面是垃圾地址，`*p` 读写的是随机内存 → 未定义行为（UB），可能当场崩溃。
- 定义指针要么给真实地址、要么 `= NULL`；用之前先 `if (p != NULL)`；**NULL 绝不能解引用**。
- C 的函数参数**永远传值**（拷贝一份）；要改调用者的变量必须传**地址**、在函数里用 `*` 写回去，这叫**传址调用**。
- **`scanf("%d", &x)` 的 `&` 是 L1 伏笔的回收**：scanf 要把值写进你的变量，就必须知道地址（数组名本身就是地址，所以不用 `&`）。
- 408 用途：单链表的 `next`、二叉树的左右孩子、图的邻接表，本质都是"**用地址把分散的结点串起来**"；指针不熟，链表寸步难行。

## 二、代码模板

> 下面的输出都是 `gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1` 编译（**零警告**）后真实运行的结果；**地址每次运行都不同**，要看的是它们之间的关系。

**模板 1：看见地址 —— `&` 取地址、`*` 解引用（一块内存两个名字）**
```c
/* L07 t1: one block of memory, two names -- score and *p. */
#include <stdio.h>

int main(void)
{
    int score = 90;
    int *p = &score;                 /* p stores the address of score */

    printf("&score = %p, p = %p, *p = %d\n", (void *)&score, (void *)p, *p);
    *p = 100;                        /* write through the pointer */
    printf("after *p = 100: score = %d\n", score);

    return 0;
}
```
```
&score = 000000000061fe14, p = 000000000061fe14, *p = 90
after *p = 100: score = 100
```
> `&score == p`（同一个地址）；`*p == 90`；`*p = 100` 改的是 p 指向的格子，所以 `score` 跟着变 —— **p 与 score 是同一块内存的两个名字**。

**模板 2：指针类型与 `sizeof`（`%zu` 在 MinGW 下必须配 `-D__USE_MINGW_ANSI_STDIO=1`）**
```c
/* L07 t2: pointer size comes from the machine, not from the pointed-to type. */
#include <stdio.h>

int main(void)
{
    printf("sizeof(int) = %zu, sizeof(int *) = %zu, sizeof(int[5]) = %zu\n",
           sizeof(int), sizeof(int *), sizeof(int[5]));
    printf("sizeof(char *) = %zu, sizeof(double *) = %zu, sizeof(void *) = %zu\n",
           sizeof(char *), sizeof(double *), sizeof(void *));

    return 0;
}
```
```
sizeof(int) = 4, sizeof(int *) = 8, sizeof(int[5]) = 20
sizeof(char *) = 8, sizeof(double *) = 8, sizeof(void *) = 8
```
> 三种指针本机都是 **8 字节**（64 位机的地址宽度），数组 `int[5]` 是 20 字节；**不加那个宏**，MinGW 8.1.0 实测报 `unknown conversion type character 'z' in format [-Wformat=]`，零警告直接泡汤。

**模板 3：传值 vs 传址 —— swap 的正反面**
```c
/* L07 t3: pass by value changes only the copy; pass by address changes the original. */
#include <stdio.h>

void swap_wrong(int a, int b)        /* a and b are copies */
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_right(int *a, int *b)      /* a and b are addresses */
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(void)
{
    int x = 1;
    int y = 2;

    swap_wrong(x, y);
    printf("after swap_wrong: x = %d, y = %d\n", x, y);
    swap_right(&x, &y);
    printf("after swap_right: x = %d, y = %d\n", x, y);

    return 0;
}
```
```
after swap_wrong: x = 1, y = 2
after swap_right: x = 2, y = 1
```
> `swap_wrong` 换的是两个副本，调用者的 x、y 一动没动；`swap_right` 拿到的是地址，`*a = *b` 写的就是 main 的变量 —— 408 的链表结点交换、快排 partition、堆调整，全都要"改调用者的变量"，必须传指针。

**模板 4：`scanf` 为什么必须写 `&`（L1 伏笔回收）**
```c
/* L07 t4: scanf writes into YOUR variable, so it needs an address. */
#include <stdio.h>

int main(void)
{
    int age = -1;
    int *p = &age;                   /* p already holds an address */

    printf("before: age = %d\n", age);
    if (scanf("%d", &age) != 1)      /* '&age' is exactly what scanf needs */
    {
        printf("bad input\n");
        return 1;
    }
    printf("after : age = %d, *p = %d (p still equals &age)\n", age, *p);

    return 0;
}
```
输入 `20`：
```
before: age = -1
after : age = 20, *p = 20 (p still equals &age)
```
> `scanf` 只会往"你给的地址"里写，写成 `scanf("%d", age)` 是把 age 的**值**当地址用（警告 `format '%d' expects argument of type 'int *'`，运行时多半崩溃）；而 `p == &age`，所以 `scanf("%d", p)` 也完全正确 —— **scanf 要的从来不是 `&` 这个符号，而是一个地址**。

**模板 5：`NULL`、返回指针、"没找到"怎么表达**
```c
/* L07 t5: return NULL when nothing is found, and check before dereferencing. */
#include <stdio.h>

int *find_first_negative(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < 0)
        {
            return &arr[i];          /* an address inside the caller's array */
        }
    }

    return NULL;
}

int main(void)
{
    int values[5] = {3, 5, -7, 9, 2};
    int *found = find_first_negative(values, 5);
    int *missing = find_first_negative(values, 2);

    if (found != NULL)
    {
        printf("first negative = %d, at index %td\n", *found, found - values);
    }
    printf("first 2 elements: %s\n", (missing == NULL) ? "not found" : "found");

    return 0;
}
```
```
first negative = -7, at index 2
first 2 elements: not found
```
> 这正是 408 里 `LNode *LocateElem(LinkList L, ElemType e)` 的雏形：**找不到就返回 NULL，调用者必须先判空再用**；`found - values` 是指针相减（两个地址差几个元素），第 8 课细讲。

## 三、坑清单（L7 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 声明了指针不初始化 | 野指针：`*p` 读写随机内存，UB，可能当场崩溃 | `int *p = NULL;` 或 `int *p = &x;`，用前判空 |
| 2 | `scanf("%d", x)` 漏 `&` | 把 x 的值当地址用；警告 `format '%d' expects argument of type 'int *'` | `scanf("%d", &x)` |
| 3 | 反过来打印时多写 `*`：`printf("%d", *x)` | 编译错误/警告 `invalid type argument of unary '*'` | 要值写 `x`，要地址写 `(void *)&x` |
| 4 | `printf("%p", p)` 不加强转 | 警告 `format '%p' expects argument of type 'void *'` | `printf("%p", (void *)p)` |
| 5 | `int *p, q;` 以为 q 也是指针 | q 其实是普通 int，后面写 `*q` 直接编译失败 | 一行只声明一个：`int *p, *q;` |
| 6 | 交换函数写成 `swap(int a, int b)` | 调用完 x、y 原封不动（换的是副本） | `swap(int *a, int *b)`，内部用 `*` 写回 |
| 7 | 调用时忘取地址：`swap_right(x, y)` | 把 1、2 当地址用 → 崩溃或乱码 | 调用处写 `swap_right(&x, &y)` |
| 8 | 函数里 `p = &b;` 想让调用者的指针改向 | 只改了形参副本，调用者的指针没变 | 要改指针本身就得传二级指针 `int **p`（本课只要知道有这回事，完整用法留到 **L12 单链表**讲透：王道 `InitList(LinkList *L)` 与头插法建表都会用到） |
| 9 | 解引用 NULL | `*p` / `p->x` 立刻崩溃（Windows 报 0xC0000005） | 用前 `if (p != NULL) { ... }` |
| 10 | 返回局部变量的地址 | 函数一返回栈帧销毁 → 悬空指针，读到垃圾 | 返回 NULL、静态数组或堆内存（L11 讲 malloc） |

## 四、复习自查清单

- [ ] 能画图讲清"变量—地址—指针"：`&score`、`p`、`*p` 各是什么
- [ ] 区分 `int *p;`（声明，读作"p 是指针"）与 `*p = 100;`（表达式，往指向的格子写值）
- [ ] 默写 `swap` 正确版本，并解释"传值为什么改不了调用者的变量"
- [ ] 一句话说明 `scanf("%d", &x)` 的 `&` 为什么不能省；`scanf("%d", p)` 为什么也对
- [ ] 口答：本机 `sizeof(int *)`、`sizeof(char *)`、`sizeof(void *)` 各是多少？为什么都相同？
- [ ] 什么是野指针？怎么避免？`int *p = NULL;` 之后能直接 `*p` 吗？
- [ ] 为什么不能返回局部变量的地址？"查找失败"该怎么表达？
- [ ] 知道 `int *p, q;` 里谁是指针，并会改成一行一个；会画栈帧图：main 的 x/y 与 swap_right 的 a/b 各在哪，指针连到哪
- [ ] 说出 408 里至少三处用指针的场合（单链表 next、二叉树左右孩子、图邻接表）

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本节只给方向，具体规格与通关测试值开课当天随讲义下发。存盘位置：`E:\learn408\homework\L07\`；判分看 `gcc -Wall -Wextra` 零警告 / 正确性 / 边界 / 风格（花括号不省、4 空格缩进、蛇形命名）。

- **HW1 ★ 地址观察台**：定义若干变量与指针，打印地址、值、解引用值，再用指针修改原变量，证明"同一块内存两个名字"。
- **HW2 ★ 指针版交换与排序**：实现 `swap(int *, int *)`，用它完成两个变量的交换或一组数据的排序，验证调用者的数据真的变了。
- **HW3 ★ 指针查找 + NULL**：写一个"在数组里找第一个满足条件的元素"的函数，返回它的地址；找不到时返回 `NULL`，由主函数判空后再用。
- **HW4 ★★★ 挑战：让调用者的指针改向**：函数除了要找到元素，还要把结果写回调用者的**指针本身**（提示：形参是 `int **`，想想为什么要多一层 `*`）。

## 参考：内存示意图（文字版）

main 调用 `swap_right(&x, &y)` 时的样子（地址数值每次运行不同，看的是**谁指向谁**）：
```
   main 的栈帧                 swap_right 的栈帧
   ┌──────────────┐      ┌────────────────────────┐
   │  x  [  1  ]  │ ←────┤ a  [ &x ]  指向 main 的 x │
   │  y  [  2  ]  │ ←────┤ b  [ &y ]  指向 main 的 y │
   └──────────────┘      │ temp [ 1 ]             │
                         └────────────────────────┘
```
- `swap_wrong` 里没有箭头：a、b 是**另开的两格**，装的是副本，怎么换都碰不到 main 的 x、y。
- 记法：**指针 = 存地址的变量**；`*` 是"顺着箭头走过去"，`&` 是"这个格子在哪"。
