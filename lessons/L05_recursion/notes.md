# L5 复习笔记 — 递归入门：三要素 / 调用栈 / 递归 vs 迭代

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- 递归 = 函数自己调用自己；能写递归的前提是「大问题 = **同型**的小问题 + 一步」。
- 递归三要素：**出口（base case）/ 规模缩小 / 自我调用**；出口写在最前面，而且宁可宽一点：`if (n <= 1)` 比 `if (n == 1)` 安全（0、负数都能接住）。
- 每次调用都会往**调用栈**上压一个**栈帧**（形参、局部变量、返回地址），返回时弹出 —— 后进先出，正是 408 里「栈」的应用。
- 递归的执行是「**一路压栈到底 → 触底 → 一路往回算**」，所以阶乘的乘法是倒着做的，`printf` 的顺序也常常是反的。
- 递归深度有上限：本机实测深度 4 万还能跑完，5 万直接栈溢出（退出码 `0xC00000FD`，即 -1073741571）。
- 朴素斐波那契是**指数级**：fib(30) 要 2,692,537 次调用，fib(40) 要 331,160,281 次（本机 0.79 秒）—— 重复子问题被算了无数遍。
- 递归 vs 迭代：能用循环就先写循环（快、省栈）；但树 / 图的遍历、分治、汉诺塔用递归写才自然。
- 汉诺塔 n 个盘要 **2ⁿ − 1** 步（n = 3 → 7 步，实测），而递归写法只有短短几行 —— 这就是递归的威力。
- 写递归先问自己三句：**出口是什么？规模怎么变小？小规模的答案怎么拼成大问题的答案？**
- 408 地位：二叉树的先 / 中 / 后序遍历、求树高与结点数、图的 DFS、快排与归并、汉诺塔，全是递归；不会递归 = 不会写树。

## 二、代码模板

**模板 1：递归三要素（一个函数里全齐）**

```c
/* Recursion's three rules: a base case, a smaller problem, a self call. */
long long factorial(int n)
{
    if (n <= 1)                     /* rule 1: base case -- answer it directly */
    {
        return 1;
    }
    return n * factorial(n - 1);    /* rule 2: smaller input; rule 3: call myself */
}
```

展开看 `factorial(4)`，注意它是**先一路下去、再一路回来**：

```text
factorial(4) = 4 * factorial(3) = 4 * (3 * factorial(2)) = 4 * (3 * (2 * factorial(1)))
             = 4 * (3 * (2 * 1)) = 24      <- the base case unwinds, layer by layer
```

**模板 2：斐波那契 —— 好看但慢（重点在调用次数）**

```c
/* Naive Fibonacci: short code, but the work doubles at every level. */
static long long call_count = 0;    /* counts how many times fib() runs */

long long fib(int n)
{
    call_count++;

    if (n <= 1)
    {
        return n;                   /* fib(0) = 0, fib(1) = 1 */
    }
    return fib(n - 1) + fib(n - 2); /* two smaller problems, not one */
}
```

`main` 里每次调用前把 `call_count` 清零，调用后连结果和次数一起打印。本机实测数据（同一程序打印，此处按行整理）：

```text
calls for n = 0..10 : 1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177
fib(10) = 55 (177 calls), fib(30) = 832040 (2692537 calls)
fib(40) = 102334155 (331160281 calls, about 0.79 s on this machine)
```

- 调用次数满足 `calls(n) = 2 × fib(n + 1) − 1`：多算几个数，工作量成倍地涨。
- 病根是**重复子问题**：fib(30) 里 fib(28) 被算两次、fib(27) 三次……408 里递归求斐波那契要配**记忆化 / 动态规划**才实用。

**模板 3：递归处理数组 —— 求和与最大值（规模从右端缩小）**

```c
/* Recursion over an array: shrink the range from the right end. */
int sum_range(const int values[], int count)
{
    if (count == 0)                 /* empty range: the sum of nothing is 0 */
    {
        return 0;
    }
    return values[count - 1] + sum_range(values, count - 1);
}

int max_range(const int values[], int count)
{
    if (count == 1)                 /* one element left: it must be the maximum */
    {
        return values[0];
    }

    int rest_max = max_range(values, count - 1);

    if (values[count - 1] > rest_max)
    {
        return values[count - 1];
    }
    return rest_max;
}
```

实测：`values[] = {3, 9, 4, 7, 1, 8}` → `sum = 32`、`max = 9`。数组本身是 L6 的内容，这里先借来用。

**模板 4：汉诺塔（完整程序：几行代码解决 2ⁿ − 1 步的问题）**

```c
/* Hanoi: move n disks from one peg to another, never putting a big disk on a small one. */
#include <stdio.h>

static int move_count = 0;

void hanoi(int disks, char from_peg, char to_peg, char via_peg)
{
    if (disks == 1)
    {
        move_count++;
        printf("move disk 1: %c -> %c\n", from_peg, to_peg);
        return;                                     /* base case: one disk left */
    }

    hanoi(disks - 1, from_peg, via_peg, to_peg);    /* step 1: clear the small pile */
    move_count++;
    printf("move disk %d: %c -> %c\n", disks, from_peg, to_peg);
    hanoi(disks - 1, via_peg, to_peg, from_peg);    /* step 3: pile it onto the target */
}

int main(void)
{
    hanoi(3, 'A', 'C', 'B');
    printf("total moves for 3 disks = %d\n", move_count);
    return 0;
}
```

本机实测输出（3 个盘，A → C，B 当辅助柱）：

```text
move disk 1: A -> C
move disk 2: A -> B
move disk 1: C -> B
move disk 3: A -> C
move disk 1: B -> A
move disk 2: B -> C
move disk 1: A -> C
total moves for 3 disks = 7
```

汉诺塔是**分治**的教科书样例：把「n 个盘」拆成「n − 1 个盘搬两次 + 最大盘搬一次」，每一步都在缩小规模，出口是「只剩 1 个盘直接搬」。

**模板 5：递归 vs 迭代（同一个问题的两张脸）**

```c
/* Same job, two styles: loop and recursion must agree on every answer. */
int sum_to_loop(int n)                  /* one frame, no depth limit */
{
    int total = 0;
    for (int value = 1; value <= n; value++)
    {
        total += value;
    }
    return total;
}

int sum_to_rec(int n)                   /* depth = n, that is n + 1 stack frames */
{
    if (n <= 0)
    {
        return 0;                       /* the empty sum */
    }
    return n + sum_to_rec(n - 1);
}
```

实测：n = 0…5 两种写法逐项相同，`n = 100` 时都是 `5050`。
**怎么选**：循环快、只占一个栈帧、不怕深度；递归代码短、贴近问题的定义（树 / 图 / 分治几乎只能递归）。408 卷面上，递归写得出、复杂度算得清，就够用了。

**模板 6：把调用栈画出来 —— 缩进打印法**

```c
/* Watch the call stack: every call prints on the way in and again on the way out. */
void countdown(int n, int depth)
{
    print_indent(depth);            /* helper: prints ".." depth times */
    printf("enter countdown(%d)\n", n);

    if (n > 0)
    {
        countdown(n - 1, depth + 1);
    }
    else
    {
        print_indent(depth);
        printf("base case: stop here\n");
    }
    print_indent(depth);
    printf("leave countdown(%d)\n", n);
}
```

完整程序另有一个 `print_indent` 辅助函数，`main` 里调用 `countdown(3, 0)`；本机实测输出（缩进 = 栈的深度）：

```text
enter countdown(3)
..enter countdown(2)
....enter countdown(1)
......enter countdown(0)
......base case: stop here
......leave countdown(0)
....leave countdown(1)
..leave countdown(2)
leave countdown(3)
```

- 前 4 行是**压栈**（越进越深），后 5 行是**弹栈**（原路返回），完全是**后进先出**。
- 调试递归的土办法：加一个 `depth` 参数打印缩进，一眼就能看出「进去了几层、停在哪一层」。

## 三、坑清单（L5 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 忘了写出口（base case），或出口太窄（`n == 1`） | 无限递归 → 栈溢出；传入 0 或负数时漏网 | 开头先判最小情形：`n <= 1` / `count == 0` |
| 2 | 规模没缩小（`f(n)` 里又调 `f(n)`） | 死递归，参数永远不变 | 每次调用参数必须**严格变小**：`n - 1` |
| 3 | 写成 `factorial(n--)` | 传进去的还是原值 → 死递归 | 用 `factorial(n - 1)`，别在实参里写自增自减 |
| 4 | 递归分支漏 `return` | 上层拿到垃圾值（`-Wall` 报 `control reaches end of non-void function`） | 每个 `if / else` 分支都 `return` |
| 5 | 递归太深（几万层以上） | 栈溢出：本机实测 4 万层能过、5 万层崩（`0xC00000FD`） | 改成循环，或把深度控制在合理范围 |
| 6 | 用 `int` 存阶乘 / 斐波那契 | 13! 起就溢出成负数或乱码 | 用 `long long` + `%lld`（MinGW 记得加宏） |
| 7 | 朴素递归求斐波那契 | 指数级爆炸：fib(40) 要 3.3 亿次调用 | 记忆化，或改成循环递推 |
| 8 | 在递归里用全局 / static 变量「累加」 | 第二次调用接着上次累，越算越离谱 | 中间结果靠**参数传下去、返回值带上来** |
| 9 | 汉诺塔参数顺序写错（from / to / via 混） | 不报错，但移动序列全错 | 先用注释定好三个柱子的角色，再照着填实参 |

## 四、复习自查清单

- [ ] 不查资料默写：阶乘递归、斐波那契递归、数组求和 / 最大值递归
- [ ] 说出递归三要素，并能指出一段递归代码缺了哪一要素
- [ ] 手算 `factorial(4)` 的展开，并画出 `fib(5)` 的调用树
- [ ] 解释「一路压栈到底、再一路往回算」，并说明为什么打印顺序会反过来
- [ ] 给一段递归代码，数出它的调用次数与递归深度（如 fib(5) 几次调用？）
- [ ] 为什么 fib(30) 只有 30 层深，却要 269 万次调用？「层数」和「次数」区别在哪？
- [ ] 汉诺塔：n 个盘为什么是 2ⁿ − 1 步？n = 3 的 7 步能背着写出来吗？
- [ ] 本机递归深度大概到多少会栈溢出？栈溢出时的退出码是多少？
- [ ] 同一个求和问题，循环版和递归版各写一遍，并说出各自的优劣
- [ ] 口答：408 里哪些算法离不开递归？（树遍历、图 DFS、分治、汉诺塔…）

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本节只给**主题**：具体输入输出格式与通关测试值随讲义下发，避免提前抄答案。

- 用递归实现阶乘 / 求和，先用纸笔写出「出口 + 规模缩小」再敲代码。
- 用递归求解斐波那契，并观察 n 变大时调用次数的增长（体会重复子问题）。
- 用递归处理数组：求最大值 / 求和，体会「把区间从右端缩小」的套路。
- 挑战：汉诺塔或「递归求最大公约数」，要求打印出每一步的过程。

## 参考：`factorial(3)` 的栈帧变化

| 时刻 | 调用栈（栈底 → 栈顶） | 说明 |
| --- | --- | --- |
| 1 | `main` → `factorial(3)` → `factorial(2)` | 一路压栈，n 依次减 1（每层都有自己的 n） |
| 2 | … → `factorial(1)` | 触底：命中出口 `n <= 1`，直接返回 1，不再压栈 |
| 3 | 逐层返回：2 × 1 = 2 → 3 × 2 = 6 | 每弹一层算一次乘法，最后回到 main |
