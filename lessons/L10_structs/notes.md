# L10 复习笔记 — 结构体（struct）

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- `struct` 是 C 里**唯一能自己造类型**的手段：把描述同一个事物的若干变量打包成一个整体，这个整体就是数据结构里的"数据元素"。
- 定义格式 `struct Student { int id; char name[20]; int score; };` —— **结尾分号千万不能丢**，丢了报 `expected ';'`，还常常连累后面一整段代码。
- 三种初始化：按声明顺序 `{1001, "Alice", 92}` ｜ 指定成员 `{.id = 1001, .name = "Alice", .score = 92}` ｜ 全清零 `{0}`。
- 访问成员两兄弟：**变量用 `.`**（`s1.id`），**指针用 `->`**（`ptr->id`）；`(*ptr).id` 与 `ptr->id` 等价，而 `*ptr.id` 是**错的**（`.` 优先级高于 `*`）。
- `typedef struct { ... } Point;` 起别名后就能省掉 `struct` 关键字；`typedef struct Student Student;` 是给标签起短名。王道教材用的是它的加强版（见 L12）。
- `s2 = s1;` 是**整块逐成员拷贝**（浅拷贝）：数组成员一起被复制，但结构体里的**指针只复制地址** —— 拷贝后两个结构体指向同一块内存。
- 结构体数组：`Point path[3] = {{0, 0}, {1, 2}, {3, 4}};`，用 `path[i].x` 访问；数组名退化成首元素指针，所以 `(pp + 1)->y` 就是 `path[1].y`。
- 作函数参数：**值传递 = 整块拷贝**（函数里改不动实参，结构体大时还费时间）；要改实参就传地址 `&s1`，形参写 `struct Student *`，函数里用 `->`。
- 结构体数组作参数**一定退化成指针**，函数里 `sizeof` 拿不到长度 —— 长度必须另开一个参数传进去。
- 内存有**填充（padding）**：成员按自身大小对齐，`sizeof` 常比"成员大小之和"大。实测 `{ char; int; char; }` 占 12 字节，成员从大到小重排后只要 8 字节。
- 结构体**不能直接 `==` 比较**（`invalid operands to binary ==`）；要逐成员比较，字符串用 `strcmp`。
- `enum` 本质是 int（编号从 0 或指定值起递增），`union` 的所有成员**共用同一块内存**、大小取最大成员 —— 都是"给数据元素贴类型标签"的工具。

## 二、代码模板

**模板 1：定义 + 三种初始化 + `.` 访问**

```c
#include <stdio.h>

struct Student
{
    int id;
    char name[20];
    int score;
};

int main(void)
{
    struct Student s1 = {1001, "Alice", 92};                        /* by order */
    struct Student s2 = {.id = 1002, .name = "Bob", .score = 85};   /* by name */
    struct Student s3 = {0};                                        /* all zero */

    struct Student s4 = s1;      /* copy every field */
    s4.score = 60;               /* s1 is untouched */

    printf("s1: %d %s %d\n", s1.id, s1.name, s1.score);
    printf("s2: %d %s %d\n", s2.id, s2.name, s2.score);
    printf("s3: %d [%s] %d\n", s3.id, s3.name, s3.score);
    printf("s4: %d %s %d, and s1.score is still %d\n",
           s4.id, s4.name, s4.score, s1.score);

    return 0;
}
```

**模板 2：`.` 与 `->`：变量用点，指针用箭头**

```c
struct Student s1 = {1001, "Alice", 92};
struct Student *ptr = &s1;

printf("dot   : %d\n", s1.id);        /* a variable uses the dot */
printf("arrow : %d\n", ptr->id);      /* a pointer uses the arrow */
printf("same  : %d\n", (*ptr).id);    /* the same as ptr->id, but ugly */
printf("ptr   = %p\n", (void *)ptr);  /* always cast to void * for %p */
```

**模板 3：typedef struct 与结构体数组**

```c
typedef struct
{
    int x;
    int y;
} Point;                       /* from now on: Point p1; not struct Point p1; */

Point path[3] = {{0, 0}, {1, 2}, {3, 4}};

for (int i = 0; i < 3; i++)
{
    printf("path[%d] = (%d, %d)\n", i, path[i].x, path[i].y);
}

Point *pp = path;                            /* the array name decays to &path[0] */
printf("(pp + 1)->y = %d\n", (pp + 1)->y);   /* this is path[1].y */
```

**模板 4：结构体作函数参数 —— 值传递 vs 指针传递**

```c
/* value parameter: the function works on a COPY */
void add_bonus_by_value(struct Student s, int bonus)
{
    s.score += bonus;          /* the caller's struct does NOT change */
}

/* pointer parameter: the function edits the caller's own struct */
void add_bonus_by_pointer(struct Student *s, int bonus)
{
    s->score += bonus;         /* the caller's struct really changes */
}

/* a struct array parameter is always a pointer: pass the length too */
void print_all(const struct Student *list, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("[%d] %d %s %d\n", i, list[i].id, list[i].name, list[i].score);
    }
}

/* how to call them */
add_bonus_by_value(s1, 10);       /* s1.score unchanged */
add_bonus_by_pointer(&s1, 10);    /* s1.score += 10 */
print_all(class_list, 3);
```

**模板 5：enum 与 union（够用版）**

```c
enum Weekday
{
    MON = 1, TUE, WED, THU, FRI, SAT, SUN      /* MON = 1, WED = 3, SUN = 7 */
};

enum Weekday today = WED;
printf("today = %d\n", (int)today);            /* an enum IS an int */

switch (today)
{
    case WED:
        printf("Wednesday\n");
        break;
    default:
        printf("some other day\n");
        break;
}

union IntBytes
{
    int value;
    unsigned char bytes[4];       /* both members share the same 4 bytes */
};

union IntBytes u;
u.value = 0x01020304;
printf("%02X %02X %02X %02X\n", u.bytes[0], u.bytes[1], u.bytes[2], u.bytes[3]);
```

## 三、坑清单（L10 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | struct 定义结尾漏分号 | `error: expected ';'`，报错行常常指向下一行，看着莫名其妙 | `struct Student { ... };` 花括号后立刻写分号 |
| 2 | 用了 `Student` 却只写了 `struct Student` 的定义（没 typedef） | `unknown type name 'Student'` | 先 `typedef struct Student Student;`，或者老实写全 `struct Student` |
| 3 | 想用 `*ptr.id` 访问成员 | 编译错或取到垃圾值（先算 `ptr.id`） | `(*ptr).id` 或直接 `ptr->id` |
| 4 | 值传递函数里改结构体 | 函数内打印是改过的，回到 main 一看没变 | 要改实参就传地址：`f(&s1)`，形参 `struct Student *s` |
| 5 | `scanf` 读成员漏 `&`：`scanf("%d", s.id)` | 崩溃或写入垃圾地址 | `scanf("%d", &s.id)`；只有字符数组名例外 |
| 6 | `scanf("%s", s.name)` 不限宽 | 输入超长就踩坏后面的成员（栈溢出） | `scanf("%19s", s.name)`，宽度 = 数组长度 - 1 |
| 7 | 两个结构体直接 `==` 比较 | `invalid operands to binary ==` | 逐成员比；字符串用 `strcmp(a.name, b.name) == 0` |
| 8 | 把结构体数组传进函数后 `sizeof` 求长度 | 得到的是指针大小（8 字节），不是数组长度 | 长度单独作参数传：`print_all(list, n)` |
| 9 | 初始化顺序与定义顺序不一致 | 静默错位，`92` 跑到 `name` 里去了 | 用指定初始化器 `.id = ...`，或严格照声明顺序写 |
| 10 | 手算 sizeof 而不测 | 忘了 padding，算出的字节数和实际不符 | 用 `sizeof`；布局敏感时成员按大小从大到小排 |
| 11 | 想用 `%s` 打印 enum | 崩溃（enum 是 int，不是字符串） | 用 `%d`，或先用 switch 把 enum 映射成字符串 |

## 四、复习自查清单

- [ ] 不查资料默写：struct 定义、三种初始化、`.` 与 `->` 各一次
- [ ] 说清 `(*ptr).id`、`ptr->id`、`*ptr.id` 谁对谁错，为什么
- [ ] 解释为什么 `s4 = s1` 之后改 `s4.score` 不影响 `s1`；如果结构体里有个 `int *p` 呢？
- [ ] 不看笔记写出「值传递改不动、指针传递改得动」两个函数并各跑一次
- [ ] 说清结构体数组作参数时 `sizeof` 为什么失效，长度该怎么传
- [ ] 解释 padding：`{ char; int; char; }` 为什么是 12 字节，怎么排能缩到 8
- [ ] 用 `offsetof` 打印出自己定义的某个结构体每个成员的偏移
- [ ] 口答：enum 的第一个值默认是几？`MON = 1` 之后 `WED` 是几？
- [ ] 解释 union 为什么大小等于最大成员，以及"写一个成员会影响另一个成员"
- [ ] 结构体直接用 `==` 报什么错？正确的比较写法是什么？
- [ ] 手敲模板 1–5 各跑一次，全程 `-Wall -Wextra` 零警告
- [ ] 用自己的话说出：为什么数据结构要用结构体定义"数据元素"

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本笔记不剧透具体规格与通关测试值：题号、输入范围、输出格式、判定用测试值全部在开课当天讲义里给出。

- **HW1 主题：定义并使用第一个结构体** —— 结构体定义、三种初始化、`.` 成员访问、按格式输出一条完整记录。
- **HW2 主题：结构体数组 + 统计** —— 一组记录上做求和 / 求最值 / 按条件筛选，练"数组 + 结构体"的组合拳。
- **HW3 主题：指针参数 + `->`** —— 把前面写的处理逻辑改成接收指针的函数，在函数内部就地修改调用者的结构体。
- **HW4（挑战 ★★★）主题：struct + enum 重构旧题** —— 把 L2 / L3 的小工具重写成"一条记录"驱动的形式，为数据结构章节的类型定义打样。

## 参考：结构体内存布局与"数据元素"

以本课实测程序（`gcc -Wall -Wextra` 零警告）的真实输出为准：

```text
struct Padded { char tag; int value; char code; };
  sizeof            = 12
  offsetof(tag)     = 0
  offsetof(value)   = 4
  offsetof(code)    = 8
struct Packed { int value; char tag; char code; };
  sizeof            = 8
  offsetof(value)   = 0
  offsetof(tag)     = 4
  offsetof(code)    = 5
same fields, different order: 12 bytes vs 8 bytes
```

同样三个成员，换个顺序就从 12 字节变 8 字节。图示：

```text
struct Padded { char tag; int value; char code; };      sizeof = 12
offset   0    1  2  3    4  5  6  7    8    9 10 11
       +----+----------+------------+----+-----------+
       |tag | padding  |   value    |code|  padding  |
       +----+----------+------------+----+-----------+
         1B      3B          4B       1B       3B

struct Packed { int value; char tag; char code; };      sizeof = 8
offset   0  1  2  3    4    5     6  7
       +------------+----+----+---------+
       |   value    |tag |code| padding |
       +------------+----+----+---------+
             4B        1B   1B     2B
```

- 规则一句话：**每个成员都要放在"自身大小的整数倍"偏移上**，结构体总大小再按最大成员对齐。
- 为什么要讲这个：408 里"结点的存储密度""结构体占几个字节"都要看它；也是 C++ 里 `class` 布局的第一层直觉。
- 顺手的工具：`#include <stddef.h>` 后用 `offsetof(struct Student, score)` 直接把偏移打出来，别靠脑补。

**为什么数据结构要用结构体定义"数据元素"**

408 里一个"数据元素"从来不是单个 int，而是一组属性：

```text
sequential list element : value + where it is stored
linked list node        : data + pointer to the next node
binary tree node        : data + left child + right child
```

数组只能装"同一种类型的格子"，只有 struct 能把不同类型的属性捆成一个整体，还能**整体赋值、整体传参、整体 malloc**：顺序表元素是「数组 + 长度」，链表结点是「data + next」，二叉树结点是「data + 左右孩子」，图的顶点和边也是同一招。

一句话记牢：**结构体是后面所有数据结构的砖头** —— 砖头没砌正，链表、树、图全会歪。
