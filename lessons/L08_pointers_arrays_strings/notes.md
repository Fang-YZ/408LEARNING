# L8 复习笔记 — 指针（下）：数组与指针、指针算术、const 与数组参数

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- **核心恒等式**：`a[i]` 与 `*(a + i)` 完全等价（编译器就是这么翻译的）；数组名在**大多数表达式**里"退化"成首元素地址，所以 `int *p = a;` 就是 `int *p = &a[0];`。
- 但数组名**不是**指针变量：它不能赋值、不能自增（`a++` 报错 `lvalue required as increment operand`）。
- 只有两个场合数组名不退化：`sizeof(a)`（整个数组的字节数，`int a[5]` 得 20）与 `&a`（类型是"指向整个数组的指针"）。
- **指针算术的步长 = `sizeof(*p)`**：`int *p` 加 1 前进 4 字节，`char *p` 加 1 前进 1 字节（本机实测）。
- 指针相减得到"**隔了几个元素**"（类型 `ptrdiff_t`，`printf` 用 `%td`，MinGW 要加 `-D__USE_MINGW_ANSI_STDIO=1`）。
- 遍历标准写法 `for (int *p = a; p < a + n; p++)`：`a + n` 是尾后地址，**只当上界，不许解引用**。
- 数组作函数实参时传的是**首元素地址**：函数里的形参是指针，`arr[i] = x` 会**改到调用者的数组**；因此长度必须另传 `int n`。
- `const` 三种写法：`const int *p`（不能改 `*p`，能改向）｜`int *const p`（能改 `*p`，不能改向）｜`const int *const p`（都不能）。
- 408 用途：顺序表用数组 + 下标，链表/树/图用指针 + 动态结点；`a[i] ≡ *(a+i)` 正是"顺序表按位查找 O(1)"的语言层依据。

## 二、代码模板

> 下面所有输出都是 `gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1` 编译（**零警告**）后真实运行的结果；**地址每次运行都不同**。

**模板 1：`a[i]` 与 `*(a + i)` 的等价性（四种写法同一个值）**
```c
/* L08 t1: a[i] and *(a + i) are the same thing. */
#include <stdio.h>

int main(void)
{
    int a[5] = {10, 20, 30, 40, 50};
    int *p = a;                      /* the array name decays to &a[0] */

    printf("a = %p, &a[0] = %p, p = %p\n", (void *)a, (void *)&a[0], (void *)p);
    for (int i = 0; i < 5; i++)
    {
        printf("i = %d: a[i] = %d, *(a + i) = %d, *(p + i) = %d, p[i] = %d\n",
               i, a[i], *(a + i), *(p + i), p[i]);
    }

    return 0;
}
```
```
a = 000000000061fdf0, &a[0] = 000000000061fdf0, p = 000000000061fdf0
i = 0: a[i] = 10, *(a + i) = 10, *(p + i) = 10, p[i] = 10
i = 1: a[i] = 20, *(a + i) = 20, *(p + i) = 20, p[i] = 20
i = 2: a[i] = 30, *(a + i) = 30, *(p + i) = 30, p[i] = 30
i = 3: a[i] = 40, *(a + i) = 40, *(p + i) = 40, p[i] = 40
i = 4: a[i] = 50, *(a + i) = 50, *(p + i) = 50, p[i] = 50
```
> 第一行三个地址完全相同 —— 数组名 `a` 就是 `&a[0]`。既然 `a[i]` 就是 `*(a+i)`，而加法可交换，`2[a]` 这种写法也合法（**看得懂就行，永远别写**）。

**模板 2：指针算术的步长 = `sizeof(*p)`**
```c
/* L08 t2: p + 1 jumps sizeof(*p) bytes, not one byte. */
#include <stdio.h>

int main(void)
{
    int a[4] = {1, 2, 3, 4};
    char s[4] = {'a', 'b', 'c', 'd'};
    int *p_int = a;
    char *p_char = s;

    printf("int *: %p -> %p, element step = %td, byte step = %td\n",
           (void *)p_int, (void *)(p_int + 1),
           (p_int + 1) - p_int, (char *)(p_int + 1) - (char *)p_int);
    printf("char*: %p -> %p, element step = %td, byte step = %td\n",
           (void *)p_char, (void *)(p_char + 1),
           (p_char + 1) - p_char, (char *)(p_char + 1) - (char *)p_char);

    return 0;
}
```
```
int *: 000000000061fe00 -> 000000000061fe04, element step = 1, byte step = 4
char*: 000000000061fdfc -> 000000000061fdfd, element step = 1, byte step = 1
```
> 看地址尾巴：`...e00` → `...e04`（+4，一个 int 4 字节）；`...dfc` → `...dfd`（+1，一个 char 1 字节）—— **`p + 1` 是"跳到下一个同类型元素"，不是"下一个字节"**，这就是指针能做数组遍历的根本原因。

**模板 3：用移动的指针遍历数组（`a + n` 是停止线）**
```c
/* L08 t3: walk an array with a moving pointer; a + n is the stop address. */
#include <stdio.h>

int main(void)
{
    int a[6] = {2, 4, 6, 8, 10, 12};
    int n = (int)(sizeof(a) / sizeof(a[0]));
    int sum = 0;

    for (int *p = a; p < a + n; p++)     /* p < a + n, never p <= a + n */
    {
        printf("%d at %p\n", *p, (void *)p);
        sum += *p;
    }
    printf("sum = %d\n", sum);

    return 0;
}
```
```
2 at 000000000061fdf0
4 at 000000000061fdf4
6 at 000000000061fdf8
8 at 000000000061fdfc
10 at 000000000061fe00
12 at 000000000061fe04
sum = 42
```
> 地址每次加 4（`sizeof(int)`），这就是"指针走一步跨一个元素"的直观画面；循环条件写 `p < a + n`（不是 `<=`），因为 `a + n` 是尾后地址，只当上界、**不许解引用**；`sizeof(a) / sizeof(a[0])` 是求元素个数的标准算法，但**只能用在定义数组的那个函数里**（见模板 5 的坑）。

**模板 4：`const` 修饰指针的三种写法**
```c
/* L08 t4: const int *p / int * const p / const int * const p */
#include <stdio.h>

int main(void)
{
    int a = 1;
    int b = 2;

    const int *p_read = &a;              /* cannot write *p_read, can repoint */
    int *const p_fixed = &a;             /* can write *p_fixed, cannot repoint */
    const int *const p_both = &a;        /* neither */

    p_read = &b;                         /* OK: the pointer itself is not const */
    *p_fixed = 100;                      /* OK: the pointee is not const */

    printf("a = %d, b = %d, *p_read = %d, *p_fixed = %d, *p_both = %d\n",
           a, b, *p_read, *p_fixed, *p_both);

    return 0;
}
```
```
a = 100, b = 2, *p_read = 2, *p_fixed = 100, *p_both = 100
```
> 读法看 `const` 在 `*` 的哪边：**在 `*` 左边 → 锁值（`*p` 只读）；在 `*` 右边 → 锁指针本身（p 不能改向）**；函数参数写 `const int *arr` 就是"我只读你的数组"，这是 C 里表达"不修改实参"的标准手段。

**模板 5：函数收数组实参的真相 —— 收到的是指针，共享同一块内存**
```c
/* L08 t5: an array parameter is really a pointer parameter -- the caller's array is shared. */
#include <stdio.h>

void print_all(int arr[], int n)     /* the compiler reads this as: int *arr */
{
    arr[0] = 999;                    /* writes straight into the caller's array */

    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    arr++;                           /* legal here: an array NAME could never do this */
    printf("after arr++: arr[0] = %d\n", arr[0]);
}

int main(void)
{
    int data[4] = {1, 2, 3, 4};

    print_all(data, 4);
    printf("data[0] after the call = %d\n", data[0]);

    return 0;
}
```
```
999 2 3 4 
after arr++: arr[0] = 2
data[0] after the call = 999
```
> 两条铁证说明形参 `int arr[]` 就是 `int *arr`：① 函数里能 `arr++`（数组名做不到）；② `arr[0] = 999` 改到了 main 的 `data[0]`。因此函数**永远不知道数组有多长**，必须再传一个 `n` —— 408 的 `void BubbleSort(int a[], int n)` 就是这个道理。

## 三、坑清单（L8 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 函数里写 `sizeof(arr)` 求数组长度 | 返回 8（指针大小）而不是数组字节数；GCC 还警告 `'sizeof' on array function parameter 'arr' will return size of 'int *' [-Wsizeof-array-argument]`（默认开启） | 长度当参数传：`void f(int arr[], int n)` |
| 2 | 以为 `p + 1` 走 1 字节 | `int *` 实际走 4 字节，`char *` 才走 1 字节 | 记住步长 = `sizeof(*p)` |
| 3 | `*(a + i)` 写成 `*a + i` | 不报错但结果错：只取了 `a[0]` 再 `+ i` | 下标运算别忘括号：`*(a + i)` |
| 4 | 遍历条件写成 `p <= a + n` | 多访问一格（越界读写），结果诡异或崩溃 | 用 `p < a + n` |
| 5 | 在别的函数里用 `sizeof(a) / sizeof(a[0])` 求长度 | 算成 `8 / 4 = 2`，完全错 | 长度随参数传进来 |
| 6 | `const` 位置记反 | `const int *p` 却去写 `*p` → 报错 `assignment of read-only location` | `const` 在 `*` 左锁值，在 `*` 右锁指针 |
| 7 | 用 `char *` 改字符串字面量：`char *s = "abc"; s[0] = 'A';` | 编译通过，运行时崩溃（本机实测退出码 `0xC0000005`，访问冲突） | 要改就开数组：`char s[] = "abc";`（第 9 课细讲） |
| 8 | `int *p[5]` 与 `int (*p)[5]` 混为一谈 | 前者是"5 个 int 指针的数组"，后者是"指向含 5 个 int 的数组的指针" | 看括号：`(*p)` 才是指针 |
| 9 | 数组越界读写（`a[5]`、解引用 `p + 5`） | 有时"看着正常"，实际改了别人的内存；UB，随时翻车 | 下标范围 `0 .. n-1`，编译器管不了，自己守边界 |
| 10 | `%zu` / `%td` 不加宏 | MinGW 8.1.0 报 `unknown conversion type character 'z'` / `'t' in format` | 编译加 `-D__USE_MINGW_ANSI_STDIO=1` |

## 四、复习自查清单

- [ ] 说出 `a[i]`、`*(a + i)`、`*(p + i)`、`p[i]` 四者的关系，并默写数组遍历的两种写法
- [ ] 数组名和指针变量到底差在哪？（能否赋值、能否自增、`sizeof` 结果）
- [ ] `sizeof(a)` 与 `sizeof(p)`（p 指向 a）分别是多少？为什么？
- [ ] `int *p` 与 `char *p` 都占 8 字节，为什么 `p + 1` 一个走 4 字节、一个走 1 字节？
- [ ] 指针相减 `p2 - p1` 得到什么？类型是什么？`printf` 用什么占位符？
- [ ] 尾后指针 `a + n` 能解引用吗？遍历的循环条件该怎么写？
- [ ] 为什么 `void f(int arr[])` 里拿不到数组长度？正确做法是什么？
- [ ] 讲清 `const int *p`、`int *const p`、`const int *const p` 各锁住了什么
- [ ] 解释"数组参数是共享的"：函数里 `arr[0] = 999` 为什么会影响调用者？
- [ ] 手敲 5 个模板，`-Wall -Wextra` 零警告并跑出与笔记一致的输出
- [ ] 说出 408 里数组/指针的分工：顺序表用数组 + 下标，链表/树/图用指针 + 结点

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本节只给方向，具体规格与通关测试值开课当天随讲义下发。存盘位置：`E:\learn408\homework\L08\`。
> 判分：`gcc -Wall -Wextra` 零警告 / 正确性 / 边界 / 风格（花括号不省、4 空格缩进、蛇形命名）。

- **HW1 ★ 两种遍历对照**：对同一个数组，一次用下标 `a[i]` 遍历、一次用指针 `*(p + i)` 遍历，输出必须逐行一致；再打印每个元素的地址，验证步长。
- **HW2 ★ 指针版统计**：用指针遍历求数组的最大值、最小值、平均值（形参用 `const int *`，长度显式传参），体会"函数共享数组"。
- **HW3 ★ 反转与查找**：用一头一尾两个指针原地反转数组；再用指针查找目标值，返回它的**地址**，找不到返回 `NULL`。
- **HW4 ★★★ 挑战：数组长度只能算一次**：故意在函数里写 `sizeof(arr) / sizeof(arr[0])`，亲眼看警告与错误结果，再改成长度作参数传入 —— 把这条坑写成注释留在代码里。

## 参考：指针类型速查表

| 声明 | 读法 | 本机 `sizeof` | `p + 1` 前进 | 408 里的典型用途 |
| --- | --- | --- | --- | --- |
| `int *p` | p 指向 int | 8 | 4 字节 | 顺序表遍历、`swap`、数组参数 |
| `char *p` | p 指向 char | 8 | 1 字节 | 字符串、串的模式匹配 |
| `void *p` | 无类型指针 | 8 | 不能 `p + 1` | `malloc` 返回值、`printf("%p")` |
| `int **p` | p 指向"int 指针" | 8 | 8 字节 | 让函数改调用者的指针、链表头插 |
| `int (*p)[5]` | p 指向"含 5 个 int 的数组" | 8 | 20 字节 | 二维数组的行指针 |

> 记忆口诀：**指针本身的大小看机器（64 位机都是 8），`p + 1` 的步长看类型（`sizeof(*p)`）**。
