# L4 复习笔记 — 函数：定义 / 声明 / 值传递 / 作用域 / static / 模块化

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- 函数三件套：**声明（原型）→ 定义 → 调用**；原型是给编译器看的「名片」，必须出现在**第一次调用之前**，定义才是真正的函数体。
- C 的传参**只有值传递**：形参拿到的是实参的**副本**，函数里改形参，外面的实参一动不动（模板 2 实测）。
- 形参 / 实参要**个数、类型、顺序**一一对上；对不上时 C 常做隐式转换，于是「不报错，但算错」。
- 想让函数改动外面的变量，目前只有一条正路：**把结果 return 出来**；传地址（指针）留到 L7。
- `return` 干两件事：**交回一个值** + **立刻结束本函数**；提前 return 能把嵌套 if 拉平（守卫子句）。
- 非 void 函数**每条路径都要 return**；漏了编译器报 `control reaches end of non-void function`，返回值是垃圾。
- 局部变量：作用域 = 从定义处到所在 `{}` 结束；生命周期 = 一次调用，函数返回就销毁。
- 全局变量：所有函数都能读能写 → 出问题查不出「谁改的」；408 代码一律用**参数和返回值**传数据。
- 同名遮蔽（shadow）：内层盖外层、局部盖全局，**不报错只出错值** —— 名字取清楚就不会踩。
- `static` 局部变量只初始化一次，函数返回后**值还留着**（生命周期 = 整个程序），下次调用接着用。
- `int f(void)` 才是「没有参数」；C 里 `int f()` 表示「参数表未知，编译器不检查」—— 一律写 `(void)`。
- 模块化：一个函数只干一件事，`main` 只负责调度；这是后面写顺序表 / 链表 / 树的基本功。

## 二、代码模板

**模板 1：函数三件套（原型 → 定义 → 调用）**

```c
/* The three pieces of every function: prototype, definition, call. */
int max_of_two(int first, int second);      /* 1. prototype: every call is checked against it */

int main(void)
{
    int best = max_of_two(7, 12);           /* 3. call: arguments are COPIED into the parameters */
    ...
}

int max_of_two(int first, int second)       /* 2. definition: the real body lives here */
{
    if (first > second)
    {
        return first;
    }

    return second;
}
```

**模板 2：值传递（完整程序 —— `swap` 为什么换不了）**

```c
/* Pass by value: the callee only gets COPIES, so swap cannot touch main's variables. */
#include <stdio.h>

void swap_by_value(int left, int right);

int main(void)
{
    int a = 1;
    int b = 2;

    printf("before: a = %d, b = %d\n", a, b);
    swap_by_value(a, b);
    printf("after : a = %d, b = %d\n", a, b);

    return 0;
}

void swap_by_value(int left, int right)
{
    int temp = left;
    left = right;
    right = temp;

    printf("inside: left = %d, right = %d\n", left, right);
}
```

本机实测输出（gcc 8.1.0，`gcc -Wall -Wextra` 零警告）：

```text
before: a = 1, b = 2
inside: left = 2, right = 1
after : a = 1, b = 2
```

`inside` 说明函数内部确实换成功了，`after` 说明 `main` 的 a、b 没变 —— 它换的是两份**副本**。

**模板 3：早返回（early return）与守卫子句（guard clause）**

```c
/* return does two jobs: hand a value back, and leave the function right away. */
int abs_value(int number)
{
    if (number < 0)
    {
        return -number;         /* early return: the line below is skipped */
    }

    return number;
}

int count_divisors(int number)
{
    if (number <= 0)
    {
        return 0;               /* guard clause: reject impossible input first */
    }

    int count = 0;

    for (int divisor = 1; divisor <= number; divisor++)
    {
        if (number % divisor == 0)
        {
            count++;
        }
    }

    return count;
}
```

实测：`abs(-5) = 5`、`abs(5) = 5`、`12 has 6 divisors`、`0 has 0 divisors (guard clause)`。

**模板 4：`static` 局部变量（记住上次的值）**

```c
/* static local: created once, keeps its value between calls. */
int next_id(void)
{
    static int counter = 0;     /* initialized only once, before main runs */

    counter++;

    return counter;
}
```

实测：连续调用三次 → `id = 1`、`id = 2`、`id = 3`；把 `static` 去掉（`int counter = 0;`），三次输出全是 `id = 1`。

**模板 5：作用域与遮蔽（谁盖住了谁）**

```c
/* Scope: an inner name hides (shadows) the outer one with the same spelling. */
int total = 100;                /* global: every function below can see it */

void show_shadow(void)
{
    int total = 7;              /* local: hides the global inside this function */

    printf("inside show_shadow  : total = %d\n", total);
}
```

`main` 里先 `total = total + 1;`，再 `show_shadow();`，然后开一个块写 `int total = 999;`，实测输出：

```text
main, before change : total = 100
main, after change  : total = 101
inside show_shadow  : total = 7
inside a block      : total = 999
main, at the end    : total = 101
```

注意最后一行还是 101：块里的 `total = 999` 和函数里的 `total = 7` 都是**局部副本**，动不了全局。

**模板 6：模块化 —— 一个函数一件事（完整程序，用到 `long long` / `%lld`）**

```c
/* Modular thinking: each function does one small job, main just wires them together. */
#include <stdio.h>

long long factorial(int n)
{
    long long product = 1;

    for (int factor = 2; factor <= n; factor++)
    {
        product *= factor;
    }

    return product;
}

int digit_sum(int number)
{
    int sum = 0;

    while (number > 0)
    {
        sum += number % 10;
        number /= 10;
    }

    return sum;
}

int main(void)
{
    printf("13! = %lld\n", factorial(13));
    printf("digit sum of 123456 = %d\n", digit_sum(123456));

    return 0;
}
```

实测输出：

```text
13! = 6227020800
digit sum of 123456 = 21
```

- 13! = 6227020800 已超过 int 上限（2147483647），所以返回类型必须是 `long long`，printf 必须用 `%lld`。
- **MinGW 坑**：`%lld`（以及 `%zu`）在 MinGW-W64 8.1.0 上要加宏才干净：`gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 ...`。
  不加宏时会稳定报 `warning: unknown conversion type character 'l' in format [-Wformat=]` 和 `too many arguments for format`；
  本机实测不加宏也侥幸打印正确，但**别赌**——有些 MinGW 版本会打出乱码或截断值。

## 三、坑清单（L4 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 调用前既没原型也没定义 | C99 起是 error（旧标准只给 `implicit declaration` 警告） | 文件顶部先写原型，或把定义放前面 |
| 2 | 原型与定义的参数类型 / 个数不一致 | 编译能过，运行时参数错位、结果离谱 | 改完定义顺手改原型，两处逐字对齐 |
| 3 | 非 void 函数漏 return | 返回值是垃圾值；`-Wall` 报 `control reaches end of non-void function` | 每条分支都 `return`，末尾补一条兜底 return |
| 4 | 以为函数能改实参（值传递） | `swap(a, b)` 后 a、b 没变 | 把结果 `return` 出去，或传地址（L7 指针） |
| 5 | 形参复制一份大数组 | 每调一次都拷一遍，慢且容易被误解 | 大数组加 `const` 传首地址，另配元素个数参数 |
| 6 | `int f()` 当「没有参数」 | C 里它是「参数表未知」，写错参数也不报警 | 没有参数就写 `int f(void)` |
| 7 | 函数返回类型与 return 表达式不匹配 | double 函数返回 int 表达式被截断；大数用 int 返回溢出 | 返回类型与表达式类型一致（大数用 `long long`） |
| 8 | 局部变量名和全局变量同名 | 不报错，改的其实是局部副本，全局没变 | 全局变量加 `g_` 前缀，或干脆别用全局变量 |
| 9 | 以为 static 局部变量每次调用都重新初始化 | 初值只在程序启动时给一次，值会累积 | 要「清零」就写普通局部变量 |
| 10 | 在函数里定义函数（嵌套函数） | gcc 扩展能过，但不是标准 C，换编译器就废 | 所有函数定义都写在文件层（互不嵌套） |
| 11 | 改了函数名却没改原型 / 调用处 | 链接错误 `undefined reference to xxx` | 用编辑器全局改名，然后重新编译 |
| 12 | 全局变量满天飞 | 一处被改，全程序遭殃，调试时无从下手 | 数据靠参数进、靠返回值出（模块化的核心） |

## 四、复习自查清单

- [ ] 不查资料默写：函数原型 + 定义 + 调用三件套，并写一个「最大值函数」
- [ ] 解释为什么 `swap_by_value(a, b)` 换不了实参，说出「副本」两个字的含义
- [ ] 口答：`return` 有哪两个作用？举一个用早返回把嵌套 if 拉平的例子
- [ ] 非 void 函数漏 return 会怎样？编译器给什么警告？
- [ ] 说清「作用域」和「生命周期」的区别，各举一个例子
- [ ] 全局变量被局部同名变量遮蔽时，函数里改的是哪一个？为什么最后全局没变？
- [ ] `static int counter = 0;` 放在函数里，连续调用三次分别返回什么？去掉 static 呢？
- [ ] `int f()` 与 `int f(void)` 在 C 里有什么区别？该写哪个？
- [ ] 写出「判断某个数是不是素数」的函数原型，并在 `main` 里调用它
- [ ] 为什么 13! 要 `long long` + `%lld`？MinGW 下编译要加哪个宏？
- [ ] 手敲模板 2 和模板 6，零警告编译并运行，输出与笔记一致

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本节只给**主题**：具体输入输出格式与通关测试值随讲义下发，避免提前抄答案。

- 用函数封装「求两个整数的最大公约数 / 最小公倍数」，`main` 只负责读入和打印。
- 写一个「判断素数」的函数，并让另一个函数调用它来统计一个区间内的素数个数。
- 用函数拆分「数位分解 / 数字反转」这类小算法，体会一个函数只做一件事。
- 挑战：把「阶乘 / 斐波那契」的循环版本抽成函数，注意返回类型与占位符要配套。

## 参考：一次函数调用的完整过程

| 步骤 | 发生什么 |
| --- | --- |
| 1 | `main` 把实参**求值**，按值复制给形参（新的一份内存） |
| 2 | 压入一个新的**栈帧**（参数、局部变量、返回地址）—— 递归讲栈帧时还会用到 |
| 3 | 执行函数体，遇到 `return` 立即结束本函数（后面的语句不再执行） |
| 4 | 返回值放进约定位置，栈帧被弹出，控制权回到调用点的下一行 |
| 5 | 局部变量随栈帧一起消失；`static` 变量不在栈上，所以能留到下次调用 |
