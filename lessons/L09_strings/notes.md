# L9 复习笔记 — 字符串处理：`'\0'`、`char[]` vs `char *`、str 系列函数与 fgets

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- C 没有字符串类型：**字符串 = 以 `'\0'`（数值 0）结尾的 char 数组**；`'\0'` 是标记"串到此结束"的唯一手段。
- `strlen(s)` 数的是 `'\0'` **前面**的字符个数（不含 `'\0'`）：`"hello"` 占 **6 字节**，长度是 **5**。
- `char s1[] = "hello";` 是**在栈上开 6 个字节并把内容抄进去**（可读可写，`sizeof(s1) == 6`）；`char *s2 = "hello";` 只是**指向只读的字面量**（`sizeof(s2) == 8`，指针大小）。
- 两者最要命的差别：`s1[0] = 'H'` 合法；`s2[0] = 'H'` 编译通过但**运行崩溃**（本机实测退出码 `0xC0000005` 访问冲突）。
- 四个必会函数：`strlen`（长度）｜`strcpy`（复制，连 `'\0'` 一起）｜`strcat`（拼接，从 dest 的 `'\0'` 处接着写）｜`strcmp`（比较）。
- `strcmp` **只保证返回值的符号**：`<0` 左小、`0` 相等、`>0` 左大。本机库函数返回 -1/0/1，手写版返回字符差值（实测同一组 `"ab"` vs `"abc"`：库给 -1、手写给 -99）——**只写 `strcmp(a,b) == 0` 判相等**。
- 读输入有讲究：`scanf("%s", s)` **遇空格/换行就停**、**不检查长度**（会溢出）、**把剩下的输入留在缓冲区**污染下一次读；`fgets(s, size, stdin)` 才是读整行的正确工具：**保留 `'\n'`**（常要手动去掉）、**最多读 size-1 个字符**（安全）。
- 408 用途：408"串"一章的 `StrCopy`/`Concat`/`StrCompare`/`StrLength` 与 `strcpy`/`strcat`/`strcmp`/`strlen` 一一对应；模式匹配（BF、KMP 的 `next` 数组）全靠"下标 + 边界"这套功夫。

## 二、代码模板

> 下面的输出都是 `gcc -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1` 编译（**零警告**）后真实运行的结果；栈上的地址每次运行都不同。

**模板 1：`char s1[]` 与 `char *s2` 住在哪、能不能改**
```c
/* L09 t1: char s[] owns a writable copy; char *s only points at a read-only literal. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char s1[] = "hello";             /* 6 bytes on the stack, including '\0' */
    char *s2 = "hello";              /* a pointer into read-only memory */

    printf("s1 = %p, s2 = %p (different places)\n", (void *)s1, (void *)s2);
    printf("sizeof(s1) = %zu (whole array), sizeof(s2) = %zu (a pointer)\n",
           sizeof(s1), sizeof(s2));

    s1[0] = 'H';                     /* OK: s1 is our own writable copy */
    printf("strlen(s1) = %zu, strlen(s2) = %zu, s1 = %s, s2 = %s\n",
           strlen(s1), strlen(s2), s1, s2);
    /* s2[0] = 'H';  <-- undefined behavior: string literals are read-only */

    return 0;
}
```
```
s1 = 000000000061fe02, s2 = 0000000000409000 (different places)
sizeof(s1) = 6 (whole array), sizeof(s2) = 8 (a pointer)
strlen(s1) = 5, strlen(s2) = 5, s1 = Hello, s2 = hello
```
> `s1` 在栈上（地址每次运行都变），`s2` 指向程序映像里的只读数据区（本机实测每次都是 `0x409000`）；三个数字必须背下来：`sizeof` 数组 = **6**（含 `'\0'`）｜`sizeof` 指针 = **8**｜`strlen` = **5**。

**模板 2：四个库函数的标准用法**
```c
/* L09 t2: the four library functions every 408 string problem starts with. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char a[32] = "hello";
    char b[32] = "world";
    char c[32];                      /* big enough for a + b + '\0' */

    printf("strlen(a) = %zu\n", strlen(a));
    strcpy(c, a);                    /* copy a into c, '\0' included */
    strcat(c, b);                    /* append b to the end of c */
    printf("c = %s, strlen(c) = %zu\n", c, strlen(c));
    printf("strcmp(a, a) = %d, strcmp(a, c) = %d\n", strcmp(a, a), strcmp(a, c));

    return 0;
}
```
```
strlen(a) = 5
c = helloworld, strlen(c) = 10
strcmp(a, a) = 0, strcmp(a, c) = -1
```
> 三条铁律：**目标数组必须先够大**（`c` 要能放下 5 + 5 + 1 = 11 字节）；**`strcpy` 会连 `'\0'` 一起抄**；**`strcmp` 的结果只当符号看**。

**模板 3：手写 `my_strlen` / `my_strcpy` / `my_strcat` / `my_strcmp`，并与库函数对照**
```c
/* L09 t3: the four string functions written by hand, checked against the library. */
#include <stdio.h>
#include <string.h>

size_t my_strlen(const char *s)
{
    size_t len = 0;

    while (s[len] != '\0')
    {
        len++;
    }
    return len;
}

char *my_strcpy(char *dest, const char *src)
{
    int i = 0;

    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';                  /* the terminator must be copied too */
    return dest;
}

char *my_strcat(char *dest, const char *src)
{
    int i = 0;

    while (dest[i] != '\0')          /* step 1: find the end of dest */
    {
        i++;
    }
    for (int j = 0; src[j] != '\0'; j++)   /* step 2: append src after it */
    {
        dest[i] = src[j];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

int my_strcmp(const char *a, const char *b)
{
    int i = 0;

    while (a[i] != '\0' && a[i] == b[i])
    {
        i++;
    }
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}

int main(void)
{
    char copy[32] = "";
    char cat[32] = "ab";

    my_strcpy(copy, "abc");
    my_strcat(cat, "cd");
    printf("my_strlen(\"abc\") = %zu, my_strcpy -> [%s], my_strcat -> [%s]\n",
           my_strlen("abc"), copy, cat);
    printf("my_strcmp(\"ab\", \"abc\") = %d, strcmp = %d (same sign, different value)\n",
           my_strcmp("ab", "abc"), strcmp("ab", "abc"));

    return 0;
}
```
```
my_strlen("abc") = 3, my_strcpy -> [abc], my_strcat -> [abcd]
my_strcmp("ab", "abc") = -99, strcmp = -1 (same sign, different value)
```
> 手写一遍胜过读十遍：**`my_strlen` 靠 `'\0'` 停下来；`my_strcpy` 必须补 `dest[i] = '\0'`；`my_strcat` 必须先走完 dest 再写**。最后一行是全部的教训：同一种"左小"，库给 **-1**、手写给 **-99** —— 判等只能写 `strcmp(a, b) == 0`，判大小只能写 `strcmp(a, b) < 0`，写成 `== 1` 就是定时炸弹；手写版用 `unsigned char` 强转，是为了让高位字符（中文、扩展 ASCII）的比较结果稳定。

**模板 4：`fgets` 与 `scanf("%s")` 的差别**
```c
/* L09 t4: fgets reads a whole line; scanf("%s") stops at the first space. */
#include <stdio.h>

int main(void)
{
    char line[16];
    char word[16];

    printf("line 1 goes to fgets:\n");
    if (fgets(line, (int)sizeof(line), stdin) != NULL)
    {
        printf("fgets read: [%s]\n", line);       /* the '\n' is kept inside */
    }

    printf("line 2 goes to scanf:\n");
    if (scanf("%15s", word) == 1)                 /* 15 = buffer size - 1 */
    {
        printf("scanf read: [%s]\n", word);       /* stops at the space */
    }

    return 0;
}
```
输入两行 `hello world`：
```
line 1 goes to fgets:
fgets read: [hello world
]
line 2 goes to scanf:
scanf read: [hello]
```
> 三处细节全是考点：① `fgets` 把 `'\n'` 也读进来了（方括号里换行了），比较前常要去掉；② `scanf("%s")` 只拿到 `hello`，**空格后面的内容留在缓冲区**，下一次读会先读到它（与 L2 里 `%c` 吃到残留 `\n` 是同一个病根）；③ `%15s` 里的 15 是**宽度上限**，防止输入太长撑爆数组 —— 读一整行且安全就用 `fgets`，只想读一个不含空格的词再用 `scanf("%s")`。

## 三、坑清单（L9 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 忘了 `'\0'` 的空间：`char s[3] = "abc";` | **编译零警告**，但 `strlen(s)` 实测返回 6（不是 3）、`%s` 打印出数组后面的垃圾字符 | 长度至少留 `字符数 + 1`：`char s[4] = "abc";` |
| 2 | `char *s = "abc"; s[0] = 'A';` | 编译通过，运行时崩溃（本机实测退出码 `0xC0000005`），连之前 printf 的内容都可能丢 | 要改就开数组：`char s[] = "abc";` |
| 3 | `strcpy` 目标数组太小 | 覆盖栈上别的变量，运行时崩溃或数据诡异变化 | 先算清 `strlen(src) + 1`，目标必须够大 |
| 4 | `strcmp(a, b) == 1` 判相等 | 库返回 -1/0/1 时"恰好"没事，换成返回字符差的实现（手写版给 -99）就永远不成立 | `strcmp(a, b) == 0`、`strcmp(a, b) < 0` |
| 5 | 手写 `my_strcat` 从 dest 开头写、或忘了补 `'\0'` | 把 dest 原有内容覆盖掉，或目标串后面跟着旧内容的尾巴 | 先 `while (dest[i] != '\0') i++;` 找到末尾再写，最后补 `dest[i] = '\0'` |
| 6 | `scanf("%s", s)` 不限宽度 / 想读整行 | 输入超长直接溢出（UB）；遇空格就停，只拿到第一个词，剩下的留在缓冲区污染下一次读 | `scanf("%15s", s)`（数组不小于 16）；读整行用 `fgets(s, size, stdin)` |
| 7 | `fgets` 读进来的 `'\n'` 没处理 | `"abc\n"` 与 `"abc"` 比较永远不相等，输出多一个空行 | 手动去掉：`s[strcspn(s, "\n")] = '\0';` |
| 8 | 用 `sizeof(s)` 当字符串长度 | 数组版多算 1（含 `'\0'`）；指针版得到 8 | 长度用 `strlen(s)`，`sizeof` 只用来算数组容量 |

## 四、复习自查清单

- [ ] 说出"字符串 = 以 `'\0'` 结尾的 char 数组"，并能解释为什么 `"hello"` 要 6 个字节
- [ ] 为什么 `s2[0] = 'H'` 会崩溃？正确做法是什么？
- [ ] 不查资料默写 `my_strlen` / `my_strcpy` / `my_strcat` / `my_strcmp` 四个函数
- [ ] `strcmp` 的返回值能直接和 1 比较吗？为什么？该写什么？
- [ ] `strcpy` 为什么必须连 `'\0'` 一起复制？手写时漏了会怎样？
- [ ] `fgets` 与 `scanf("%s")` 的三点差别（空格、`'\n'`、长度安全）各是什么？
- [ ] 为什么 `scanf("%s", s)` 不用写 `&`，而 `scanf("%d", &x)` 必须写？
- [ ] 输入 `hello world` 后连续调用 `scanf("%s")` 两次，分别读到什么？为什么？
- [ ] 手敲 4 个模板，`-Wall -Wextra` 零警告，输出与笔记一致；并说出 408"串"的基本操作（`StrCopy`/`Concat`/`StrCompare`/`StrLength`）与 C 库函数的对应关系

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本节只给方向，具体规格与通关测试值开课当天随讲义下发。存盘位置：`E:\learn408\homework\L09\`；判分看 `gcc -Wall -Wextra` 零警告 / 正确性 / 边界 / 风格（花括号不省、4 空格缩进、蛇形命名）。

- **HW1 ★ 字符串体检报告**：读入一个字符串，输出长度、逆序串、大写/小写/数字字符个数（用 `'\0'` 作循环终点）。
- **HW2 ★ 手写四大件**：不调用库函数，自己实现 `my_strlen` / `my_strcpy` / `my_strcat` / `my_strcmp`，并与库函数结果对照验证。
- **HW3 ★ 一行读入 + 安全处理**：用 `fgets` 读整行（可能含空格），去掉末尾 `'\n'`，再完成判回文或统计单词数一类的任务。
- **HW4 ★★★ 挑战：模式匹配预告**：在主串里找子串第一次出现的位置 —— 先写朴素的逐字符比较（BF 思想），体会"失配后主串指针要不要回退"，为 KMP 与 `next` 数组埋线。

## 参考：串的术语与 408 衔接

- 术语：**空串**（长度 0，`char s[] = "";` 仍占 1 字节）｜**空格串**（`"   "` 长度是 3，**不是空串**）｜**子串/主串**（一段连续下标的区间 `s[i .. i+len-1]`）｜**模式匹配**（BF 双重循环，KMP 用 `next` 数组免去主串指针回退）；408 的串有三种存储：**定长顺序存储**（`char ch[MAXLEN]`，就是本课的数组）｜**堆分配存储**（`malloc`，第 11 课）｜**块链存储**（每结点存若干字符，第 12 课链表）。C 用 `'\0'` 标记结尾，教材另用 `length` 字段记长度，两种记法都要会。
