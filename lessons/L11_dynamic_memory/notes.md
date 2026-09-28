# L11 复习笔记 — 动态内存：malloc / calloc / realloc / free

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- 程序的内存分两块地：**栈**（局部变量、函数参数；自动分配、自动回收；函数一返回就没了）和**堆**（`malloc` 申请、`free` 归还；想活多久活多久；**由你负责**）。栈很小：本机实测 1 MB 局部数组还能跑，4 MB 编译零警告但**运行直接崩**（退出码 `0xC00000FD` = 栈溢出）。
- 四件套都在 `<stdlib.h>`：`malloc(字节数)` 申请、**不初始化** ｜ `calloc(个数, 每个字节数)` 申请、**清零** ｜ `realloc(旧指针, 新字节数)` 扩缩、**保留旧内容** ｜ `free(指针)` 归还。
- `malloc` 返回 `void *`：`int *arr = (int *)malloc(n * sizeof(int));` —— C 里不转也能过，但 C++ 必须转，本课统一**强转**。
- **申请字节数 = 元素个数 × sizeof(元素类型)**。写成 `malloc(n)` 只给 n 个字节却当 n 个 int 用，就是越界写。
- **返回值必须检查**：内存不够时返回 `NULL`，直接解引用就是崩溃。`if (p == NULL) { ... }` 是最低成本的保险。
- 四大地雷：**内存泄漏**（只申请不释放）、**野指针**（free 后还留着地址）、**重复释放**（double free）、**释放后使用**（use after free）。
- 保命习惯：**`free(p); p = NULL;`** —— 一行同时防住野指针和重复释放（`free(NULL)` 是合法的空操作）。
- `free` 只接收**堆上、malloc 家族返回的原始首地址**：`free(&x)`、`free(arr + 1)`、free 两次都是错的。
- `realloc` 可能**整块搬家**：必须用临时指针接返回值，成功后再赋给原指针；写成 `p = realloc(p, ...)` 一旦失败就丢了旧地址 = 泄漏。
- **动态数组**（capacity 满了就翻倍）就是顺序表的 C 实现雏形：`data` 指针 + `size` 已用长度 + `capacity` 现有容量。
- 链表结点必须用 `malloc`：**局部变量在函数返回时就被销毁**，把它的地址传出去就是野指针 —— 这是 L12 的前置结论。
- 打印指针统一 `printf("%p", (void *)p)`；MinGW 下 NULL 打印成 `0000000000000000`（不是 `(nil)`），且地址每次运行都可能不同。

## 二、代码模板

**模板 1：一个 int 的堆内存全生命周期（申请 → 检查 → 用 → 还 → 置空）**

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *single = (int *)malloc(sizeof(int));   /* 1 int = 4 bytes, NOT zeroed */
    if (single == NULL)                         /* always check the return value */
    {
        printf("malloc failed\n");
        return 1;
    }

    *single = 42;
    printf("*single = %d, single = %p\n", *single, (void *)single);

    free(single);        /* give the block back exactly once ... */
    single = NULL;       /* ... then kill the pointer: no dangling address left */
    return 0;
}
```

**模板 2：calloc 清零 与 realloc 扩容（必须用临时指针）**

```c
int count = 5;
int *zeros = (int *)calloc((size_t)count, sizeof(int));   /* all bytes are zero */
if (zeros == NULL)
{
    return 1;
}

zeros[0] = 10;
zeros[4] = 50;

int new_count = 8;
int *grown = (int *)realloc(zeros, (size_t)new_count * sizeof(int));
if (grown == NULL)
{
    free(zeros);         /* realloc failed: the OLD block is still valid */
    zeros = NULL;
    return 1;
}
zeros = grown;           /* only now is it safe to overwrite the old pointer */

for (int i = count; i < new_count; i++)
{
    zeros[i] = 100 + i * 10;   /* the new room is uninitialized: fill it */
}

free(zeros);
zeros = NULL;
```

**模板 3：动态数组（capacity 满了就翻倍）**

```c
typedef struct
{
    int *data;       /* the heap block that holds the values */
    int size;        /* how many values are stored right now */
    int capacity;    /* how many fit before the next grow */
} IntVector;

int vector_init(IntVector *vec, int capacity)
{
    vec->data = (int *)malloc((size_t)capacity * sizeof(int));
    vec->size = 0;
    vec->capacity = (vec->data != NULL) ? capacity : 0;
    return vec->data != NULL;                   /* 1 = ok, 0 = failed */
}

int vector_push(IntVector *vec, int value)
{
    if (vec->size == vec->capacity)                 /* full: double the capacity */
    {
        int new_capacity = vec->capacity * 2;
        int *new_block = (int *)realloc(vec->data, (size_t)new_capacity * sizeof(int));
        if (new_block == NULL)
        {
            return 0;                               /* the old block is still usable */
        }
        vec->data = new_block;
        vec->capacity = new_capacity;
        printf("  [grow] capacity %d -> %d\n", vec->capacity / 2, vec->capacity);
    }
    vec->data[vec->size] = value;
    vec->size++;
    return 1;                                       /* 1 means success */
}

void vector_free(IntVector *vec)
{
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}
```

补上 `vector_print` 和 `main` 就是一个完整程序，本课实测输出（`gcc -Wall -Wextra` 零警告，容量初值 2，push 1..6）：

```text
init: capacity = 2
  size = 1, capacity = 2, data = 1
  size = 2, capacity = 2, data = 1 2
  [grow] capacity 2 -> 4
  size = 3, capacity = 4, data = 1 2 3
  size = 4, capacity = 4, data = 1 2 3 4
  [grow] capacity 4 -> 8
  size = 5, capacity = 8, data = 1 2 3 4 5
  size = 6, capacity = 8, data = 1 2 3 4 5 6
after free: data = 0000000000000000, size = 0, capacity = 0
```

> 上面是删掉每行 `push N` 提示语之后的输出；两次 `[grow]` 就是容量翻倍发生的地方。

**模板 4：把结构体放到堆上 —— 为链表结点申请内存（L12 预告，`->` 登场）**

```c
struct Node
{
    int data;
    struct Node *next;
};

struct Node *node_create(int value)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL)
    {
        printf("malloc failed: out of memory\n");
        exit(1);                  /* nothing sane left to do */
    }
    new_node->data = value;
    new_node->next = NULL;        /* a fresh node points to nothing */
    return new_node;
}
```

## 三、坑清单（L11 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | `malloc` 后不检查 `NULL` | 内存不足时解引用空指针，直接崩 | `if (p == NULL) { printf("malloc failed\n"); return 1; }` |
| 2 | `malloc(n)` 只给 n 个字节 | 当 n 个 int 用时越界写，踩坏别的变量 | `malloc((size_t)n * sizeof(int))` |
| 3 | 类型与 `sizeof` 不一致：`malloc(sizeof(int *))` | 64 位下只有 8 字节，装不下 3 个 int | `sizeof` 里写元素类型：`sizeof(int)` |
| 4 | 只申请不释放（内存泄漏） | 内存占用只涨不降；长跑程序最后吃光内存 | 成对出现：每个 `malloc` 都有对应的 `free` |
| 5 | `free` 后继续用（use after free） | 有时"看着是对的"，有时乱码/崩溃，最难查 | `free(p); p = NULL;`，之后不再碰 `p` |
| 6 | 同一块内存 `free` 两次（double free） | 运行时报错 / abort，堆结构被破坏 | free 后立刻置 NULL；`free(NULL)` 是合法的空操作 |
| 7 | `free(&x)` 或 `free(arr + 1)` | 直接崩：释放了栈地址或非首地址 | `free` 只接收 malloc 家族返回的原始首地址 |
| 8 | `p = realloc(p, new_size)` 直接覆盖 | realloc 失败返回 NULL，旧地址丢失 = 泄漏 | 临时指针接住，成功后再赋回 `p` |
| 9 | 返回局部变量的地址 | 函数一返回那块栈就作废，调用者拿到野指针 | 要活得比函数长，就 `malloc` 到堆上 |
| 10 | 忘写 `#include <stdlib.h>` | 隐式声明，返回值被当 int 截断（64 位下高危） | 文件头写 `#include <stdlib.h>` |
| 11 | 大数组开在栈上 | 编译零警告，运行时崩：实测 4 MB 局部数组退出码 `0xC00000FD`（栈溢出） | 大块内存一律 `malloc`；1 MB 的局部数组本机还能跑，但别赌 |
| 12 | `%zu` / `%lld` 不加选项 | MinGW 下输出乱码或错值 | 加 `-D__USE_MINGW_ANSI_STDIO=1`，或先转成 int 再打印 |

## 四、复习自查清单

- [ ] 说清栈和堆的区别：谁分配、谁回收、活多久、大概多大
- [ ] 不查资料默写 malloc / calloc / realloc / free 的参数与返回值含义
- [ ] 脱口而出：申请 n 个 int 的字节数怎么写？写成 `malloc(n)` 会怎样？
- [ ] 为什么每次 malloc 之后都要检查 NULL？不检查最坏会怎样？
- [ ] 内存泄漏 / 野指针 / 重复释放 / 释放后使用，四个名词各举一个例子
- [ ] 解释 `free(p); p = NULL;` 为什么能一次防住两个坑
- [ ] 为什么 `free(&x)` 和 `free(arr + 1)` 都是错的？
- [ ] realloc 为什么会"搬家"？直接 `p = realloc(p, ...)` 的风险是什么？
- [ ] 手写一遍动态数组的 push（满了翻倍），并解释扩容后旧元素为什么还在
- [ ] 用自己的话说出：为什么链表结点必须 malloc，不能是函数里的局部变量
- [ ] 打印指针为什么要 `(void *)` 强转？本机 `%p` 打印 NULL 长什么样？
- [ ] 实测一遍：把 4 MB 数组开在栈上跑一次，记下退出码

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本笔记不剧透具体规格与通关测试值：题号、输入范围、输出格式、判定用测试值全部在开课当天讲义里给出。

- **HW1 主题：一维动态数组的全生命周期** —— 按输入规模申请、赋值、遍历输出、释放，全程带着 NULL 检查。
- **HW2 主题：calloc 与 realloc 对比实验** —— 亲手验证"calloc 出来是 0"和"realloc 之后旧数据还在"。
- **HW3 主题：结构体数组搬到堆上** —— 用 `malloc` 申请一组记录，用 `->` 访问，最后整块 `free`。
- **HW4（挑战 ★★★）主题：动态数组容器** —— 把模板 3 补成能自动扩容、能报错、能清零的完整小容器，为顺序表章节打样。

## 参考：栈与堆对照与"四大地雷"心法

| 对比项 | 栈 stack | 堆 heap |
| --- | --- | --- |
| 谁分配 / 谁回收 | 编译器自动分配，函数返回时自动回收 | 你 `malloc` 申请，你 `free` 归还；不 free 就一直占着 |
| 生命周期 | 出了作用域就没了 | 从申请到 free，跨函数、跨循环都活着 |
| 大小 | 小，默认几 MB | 大，受限于系统可用内存 |
| 典型错误 | 栈溢出（大数组）、返回局部变量地址 | 泄漏、野指针、重复释放、释放后使用 |

```text
        high address
        +----------------------+
        |  stack (grows down)  |  main() locals, function frames,
        |   int i; double d;   |  freed automatically on return
        +----------------------+
        |  heap (grows up)     |  malloc(...) blocks: YOU call free
        |   [20 bytes] [28 B]  |  no name, only the pointer knows them
        +----------------------+
        |  globals / code      |
        +----------------------+
        low address
```

**四大地雷速查**：内存泄漏（申请了不还）｜ 野指针（free 了还留着地址）｜ 重复释放（同一块 free 两次）｜ 释放后使用（free 了还去读写）。前两个的克星是同一句话：

```c
free(p);
p = NULL;      /* free(NULL) is a safe no-op, and p can never dangle again */
```

> 心法：**谁申请，谁释放；释放完，置 NULL**。L12 写链表时会反复用到这两句。
