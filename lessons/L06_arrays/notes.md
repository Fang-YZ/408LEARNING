# L6 复习笔记 — 数组：一维 / 二维 / 越界与经典小算法

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- `int scores[5];` 一次开出 5 个 int，**下标是 0…4**，`scores[5]` 已经越界 —— C **不检查**，这是最坑的地方。
- 内存里元素**连续排列**：相邻元素地址差 = 一个元素的大小（int 是 4 字节）→ 这就是 408 里「顺序表随机存取 O(1)」的物理基础。
- 初始化三种档次：`{90, 85, 77, 60, 98}` 全给；`{0}` 全部变 0；`{1, 2, 3}` 只给前面，**剩下的自动补 0**；一个都不给就是垃圾值。
- 长度用 `sizeof(values) / sizeof(values[0])` 算出来，别写死数字 —— 但**在函数里算会得到 8**（数组作形参退化成指针）。
- 遍历永远写 `for (int index = 0; index < count; index++)`：是 `<` 不是 `<=`，是 `count` 不是 `count - 1`。
- 求最值时初值**不能写 0**（全是负数时 0 会冒充最大值），要求平均则两个 int 相除会截断（74 / 6 = 12）：`int maximum = values[0];` + `(double)total / count`。
- 反转用**双下标**：`left++, right--`，循环条件 `left < right`；奇数个元素时中间那个自己不用动。
- 二维数组 `int grid[3][4]` 是「3 行 4 列」；内存里仍然**一行接一行**（行优先），所以矩阵压缩存储能摊成一维。
- 数组做实参传的是**首元素地址**：函数里写 `values[index] = x` 能改到原数组（冒泡排序就靠这个），但 `sizeof` 在函数里失效。
- 冒泡：外层 `pass` 控制轮数（最多 n − 1 轮），内层 `index < count - 1 - pass`，相邻逆序就交换；某一轮没交换过就提前收工。
- 二分查找的前提是**有序**：每次砍一半，16 个元素里找 29 只要 4 次比较，顺序查找要 15 次（本机实测）。
- 408 用途：顺序表 / 栈 / 队列全都建在数组上；二分查找、矩阵压缩存储（对称矩阵、三角矩阵）都是必考点。

## 二、代码模板

**模板 1：定义、初始化、遍历**

```c
/* Array basics: fixed length, indexes run from 0 to count - 1. */
int scores[5] = {90, 85, 77, 60, 98};                     /* full initialization */
int zeros[4] = {0};                                       /* every element becomes 0 */
int partial[5] = {1, 2, 3};                               /* the tail is filled with 0 */
int count = (int)(sizeof(scores) / sizeof(scores[0]));    /* 5: computed, not hard-coded */

for (int index = 0; index < count; index++)               /* < not <= */
{
    printf("scores[%d] = %d\n", index, scores[index]);
}
```

实测：`count = 5, first = 90, last = 98`；`zeros` 打印 `0 0 0 0`；`partial` 打印 `1 2 3 0 0`（后两个真的是 0，不是垃圾值）。

**模板 2：求和 / 最值 / 平均 / 反转**

```c
/* One pass collects the sum and the maximum; then reverse in place with two indexes. */
int total = 0;
int maximum = values[0];                /* start from a real element, not from 0 */

for (int index = 0; index < count; index++)
{
    total += values[index];
    if (values[index] > maximum)
    {
        maximum = values[index];
    }
}

printf("sum = %d, average = %.2f\n", total, (double)total / count);
printf("max = %d\n", maximum);

for (int left = 0, right = count - 1; left < right; left++, right--)
{
    int temp = values[left];            /* swap the two ends, then move inward */
    values[left] = values[right];
    values[right] = temp;
}
```

实测 `values[] = {12, 5, 27, 8, 19, 3}`：`sum = 74, average = 12.33`、`max = 27`（原程序还打印了 `min = 3`），反转后是 `3 19 8 27 5 12`。
`(double)` 不能省，否则整数除法先把 74 / 6 截断成 12；求最小值同理，把 `>` 改成 `<`，初值仍取 `values[0]`。

**模板 3：二维数组与矩阵（行、列、转置）**

```c
/* A 2D array is rows of rows; in memory it is still one flat block, row by row. */
int grid[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

for (int row = 0; row < 3; row++)           /* walk row by row */
{
    int row_sum = 0;
    for (int col = 0; col < 4; col++)
    {
        row_sum += grid[row][col];
    }
    printf("row %d sum = %d\n", row, row_sum);
}

for (int col = 0; col < 4; col++)           /* transpose: just swap the two loops */
{
    for (int row = 0; row < 3; row++)
    {
        printf("%4d", grid[row][col]);
    }
    printf("\n");
}
```

本机实测输出（节选：行和 + 转置后的 4 行 3 列）：

```text
row 0 sum = 10
row 1 sum = 26
row 2 sum = 42
   1   5   9
   2   6  10
   3   7  11
   4   8  12
```

**模板 4：数组做实参 —— 退化成指针（完整程序）**

```c
/* An array parameter is really a pointer: sizeof() inside the callee tells the truth about that. */
#include <stdio.h>

void show_decay(const int *values_ptr)     /* the same thing as writing const int values[] */
{
    printf("in the callee : sizeof(values_ptr) = %zu bytes (one pointer, not the array)\n", sizeof(values_ptr));
}

int main(void)
{
    int values[] = {10, 20, 30, 40, 50};

    printf("in main       : sizeof(values) = %zu bytes\n", sizeof(values));
    printf("in main       : element count  = %zu\n", sizeof(values) / sizeof(values[0]));

    show_decay(values);
    return 0;
}
```

本机实测输出：

```text
in main       : sizeof(values) = 20 bytes
in main       : element count  = 5
in the callee : sizeof(values_ptr) = 8 bytes (one pointer, not the array)
```

- 20 字节 = 5 × 4：`main` 里它是**真数组**；函数里只剩 **8 字节**（64 位指针），数组的长度信息丢了。
- 所以**长度必须作为参数一起传**（`sum_of(values, 5)`）；反过来，函数里写 `values[index] = x` 会改到原数组。
- 这就是「数组作实参退化为指针」的预告，L7 / L8 会把这件事讲透。
- **MinGW 坑**：`%zu` 和 `%lld` 一样要加 `-D__USE_MINGW_ANSI_STDIO=1`，否则 `-Wall -Wextra` 会报
  `warning: unknown conversion type character 'z' in format [-Wformat=]`（本机实测仍能打印对，但不能赌）。

**模板 5：冒泡排序（排序核心 + 提前收工）**

```c
/* Bubble sort: keep swapping neighbours that are out of order, pass by pass. */
void bubble_sort(int values[], int count)   /* values[] here is a pointer: the caller's array gets changed */
{
    for (int pass = 0; pass < count - 1; pass++)
    {
        int swapped = 0;
        for (int index = 0; index < count - 1 - pass; index++)
        {
            if (values[index] > values[index + 1])
            {
                int temp = values[index];
                values[index] = values[index + 1];
                values[index + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0)       /* a clean pass means it is already sorted */
        {
            break;
        }
    }
}
```

完整程序（加一个 `print_array` 辅助函数，`main` 里排序前后各打印一次）本机实测：
`before: 5 1 4 2 8 0 9` → `after : 0 1 2 4 5 8 9`。
每轮至少把一个最大值「冒」到右端，所以内层可以少比一个（`- pass`）；最坏 O(n²)，最好（已有序）O(n)。

**模板 6：顺序查找与二分查找**

```c
/* Linear search works on any array; binary search demands a sorted one. */
int linear_search(const int values[], int count, int target)
{
    for (int index = 0; index < count; index++)
    {
        if (values[index] == target)
        {
            return index;               /* found: report the position */
        }
    }
    return -1;                          /* -1 means "not found" */
}

int binary_search(const int values[], int count, int target)
{
    int low = 0;
    int high = count - 1;

    while (low <= high)                 /* the search range is [low, high] */
    {
        int middle = low + (high - low) / 2;   /* safe middle: (low + high) / 2 can overflow */
        if (values[middle] == target)
        {
            return middle;
        }
        if (values[middle] < target)
        {
            low = middle + 1;           /* throw away the left half */
        }
        else
        {
            high = middle - 1;          /* throw away the right half */
        }
    }
    return -1;
}
```

本机实测（带计数器的完整程序，16 个有序元素 `1 3 5 … 31` 里找 29）：顺序查找比较 **15** 次、二分查找只比较 **4** 次；
找不存在的 30 时，二分比较 5 次后返回 -1。二分是 O(log n)，但**数组必须先有序**，否则结果随机错。

## 三、坑清单（L6 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 下标越界：写 `values[count]`，或遍历用 `index <= count` | 读到垃圾值 / **静默改坏**旁边变量，编译器不报警 | 合法下标 `0 … count - 1`；遍历写 `index < count` |
| 2 | 在函数里用 `sizeof(values) / sizeof(values[0])` | 得到 8（指针大小），循环次数完全错 | 长度当参数传进来 |
| 3 | 数组整体赋值 `copy = values;` | 编译错误：数组不是可赋值的对象 | 逐元素拷贝（循环），或以后学 `memcpy` |
| 4 | 用 `==` 比较两个数组 | 比的是首地址，永远不相等 | 逐元素比较，发现不同就返回 0 |
| 5 | 求最值初值写 0 | 全是负数时输出 0，答案错 | `int maximum = values[0];` |
| 6 | 求和后直接 `total / count` 求平均 | 整数除法截断（74 / 6 → 12） | `(double)total / count` |
| 7 | 定义数组长度用变量（VLA） | gcc 能过，C++ / 408 环境不通用，且不能初始化 | 用常量长度或宏 `#define N 100` |
| 8 | 冒泡内层写成 `count - 1`（忘 `- pass`） | 多比很多次，极端情况越界 | `index < count - 1 - pass` |
| 9 | 对**无序**数组做二分查找 | 找不到或返回错误下标，程序不报错 | 先排序，或改用顺序查找 |
| 10 | 二分里写 `low = middle;` / `high = middle;` | 区间不缩小 → 死循环 | `low = middle + 1;` / `high = middle - 1;` |
| 11 | `%zu` / `%lld` 没加 MinGW 宏 | `-Wformat=` 警告（本机实测仍打印对，但不能赌） | 编译加 `-D__USE_MINGW_ANSI_STDIO=1` |

## 四、复习自查清单

- [ ] 不查资料默写：数组定义与三种初始化、遍历、求和 / 最值 / 平均 / 反转
- [ ] 口答：`int scores[5]` 的合法下标范围？`scores[5]` 会发生什么？
- [ ] 解释「数组在内存中连续存放」，并说明它和顺序表随机存取的关系
- [ ] `{1, 2, 3}` 初始化长度 5 的数组，后两个元素是什么？不写 `{}` 呢？
- [ ] 为什么求最值的初值不能用 0？举一组让它出错的数据
- [ ] 为什么在函数里 `sizeof(values) / sizeof(values[0])` 不对？正确做法是什么？
- [ ] 数组作实参时，函数里改 `values[index]` 为什么能改到原数组？（说出「首地址」三个字）
- [ ] 手算一次冒泡排序的每一轮结果（给定 5 个数），并说出总共比较了多少次
- [ ] 二分查找：给定 16 个有序数，手写查找 29 时的 low / high / middle 变化
- [ ] 手敲模板 4 与模板 5，零警告编译并运行，输出与笔记一致

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本节只给**主题**：具体输入输出格式与通关测试值随讲义下发，避免提前抄答案。

- 一维数组的读入与统计：最大值 / 最小值 / 总和 / 平均值一起求出来。
- 数组的原地反转与「把数据循环移动若干位」，重点练双下标与边界。
- 手写冒泡排序（或选择排序）并按格式输出排序前后的数组。
- 挑战：在**有序**数组上做二分查找，并在找不到时给出正确的失败标记。

## 参考：`int values[5]` 在内存中的布局

| 下标 | 地址 | 相邻间隔 | 说明 |
| --- | --- | --- | --- |
| `values[0]` | 基址 + 0 | — | 首元素地址 = 数组名 `values` 的值 |
| `values[1]` | 基址 + 4 | 4 字节 | 正好是一个 int 的大小 |
| `values[4]` | 基址 + 16 | 4 字节 | 最后一个合法元素（地址差 = 下标差 × 4） |
| `values[5]` | 基址 + 20 | 4 字节 | **越界**：属于别人的内存，写它会出灵异 bug |

> 记法：`&values[i] == values + i`（以元素为单位），字节地址 = 基址 + i × sizeof(int)。
> 地址连续 ⇒ 知道基址和下标就能直接算出地址 ⇒ 随机存取 O(1)，这正是顺序表的看家本领。
