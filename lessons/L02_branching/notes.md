# L2 复习笔记 — 分支与流程控制：if / else / 逻辑运算 / switch

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-09 ｜ 示例源码：lessons/L02_branching/examples/（ex1_sign_parity / ex2_range_shortcircuit / ex3_switch_weekday）

## 一、30 秒速览

- 关系运算（`==` `!=` `<` `>` `<=` `>=`）的结果是 int：**1 真 / 0 假**；C 里任何**非 0 都算真**。
- 条件三兄弟：`if (条件) { } else if (条件) { } else { }` —— **花括号永不省略**。
- **数学区间 `10 ≤ x ≤ 20` 必须写成 `x >= 10 && x <= 20`**，不能连写 `10 <= x <= 20`（它永远是"真的"）。
- `&&`（且）/ `||`（或）**短路求值**：从左往右，结果一旦确定就停下 → `b != 0 && a / b > 1` 不会除零。
- 经典陷阱：判断相等用 `==`，不是 `=`；写错时 `-Wall` 会警告 `suggest parentheses around assignment`。
- 奇偶判断：`n % 2` 可能是 -1/0/1 → 判奇数用 `n % 2 != 0`（`n % 2 == 1` 对负数失效）。
- 浮点数**禁止直接 `==`**，用 `fabs(a - b) < 1e-9`（判别式 `d == 0` 同理）。
- `switch(整型表达式)` + `case 常量:` + **`break` 防穿透** + `default` 兜底；`case 6: case 7:` 可故意合并。
- 优先级速记：算术 > 关系 > `&&` > `||` > 赋值；记不住就**加括号**，永远不吃亏。

## 二、代码模板

**模板 1：if / else if 阶梯（成绩分级模式，条件从高到低）**

```c
if (score >= 90)
{
    printf("A\n");
}
else if (score >= 80)
{
    printf("B\n");
}
else if (score >= 70)
{
    printf("C\n");
}
else
{
    printf("Below C\n");
}
```

**模板 2：switch（等值多路）**

```c
switch (day)
{
    case 1:
        printf("Monday\n");
        break;
    case 6:
    case 7:            /* intentional fall-through: share one body */
        printf("Weekend!\n");
        break;
    default:
        printf("Invalid day\n");
        break;
}
```

**模板 3：区间判断 + 短路保护**

```c
if (x >= 10 && x <= 20)          /* 10 <= x <= 20 的正确写法 */
{
    ...
}

if (b != 0 && a / b > 1)         /* b == 0 时短路，不会执行 a / b */
{
    ...
}
```

**模板 4：浮点判等（HW3 判别式用）**

```c
#include <math.h>                /* fabs, sqrt 都要它 */

double d = b * b - 4.0 * a * c;

if (fabs(d) < 1e-9)              /* d 约等于 0：一个实根 */
{
    ...
}
```

## 三、坑清单（L2 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | `=` 写成 `==`（赋值当比较） | 条件恒真，逻辑全乱；`-Wall` 会警告 | `if (n == 5)`；用 `-Wall` 让编译器当守门员 |
| 2 | 数学区间连写 `10 <= x <= 20` | 编译不报错但逻辑恒真（先算 `10<=x` 得 0/1，再和 20 比永远 ≤20） | `x >= 10 && x <= 20` |
| 3 | 奇偶判断写 `n % 2 == 1` | 负数奇数（如 -3）`%2 == -1`，被判成偶数 | `n % 2 != 0` |
| 4 | 单语句省略花括号 | 以后加第二行时逻辑悄悄变错（else 悬挂） | 花括号永不省略 |
| 5 | switch 里漏 `break` | 穿透（fall-through），后面 case 全执行 | 每个 case 体结尾 `break;`（故意合并除外） |
| 6 | 浮点直接 `== 0` / 判等 | 因存储误差永远不相等 / 误判 | `fabs(d) < 1e-9` |
| 7 | 以为 `&&` `||` 两边都会执行 | 短路：左边已定结果，右边被跳过 | 需要两边都算时分开写 |
| 8 | `scanf("%c", ...)` 直接跟在数字输入后 | `%c` 不跳过空白，吃到残留的 `\n` 或空格 | 格式串里 `%c` 前留空格：`scanf("%lf %c %lf", ...)` |

## 四、复习自查清单

- [ ] 不查资料默写：if/else if/else 阶梯、switch + break + default
- [ ] 解释"关系运算的结果是 int（1/0）"；`10 <= x <= 20` 为什么恒真
- [ ] 说清 `&&` `||` 的短路求值，并举一个"用短路保护除零/越界"的例子
- [ ] 一眼看出 `if (x = 5)` 的 bug；知道 `-Wall` 会给什么警告
- [ ] 口答：-7、-4 的 `% 2` 各是多少？判断奇数应该怎么写？
- [ ] 解释 switch 的穿透（fall-through），并演示故意合并 case
- [ ] 为什么 `double d` 不能直接 `if (d == 0)`？标准写法是什么？
- [ ] 浮点输入输出占位符（scanf `%lf` / printf `%.2f`）仍然脱口而出
- [ ] 手敲 3 个示例并编译运行；风格：花括号、4 空格、蛇形命名
- [ ] 做题前先手算"已知正确答案"，边界值必须测（== 分界点两侧都要）

## 五、作业清单（含通关测试值）

> 存盘位置：E:\learn408\homework\ ｜ 判分：编译（`-Wall -Wextra` 零警告）/ 正确性 / 边界 / 风格。
> 通关线：HW1–HW3 全部 ✅ 解锁 L3（循环）；HW4 ★★★ 挑战不计门槛。
> 判分只看关键输出行（printf 提示语不计）。

### HW1（必做 ★）闰年判断 — hw1_leap.c

- 输入 int `year`（1 ≤ year ≤ 100000，保证合法）。
- 闰年规则：**能被 4 整除且不能被 100 整除，或能被 400 整除**。
- 输出一行：`year Y is a leap year` 或 `year Y is not a leap year`。
- 通关测试值（输入 → 期望）：

| 输入 | 期望输出 |
| --- | --- |
| 2000 | year 2000 is a leap year |
| 1900 | year 1900 is not a leap year |
| 2024 | year 2024 is a leap year |
| 2023 | year 2023 is not a leap year |
| 400 | year 400 is a leap year |
| 100 | year 100 is not a leap year |
| 1 | year 1 is not a leap year |

- 验算方法：手算 1900：能被 100 整除但不能被 400 → 不是闰年（这是最常见的错点，很多人以为整百都是闰年）。

### HW2（必做 ★★）成绩分级 — hw2_grade.c

- 输入 int `score`（0 ≤ score ≤ 100，保证合法）。
- 等级规则：90–100 → A ｜ 80–89 → B ｜ 70–79 → C ｜ 60–69 → D ｜ 0–59 → E。
- 输出一行：`score -> grade`，如输入 85 输出 `85 -> B`。
- 通关测试值（**每个分界点两侧都要测**）：

| 输入 | 期望输出 | 输入 | 期望输出 |
| --- | --- | --- | --- |
| 100 | 100 -> A | 69 | 69 -> D |
| 90 | 90 -> A | 60 | 60 -> D |
| 89 | 89 -> B | 59 | 59 -> E |
| 80 | 80 -> B | 0 | 0 -> E |
| 79 | 79 -> C | 91 | 91 -> A |
| 70 | 70 -> C | 61 | 61 -> D |

### HW3（必做 ★★）一元二次方程实根讨论 — hw3_quadratic.c

- 输入 double `a b c`（保证 a ≠ 0），`#include <math.h>`。
- 判别式 `d = b*b - 4*a*c`，用 `fabs(d) < 1e-9` 判断"约等于 0"。
- 输出规则：
  - `fabs(d) < 1e-9`：`One real root: x = %.2f`，其中 x = -b / (2a)
  - `d > 0`：`x1 = %.2f, x2 = %.2f`，x1 = (-b + sqrt(d)) / (2a)，x2 = (-b - sqrt(d)) / (2a)
  - `d < 0`：`No real roots`
- 通关测试值（输入 a b c → 期望输出）：

| 输入 | 期望输出 |
| --- | --- |
| 1 -3 2 | x1 = 2.00, x2 = 1.00 |
| 2 -5 2 | x1 = 2.00, x2 = 0.50 |
| 1 -2 1 | One real root: x = 1.00 |
| 2 4 2 | One real root: x = -1.00 |
| 1 0 -4 | x1 = 2.00, x2 = -2.00 |
| 1 0 1 | No real roots |
| 1 5 6 | x1 = -2.00, x2 = -3.00 |
| 1 -2 1.000000000001 | One real root: x = 1.00（考验 fabs 判等） |

- Windows MinGW 下 sqrt 一般不需要 `-lm`；若报 `undefined reference to sqrt` 就加：`gcc ... hw3_quadratic.c -o hw3_quadratic.exe -lm`（Linux 上必加）。

### HW4（挑战 ★★★）简单计算器 — hw4_calculator.c

- 输入格式 `%lf %c %lf`（如 `8 + 2` 或 `8+2` 都行，注意 `%c` 前留空格）。
- 运算 `+ - * /` 输出：`%.2f %c %.2f = %.2f`，如 `8.00 + 2.00 = 10.00`。
- 除法且 b == 0：输出 `Division by zero.`；未知运算符：输出 `Unknown operator.`
- 用 switch 实现四则；除法结果保留两位。
- 通关测试值（输入 → 期望）：

| 输入 | 期望输出 |
| --- | --- |
| 8 + 2 | 8.00 + 2.00 = 10.00 |
| 8 - 2 | 8.00 - 2.00 = 6.00 |
| 8 * 2 | 8.00 * 2.00 = 16.00 |
| 8 / 2 | 8.00 / 2.00 = 4.00 |
| 7 / 2 | 7.00 / 2.00 = 3.50 |
| 2.5 * 4 | 2.50 * 4.00 = 10.00 |
| 0 / 5 | 0.00 / 5.00 = 0.00 |
| 5 / 0 | Division by zero. |
| 1 ? 2 | Unknown operator. |

---

## 参考：运算符优先级（背前 4 行即可，其余加括号）

| 优先级 | 运算符 | 说明 |
| --- | --- | --- |
| 高 | `( )` | 括号永远最高，救命用 |
| | `*` `/` `%` | 算术乘除 |
| | `+` `-` | 算术加减 |
| | `<` `>` `<=` `>=` | 关系（比较） |
| | `==` `!=` | 相等判断（**低于**上面一组！`a < b == c` 先比大小） |
| | `&&` | 逻辑与 |
| | `\|\|` | 逻辑或 |
| 低 | `=` `+=` 等 | 赋值（最低） |

> 记忆口诀：**乘除加减 → 比较大小 → 判相等 → 且 → 或 → 赋值**。不确定就加括号，判分不扣分，写错才扣分。

## 六、如何测试（L2 新增工具，以后每课都用）

### 方法 A：手工单个测试（最基础）

```
gcc -Wall -Wextra hw1_leap.c -o hw1_leap.exe
.\hw1_leap.exe
2000                      ← 手工敲输入，看输出
```

### 方法 B：批量自动测试（推荐；工具已做好）

在 `E:\learn408\homework\` 下：

```
gcc -Wall -Wextra hw1_leap.c -o hw1_leap.exe
..\tools\check.ps1 -Exe .\hw1_leap.exe -Tests ..\tools\tests\hw1_leap.txt
```

若终端提示"禁止运行脚本 / running scripts is disabled"，改用：

```
powershell -ExecutionPolicy Bypass -File ..\tools\check.ps1 -Exe .\hw1_leap.exe -Tests ..\tools\tests\hw1_leap.txt
```

输出示例（用参考答案实测）：

```
PASS  2000
PASS  1900
...
Result: 7 passed, 0 failed, 0 skipped
```

- 测试数据：`tools\tests\hw1_leap.txt`（HW1–HW4 各一份），格式 `输入 => 期望输出`，可自己加行；
- FAIL 会打印 `expected >` 与 `actual >` 两行，直接定位差异；
- 判定方式："期望输出"是程序输出的子串即算通过，所以 printf 提示语（`Enter a year: `）不影响判分。

### 方法 C：重定向（理解原理，考场常用）

cmd 里：`hw1_leap.exe < cases.txt`；PowerShell 不支持 `<`，改用：

```
Get-Content cases.txt | .\hw1_leap.exe
"2000" | .\hw1_leap.exe
```

### 测试心法（判分就看这个）

1. **先手算期望值**（含边界两侧），再跑程序对答案；
2. 每个分界点**两侧都测**（89/90、59/60、1899/1900/2000…）；
3. 先做到零警告编译，再谈测试；
4. 全绿不等于万事大吉——还要自己**读一遍输出**，看有没有多打印 / 漏打印。

> 参考答案在 `lessons/L02_branching/solutions/`，**做完作业再看**，否则等于抄答案。
