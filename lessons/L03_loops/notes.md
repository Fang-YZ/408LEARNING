# L3 复习笔记 — 循环：while / for / 嵌套 / break · continue

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 示例源码：lessons/L03_loops/examples/（ex1_sum_for / ex2_stars_triangle / ex3_break_continue）

## 一、30 秒速览

- 循环三要素：**起点（初始化）→ 终点（条件）→ 推进（步进）**，缺一个就死循环或空转。
- `for (int i = 1; i <= n; i++)`：数 1..n 共 n 个数；`i < n` 从 0 开始则是 0..n-1 也是 n 个。
- `while (条件) { ... }`：适合"不知转几圈"的场景（数字分解、读输入直到结束）。
- `do { ... } while (条件);`：至少执行一次，**while 后面必须有分号**。
- 三大套路：**累加** `sum += x`（初值 0）、**累乘** `product *= i`（初值 **1**）、**最值** `if (x > best) best = x`。
- 数字分解套路：`while (n > 0) { digit_sum += n % 10; n /= 10; }` —— `%10` 取个位，`/10` 去掉个位。
- `break` = 立刻跳出整个循环（找到一个答案就够时用）；`continue` = 跳过本轮剩余语句，直接进下一轮。
- break / continue 只作用于**最内层**循环；嵌套里想跳两层要用标志变量。
- 边界必测：**n = 0、n = 1、最大值**（"循环体一次都不执行"最容易出错）。

## 二、代码模板

**模板 1：计数 for（最常用）**

```c
for (int i = 1; i <= n; i++)
{
    sum += i;
}
```

**模板 2：while 数字分解**

```c
while (n > 0)
{
    digit_sum += n % 10;    /* take the last digit */
    n /= 10;                /* drop the last digit */
    digits++;
}
```

**模板 3：素数判定（带 break）**

```c
int divisor = 0;

for (int d = 2; d <= n / 2; d++)
{
    if (n % d == 0)
    {
        divisor = d;
        break;              /* one divisor is enough: stop early */
    }
}

if (divisor == 0)
{
    printf("%d is prime\n", n);
}
```

**模板 4：嵌套循环（外层行、内层列）**

```c
for (int row = 1; row <= h; row++)
{
    for (int space = 1; space <= h - row; space++)
    {
        printf(" ");                       /* leading spaces */
    }
    for (int star = 1; star <= 2 * row - 1; star++)
    {
        printf("*");                       /* stars: 1, 3, 5, 7 ... */
    }
    printf("\n");                          /* end of this row */
}
```

**模板 5：do-while（至少执行一次）**

```c
int value = 0;

do
{
    printf("Enter a positive number: ");
    scanf("%d", &value);
} while (value <= 0);
```

## 三、坑清单（L3 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | `for (...);` 多写一个分号 | 循环空转，后面语句只执行一次 | 删掉分号，`{` 紧跟 `)` |
| 2 | while 里忘记推进变量 | 死循环，终端卡住 | 循环体内写 `i++`，或改用 for |
| 3 | while 里 `continue` 跳过了 `i++` | 死循环（for 无此问题：`i++` 仍在执行） | while 中把推进语句放最前，或用 for |
| 4 | 累乘初值写成 0 | 结果永远是 0 | `long long product = 1;` |
| 5 | 累加变量没初始化 | 垃圾值，每次运行结果不同 | `int sum = 0;` |
| 6 | off-by-one：`<=` 与 `<` 混淆 | 多算/少算一次 | 数 1..n 用 `i = 1; i <= n`；数组下标用 `i = 0; i < n` |
| 7 | 没测 n = 0 / n = 1 | 循环一次不执行时输出错 | 先手算最小边界再跑 |
| 8 | int 溢出（阶乘、大和） | 结果变负或错乱 | 用 `long long`；MinGW 下 `%lld` 要配 `-D__USE_MINGW_ANSI_STDIO=1` |
| 9 | 嵌套里用 break 想跳出两层 | 只跳出内层 | 用标志变量 `int done = 0;` 或抽成函数 `return` |
| 10 | 打印图形忘 `\n` | 所有行挤成一行 | 每行结束 `printf("\n");` |
| 11 | 死循环了不知道怎么停 | 终端没反应 | 按 **Ctrl + C** 强制中断 |

## 四、复习自查清单

- [ ] 不查资料默写：计数 for、数字分解 while、素数判定（带 break）、嵌套图形
- [ ] 说出循环三要素，并能指出某段代码缺了哪一要素
- [ ] `i = 0; i < n` 与 `i = 1; i <= n` 各循环几次？分别适合什么场景？
- [ ] `break` 与 `continue` 的区别，各举一个实际例子
- [ ] 累加初值为什么是 0，累乘初值为什么是 1？
- [ ] 手算 `n = 12345` 的数字分解过程（每一步的 `n % 10` 与 `n /= 10`）
- [ ] 为什么 n = 0 时"数字位数"要特判成 1？
- [ ] 20! 是多少？为什么必须用 long long？
- [ ] 嵌套循环里，外层次数与内层次数怎么相乘得到总执行次数？
- [ ] 调试手段：打印中间值、看警告、测边界、Ctrl+C 停死循环——四样都会

## 五、作业清单（含通关测试值）

> 存盘位置：`E:\learn408\homework\L03\` ｜ 编译：`gcc -Wall -Wextra`（用到 `%lld` 的加 `-D__USE_MINGW_ANSI_STDIO=1`）
> 通关线：HW1–HW3 全部 ✅ 解锁 L4（函数）；HW4 ★★★ 挑战不计门槛。
> 判分只看关键输出行（printf 提示语不计）。

### HW1（必做 ★）阶乘 n! — hw1_factorial.c

- 输入 int `n`（1 ≤ n ≤ 20），输出一行 `n! = 结果`（如 `5! = 120`）。
- 必须用循环累乘，且用 `long long` 保存结果。
- 编译（MinGW 配方，因为用了 `%lld`）：
  `gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 hw1_factorial.c -o hw1_factorial.exe`

| 输入 | 期望输出 |
| --- | --- |
| 1 | 1! = 1 |
| 2 | 2! = 2 |
| 5 | 5! = 120 |
| 10 | 10! = 3628800 |
| 13 | 13! = 6227020800 |
| 20 | 20! = 2432902008176640000 |

### HW2（必做 ★★）素数判定 — hw2_prime.c

- 输入 int `n`（2 ≤ n ≤ 1000000）。
- 若 n 是素数：`n is prime`；否则：`n is not prime, smallest divisor = d`（d 为最小因子）。
- 要求：找到第一个因子后 **break 提前结束**（别白跑完全部）。
- 提示：循环上界用 `d <= n / 2` 即可（想更快可以试 `d * d <= n`）。

| 输入 | 期望输出 |
| --- | --- |
| 2 | 2 is prime |
| 3 | 3 is prime |
| 4 | 4 is not prime, smallest divisor = 2 |
| 91 | 91 is not prime, smallest divisor = 7 |
| 97 | 97 is prime |
| 999983 | 999983 is prime |
| 999999 | 999999 is not prime, smallest divisor = 3 |
| 1000000 | 1000000 is not prime, smallest divisor = 2 |

- 验算方法：91 = 7 × 13（最小因子 7）；999999 = 3 × 333333。

### HW3（必做 ★★）数位分解 — hw3_digit_sum.c

- 输入 int `n`（0 ≤ n ≤ 2147483647）。
- 输出一行：`digits = 位数, digit sum = 各位数字之和`。
- 要求：用 `while (n > 0)` 循环 + `%10` / `/10`；**注意 n = 0 的特判**（0 是 1 位数，数字和为 0）。

| 输入 | 期望输出 |
| --- | --- |
| 0 | digits = 1, digit sum = 0 |
| 5 | digits = 1, digit sum = 5 |
| 10 | digits = 2, digit sum = 1 |
| 999 | digits = 3, digit sum = 27 |
| 12345 | digits = 5, digit sum = 15 |
| 1000000000 | digits = 10, digit sum = 1 |
| 2147483647 | digits = 10, digit sum = 46 |

### HW4（挑战 ★★★）等腰三角形 — hw4_triangle.c

- 输入 int `h`（1 ≤ h ≤ 9）。
- 第 i 行（i 从 1 到 h）：先打印 `h - i` 个空格，再打印 `2 * i - 1` 个星号，最后换行。
- 输出形状（h = 5）：

```
    *
   ***
  *****
 *******
*********
```

| 输入 | 星号个数（每行） |
| --- | --- |
| 1 | 1 |
| 2 | 1, 3 |
| 3 | 1, 3, 5 |
| 4 | 1, 3, 5, 7 |
| 9 | 1, 3, 5, 7, 9, 11, 13, 15, 17 |

> 自动化脚本会把空格折叠（只校验星号图案），**前导空格请你用眼睛确认**。

## 六、调试技巧（L3 新增：这是本课第二重点）

1. **打印中间值法**：怀疑循环算错时，在循环体里加一行
   `printf("debug: i = %d, sum = %d\n", i, sum);`，看清每一步，查完删掉；
2. **边界先行**：先跑 n = 0、n = 1、最大值三组，再看常规值；
3. **警告当错误**：`-Wall -Wextra` 零警告是底线；
4. **死循环逃生**：终端卡住按 **Ctrl + C**；
5. **批量回归**：`tools\check.ps1` 一键跑完全部测试值。

## 七、测试命令（L3 版）

```
cd E:\learn408\homework\L03
gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 hw1_factorial.c -o hw1_factorial.exe
..\..\tools\check.ps1 -Exe .\hw1_factorial.exe -Tests ..\..\tools\tests\hw1_factorial.txt
```

（hw2 / hw3 / hw4 不带 `%lld`，编译命令去掉 `-D__USE_MINGW_ANSI_STDIO=1` 即可。）

> 参考答案在 `lessons/L03_loops/solutions/`，做完再看。
