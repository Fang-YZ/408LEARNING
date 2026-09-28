# L13 复习笔记 — 多文件与编译过程：include 本质 / 头文件卫士 / 编译链接 / extern 与 static

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- **`#include` 的本质是文本替换**：预处理器把被包含文件的全部内容**原样粘**到 `#include` 那一行，之后才轮到编译。
- 因此 `<stdio.h>` 与 `"mylib.h"` 只是找文件的路径规则不同（系统目录 vs 当前目录），**粘贴动作完全一样**。
- 分工口诀：**`.h` 放声明（告诉编译器"有这个东西"），`.c` 放定义（真正生成代码）**；声明可以重复无数遍，定义整个工程只能有一份。
- 头文件卫士 `#ifndef MYLIB_H / #define MYLIB_H / #endif` 是**防重复粘贴**的开关：第二次被包含时整段跳过；`#pragma once` 是更省事的等价写法。
- 编译过程四步：**预处理（.c → .i）→ 编译（.i → .s）→ 汇编（.s → .o）→ 链接（.o + 库 → .exe）**。
- 一条命令搞定：`gcc -Wall -Wextra mylib.c main.c -o app.exe`（**列出所有 .c**，头文件不用写）。
- 分步编译：`gcc -Wall -Wextra -c mylib.c -o mylib.o` 只编译不链接；改一个文件只重编一个，这也是 IDE "Build" 的原理。
- `extern int g;` = **声明**（别的 .c 里定义，链接器去找）；`int g = 0;` = **定义**（就在这里，占内存）。
- `static` 加在**全局变量/函数**前 = 只在本 .c 内可见；加在**局部变量**前 = 函数调用之间共享的一份内存（只初始化一次）。
- **编译错误 vs 链接错误**：语法/类型/缺分号 = 编译器报错（**带行号**）；`undefined reference to 'xxx'` = 链接器报错（**函数名找不到实现在哪个 .o 里**）。
- 408 答题**只需要单文件**（一个 `main.c` 从头写到尾），但读真题代码、开源工程、王道配套源码时必须看得懂多文件布局。
- MinGW 下链接 `sqrt` 一般不用 `-lm`（Linux 上要加）；代码里的 Windows 路径要转义：`#include "sub\\mylib.h"`。

## 二、代码模板

**模板 1：一个能直接编译通过的三文件小工程（下面三段分别是 `mylib.h` / `mylib.c` / `main.c` 的内容）。**

```c
/* ================= 1. mylib.h : DECLARATIONS only ================= */
#ifndef MYLIB_H
#define MYLIB_H

#define STACK_CAPACITY 8            /* stack layout: data[0] is the bottom */
int stack_push(int value);          /* prototypes: the compiler trusts these */
int stack_size(void);
void stack_print(void);
int int_min(int a, int b);
int int_max(int a, int b);

/* extern: "this object is DEFINED in some other .c; the linker will find it". */
extern int stack_ops_count;

#endif /* MYLIB_H */

/* ================= 2. mylib.c : DEFINITIONS only ================= */
#include <stdio.h>
#include "mylib.h"

static int stack_data[STACK_CAPACITY];   /* static = file-local: no other .c sees it */
static int stack_top = 0;
int stack_ops_count = 0;                 /* one real definition, reached via extern */

int stack_push(int value)
{
    if (stack_top >= STACK_CAPACITY)
    {
        return 0;                        /* overflow: push rejected */
    }
    stack_data[stack_top] = value;
    stack_top++;
    stack_ops_count++;
    return 1;
}

int stack_size(void)
{
    return stack_top;
}

void stack_print(void)
{
    printf("bottom [");
    for (int index = 0; index < stack_top; index++)
    {
        printf("%d", stack_data[index]);
        if (index < stack_top - 1)
        {
            printf(" ");
        }
    }
    printf("] top\n");
}

int int_min(int a, int b)
{
    return (a < b) ? a : b;
}

int int_max(int a, int b)
{
    return (a > b) ? a : b;
}

/* ================= 3. main.c : uses the library ================= */
int main(void)
{
    printf("push 3: %s\n", stack_push(3) ? "ok" : "overflow");
    printf("push 7: %s\n", stack_push(7) ? "ok" : "overflow");
    printf("push 5: %s\n", stack_push(5) ? "ok" : "overflow");
    stack_print();
    printf("ops so far = %d\n", stack_ops_count);
    printf("min(3, 7) = %d, max(3, 7) = %d\n", int_min(3, 7), int_max(3, 7));
    return 0;
}
```

**模板 2：编译命令（MinGW 实测，全部零警告，输出如下）**

```
gcc -Wall -Wextra mylib.c main.c -o app.exe          # one step: list every .c
gcc -Wall -Wextra -c mylib.c -o mylib.o              # separate steps: compile only
gcc -Wall -Wextra -c main.c -o main.o
gcc mylib.o main.o -o app.exe                        # then link the .o files
.\app.exe
push 3: ok
push 7: ok
push 5: ok
bottom [3 7 5] top
ops so far = 3
min(3, 7) = 3, max(3, 7) = 7
```

**模板 3：头文件卫士的两种写法（二选一，不要写重复）**

```c
/* Style A: standard guard, works on every compiler */
#ifndef MYLIB_H
#define MYLIB_H
int int_min(int a, int b);
#endif /* MYLIB_H */

/* Style B: one line, understood by gcc / clang / MSVC */
#pragma once
int int_min(int a, int b);
```

**模板 4：`extern` / `static` 一个文件里全部演示**

```c
static int hidden_counter = 0;      /* file-local: other .c files cannot link it */
int shared_counter = 0;             /* global: reached from other .c via extern */

static int bump(void)               /* file-local function */
{
    static int calls = 0;           /* static local: one copy, initialized once */
    calls++;
    hidden_counter++;
    return calls;
}
```

## 三、坑清单（L13 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 文件名写错 / 文件不在当前目录 | `fatal error: mylib.h: No such file or directory` | 用 `"mylib.h"` 时文件必须与 .c 同目录；先 `cd` 到项目目录再编译 |
| 2 | `#include "mylib.c"` | 定义被粘进来两份 → `multiple definition of 'int_min'` | `#include` 只包含 `.h`；`.c` 用命令行列出来交给 gcc |
| 3 | 忘了写头文件卫士 | 头文件被链式包含两次 → `redefinition of 'struct Node'` | `#ifndef/#define/#endif` 或 `#pragma once` |
| 4 | 在 .h 里写了函数**定义**（带 `{ }`） | 两个 .c 都包含它 → `multiple definition` | .h 里只写 `int int_min(int a, int b);` 这种声明 |
| 5 | 在 .h 里定义**全局变量** `int count = 0;` | 同上，多个 .o 各有一份 → 链接冲突 | .h 写 `extern int count;`，某个 .c 写 `int count = 0;` |
| 6 | 忘了把某个 .c 加进命令 | 链接器报 `undefined reference to 'int_max'`（是"找不到"，不是语法错） | `gcc -Wall -Wextra mylib.c main.c -o app.exe`，所有 .c 都要写 |
| 7 | 声明与定义签名不一致（拼写/参数） | 同样是 `undefined reference`，编译器查不出 | 从 .h 里复制粘贴签名，逐字一致 |
| 8 | 改了 `mylib.h` 却只重编部分文件 | 代码明明改了，程序行为却没变 | 头文件一改，**包含它的 .c 全部重编**（分步编译的经典事故） |
| 9 | 把链接错误当语法错误查 | 对着 `undefined reference` 找分号，白找半小时 | 带行号 = 编译器（语法）；只有函数名 + `ld returned 1` = 链接器（缺实现/缺文件） |
| 10 | 忘记 `-Wall -Wextra` | 未使用变量、隐式声明等悄悄放过 | 每次编译都带上，零警告是底线 |
| 11 | 分步编译时两个 `-o` 写成同名 | 后一个 `.o` 覆盖前一个，链接时报缺符号 | 每个 `-c` 配不同的 `-o`：`mylib.o` / `main.o` |
| 12 | Linux 上链接数学库失败 | `undefined reference to 'sqrt'`（MinGW 一般不用管） | 命令末尾加 `-lm`：`gcc ... -o app.exe -lm` |

## 四、复习自查清单

- [ ] 用自己的话说出"`#include` 就是文本替换"，并指出替换发生在**编译之前**
- [ ] 默写一个带头文件卫士的 `mylib.h`（函数声明 + `#define` 常量 + `extern` 变量声明）
- [ ] 说出 `.h` 与 `.c` 的分工；解释为什么变量定义写进 .h 会炸
- [ ] 说出 `gcc -c` 与直接 `gcc a.c b.c -o app` 的区别，以及分步编译为什么省时间
- [ ] 解释 `extern int g;` 与 `int g = 0;` 的区别（声明 vs 定义），以及 `static` 三个位置各是什么意思
- [ ] 看到 `undefined reference to 'xxx'`，知道去查三件事：文件加了没、函数写了没、签名对不对
- [ ] 看到 `error: expected ';' before '}' token` 这类带行号的报错，知道是编译器在说话
- [ ] 能画出 预处理 → 编译 → 汇编 → 链接 四步流程，并说出每步的产物后缀
- [ ] 说清"为什么 408 考试写单文件，但读工程代码必须懂多文件"
- [ ] 讲一遍完整建库思路：`mylib.h`（接口）+ `mylib.c`（实现）+ `main.c`（使用），三方谁都不越界

## 五、作业预告（开课当天随讲义下发，含通关测试值）

- **HW1（必做 ★★）**：把之前写过的某个单文件小程序拆成三文件（`.h` + 两个 `.c`），要求带头文件卫士、声明与定义严格分离，并能用一条 `gcc` 命令编译通过。
- **HW2（必做 ★★）**：分步编译与增量构建 —— 用 `-c` 先生成 `.o` 再链接，体验"只改一个文件就只重编一个"；讲义会给出必须观察到的中间产物与链接器报错现场。
- **HW3（必做 ★★）**：故意制造三种错误（缺分号、缺 `.c` 文件、头文件重复包含），分别抄下编译器与链接器给出的原文报错，并写出各自修法。
- **HW4（挑战 ★★★）**：`extern` 与 `static` 综合练习 —— 用 `static` 隐藏模块内部数据，用 `extern` 暴露一个只读计数器；验收口径与通关测试值随讲义下发。

---

## 参考一：编译链接流程与报错来源

```
   mylib.h --+ text replace          main.c --+ text replace
             v                                v
          mylib.c                          main.c
             |  preprocess (gcc -E)  ->  mylib.i  : missing header = fatal error here
             |  compile    (gcc -S)  ->  mylib.s  : syntax / type errors, with line numbers
             |  assemble   (gcc -c)  ->  mylib.o  : object file, almost never fails
             v  link (gcc *.o -o app.exe)  ->  [ linker ld ]  : undefined reference / multiple definition
          app.exe
```

- 记忆锚点：**报错带 `.c` 行号 → 编译器（语法）；报错只有函数名和 `.text+0x...` → 链接器（缺文件 / 缺定义）。**

## 参考二：extern / static 速查

| 写法 | 位置 | 含义 | 别的 .c 能用吗 |
| --- | --- | --- | --- |
| `int g;` / `int g = 0;` | .c | 定义（实际占内存） | 能（配 `extern` 声明） |
| `extern int g;` | .h / .c | 声明（不占内存） | 能 |
| `static int g;` / `static int f(void)` | .c | 文件私有的定义 / 函数 | **不能**（链接器看不见） |
| `static int x;`（函数内） | 函数体 | 只初始化一次，函数调用间共享 | 不适用 |
| `int g = 0;` | **.h** | ❌ 每个包含者一份 → 冲突 | 会炸 |

## 参考三：实测报错原文（MinGW-W64 gcc 8.1.0，忘记带上 mylib.c 时）

```
.../ccMxc5eT.o:link_demo.c:(.text+0x24): undefined reference to `int_min'
.../ccMxc5eT.o:link_demo.c:(.text+0x4d): undefined reference to `int_max'
collect2.exe: error: ld returned 1 exit status
```

- 报错**不带 `.c` 行号** → 一定是链接阶段，别去找分号；`collect2.exe ... ld returned 1` 是 MinGW 的固定收尾句，与语法无关；修法只有三条：把文件加进命令 / 把函数写出来 / 把签名改一致。
