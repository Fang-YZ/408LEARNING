# L14 复习笔记 — C++ 速览与查漏补缺（面向"用 C++ 答题"的路线）

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- **`cin` / `cout` 不用写类型**：`cin >> n`、`cout << n`，编译器自己认；一长串 `%d` 格式串从此少写。
- `cin >>` **自动跳过空白**（空格、Tab、换行），`cin >> a >> b` 就等于 `scanf("%d %d", &a, &b)`，而且**不用写取地址符 `&`**。
- 换行优先 `"\n"`，少用 `std::endl`：`endl` 每次都**强制刷新缓冲区**，数据量大时明显更慢。
- `bool` 是**真正的类型**（`true` / `false`）；`cout` 默认只打印 `1 / 0`，加 `std::boolalpha` 才打印 `true / false`。
- **引用 `&` 是"变量的别名"**：`int &r = a;` 之后 `r` 与 `a` 是同一个东西；函数参数写成引用 = 能改实参又不用写 `*` / `&`。
- 引用必须**定义时绑定**，且一辈子只绑这一个；指针可以改指向、可以为空——这是两者最大的区别。
- **函数重载**：同名函数靠**参数列表**区分；返回类型不同**不算**重载。**默认参数**只能写一处，且必须靠右排。
- `std::string` **自带长度、能拼接能比较**，`strlen` / `strcpy` / `strcmp` 三件套可以退休，也不会越界踩内存。
- `std::vector<T>` 是**会自动扩容的数组**：`push_back` 追加、`size()` 取长度、`v[i]` 下标访问，比 `malloc + realloc` 省心。
- C++ 里 **`struct` 可以直接带成员函数**（`first.print()`），这就是"类与对象"的最简形态；成员函数末尾写 `const` 表示"不修改本对象"。
- `new / delete` 会调用构造与析构并返回正确类型，`malloc / free` 只借内存；**两者绝对不能混用**，`new[]` 必须配 `delete[]`。
- `std::sort(v.begin(), v.end())` 一行升序排序；自定义规则就传第三个参数（比较函数）。
- 编译配方：`g++ -Wall -Wextra -std=c++11 文件.cpp -o 文件.exe`；本课全部示例实测**零警告**，`%lld` 那类格式坑用 `cout` 直接绕开。

## 二、代码模板

**模板 1：输入输出与 bool（对比 printf / scanf）**

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    int student_id = 0;
    double score = 0.0;

    // cin >> skips whitespace, like scanf with a space in the format string.
    std::cout << "Enter id and score: ";
    std::cin >> student_id >> score;
    std::cout << "id = " << student_id << ", score = " << score << "\n";

    // bool prints as 1 / 0 by default; boolalpha gives true / false.
    bool passed = (score >= 60.0);
    std::cout << "passed = " << passed
              << ", boolalpha = " << std::boolalpha << passed << "\n";

    // Two decimals without printf: fixed + setprecision(2) from <iomanip>.
    std::cout << "score = " << std::fixed << std::setprecision(2) << score << "\n";

    return 0;
}
```

**模板 2：引用参数 / 函数重载 / 默认参数**

```cpp
#include <iostream>

// Pass by reference: the caller's variable IS changed; no * here, no & at the call.
void swap_values(int &lhs, int &rhs)
{
    int temp = lhs;
    lhs = rhs;
    rhs = temp;
}

// Overloading: same name, different parameter lists; the compiler picks by type.
int square(int value)
{
    return value * value;
}

double square(double value)
{
    return value * value;
}

// Default argument: written once, in the definition (or in the prototype), not both.
int power(int base, int exponent = 2)
{
    int result = 1;
    for (int step = 0; step < exponent; step++)
    {
        result *= base;
    }
    return result;
}

int main()
{
    int a = 3;
    int b = 9;

    swap_values(a, b);
    std::cout << "swap: " << a << " " << b << "\n";
    std::cout << "square: " << square(5) << " " << square(2.5) << "\n";
    std::cout << "power: " << power(5) << " " << power(2, 10) << "\n";

    return 0;
}
```

**模板 3：std::string 常用操作**

```cpp
#include <iostream>
#include <string>

int main()
{
    char old_style[32] = "hello";
    std::string name(old_style);        // C string -> std::string

    name += " world";                   // grow: no fixed buffer, no overflow
    std::cout << "name = " << name << ", size = " << name.size() << "\n";
    std::cout << "first = " << name[0] << ", last = " << name.back()
              << ", substr = " << name.substr(0, 5) << "\n";

    std::size_t found = name.find("world");        // npos means "not found"
    std::cout << "find(world) = " << found << ", find(xyz) = "
              << (name.find("xyz") == std::string::npos ? -1 : 0) << "\n";
    std::cout << "c_str() = " << name.c_str() << "\n";    // for printf("%s") users

    return 0;
}
```

**模板 4：std::vector 入门（push_back / size / 遍历 / 下标）**

```cpp
#include <iostream>
#include <vector>

// Const reference: no copy of the whole vector, and the callee cannot modify it.
int sum_of(const std::vector<int> &values)
{
    int total = 0;
    for (std::size_t index = 0; index < values.size(); index++)
    {
        total += values[index];
    }
    return total;
}

int main()
{
    std::vector<int> data;              // starts empty, no capacity needed
    data.push_back(10);
    data.push_back(20);
    data.push_back(30);

    std::cout << "size = " << data.size() << ", data[0] = " << data[0]
              << ", front = " << data.front() << ", back = " << data.back()
              << ", sum = " << sum_of(data) << "\n";

    std::cout << "range-for:";          // C++11 range-based for
    for (int value : data)
    {
        std::cout << " " << value;
    }
    std::cout << "\nindex-for:";        // the style closest to C arrays
    for (std::size_t index = 0; index < data.size(); index++)
    {
        std::cout << " " << data[index] * 2;
    }
    std::cout << "\n";

    std::vector<int> zeros(4, 0);       // four zeros
    std::cout << "zeros = " << zeros.size() << ", ";
    data.clear();
    std::cout << "after clear = " << data.size() << "\n";

    return 0;
}
```

**模板 5：struct 带成员函数（类与对象的最简形态）**

```cpp
#include <iostream>
#include <string>

struct Student
{
    std::string name;
    int score;

    void set_values(const std::string &new_name, int new_score)   // changes the object
    {
        name = new_name;
        score = new_score;
    }

    void add_bonus(int extra)
    {
        score += extra;
    }

    // const = "will not modify the object"; is_passed is called inside print,
    // so it must be const as well.
    bool is_passed() const
    {
        return score >= 60;
    }

    void print() const
    {
        std::cout << name << ":" << score << (is_passed() ? "(pass)" : "(fail)") << "\n";
    }
};

// Passing by value copies the whole struct; const reference does not copy.
void print_copy(Student one)
{
    one.score = 999;                    // only the copy changes
    std::cout << "in print_copy: " << one.name << " " << one.score << "\n";
}

int main()
{
    Student first;
    first.set_values("Alice", 58);
    first.print();
    first.add_bonus(5);
    first.print();

    Student second = {"Bob", 91};       // aggregate init still works
    print_copy(second);
    std::cout << "original score = " << second.score << "\n";

    return 0;
}
```

**模板 6：new / delete 与 std::sort（printf 版，编译时加 `-D__USE_MINGW_ANSI_STDIO=1`）**

```cpp
#include <algorithm>
#include <cstdio>
#include <vector>

struct Edge
{
    int from;
    int to;
    int weight;
};

// Custom comparison: smaller weight first, then smaller "from".
bool by_weight(const Edge &lhs, const Edge &rhs)
{
    if (lhs.weight != rhs.weight)
    {
        return lhs.weight < rhs.weight;     // strict less-than: never use <= here
    }
    return lhs.from < rhs.from;
}

int main()
{
    int *single = new int(42);              // new: one int on the heap
    std::printf("*single = %d\n", *single);
    delete single;                          // release it, exactly once

    int count = 5;
    int *block = new int[count];            // new[] for arrays
    for (int index = 0; index < count; index++)
    {
        block[index] = (index + 1) * 10;
    }
    std::printf("block[0..2] = %d %d %d\n", block[0], block[1], block[2]);
    delete[] block;                         // must match new[], never plain delete

    std::vector<int> values = {42, 7, 19, 7, 3};
    std::sort(values.begin(), values.end());      // default: ascending
    std::printf("sorted = %d %d %d %d %d\n",
                values[0], values[1], values[2], values[3], values[4]);

    // Sort structs with a custom rule -- the 408 favourite (shortest edge first).
    std::vector<Edge> edges = {{0, 1, 9}, {1, 2, 4}, {2, 3, 4}, {0, 3, 7}};
    std::sort(edges.begin(), edges.end(), by_weight);
    std::printf("edges = (%d,%d,%d) (%d,%d,%d)\n",
                edges[0].from, edges[0].to, edges[0].weight,
                edges[1].from, edges[1].to, edges[1].weight);

    return 0;
}
```

**六个模板的真实输出（全部零警告；模板 1 输入为 `1001 85.5`，模板 6 的 g++ 多带一个 `-D__USE_MINGW_ANSI_STDIO=1`）**

```
1: Enter id and score: id = 1001, score = 85.5 / passed = 1, boolalpha = true / score = 85.50
2: swap: 9 3 / square: 25 6.25 / power: 25 1024
3: name = hello world, size = 11 / first = h, last = d, substr = hello
3: find(world) = 6, find(xyz) = -1 / c_str() = hello world
4: size = 3, data[0] = 10, front = 10, back = 30, sum = 60
4: range-for: 10 20 30 / index-for: 20 40 60 / zeros = 4, after clear = 0
5: Alice:58(fail) / Alice:63(pass) / in print_copy: Bob 999 / original score = 91
6: *single = 42 / block[0..2] = 10 20 30 / sorted = 3 7 7 19 42 / edges = (1,2,4) (2,3,4)
```

## 三、坑清单（L14 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | 混用 `cin` 与 `scanf`（或 `cout` 与 `printf`） | 输出顺序错乱、输入被跳过 | 一个程序**只用一套**；非混不可时先 `std::ios::sync_with_stdio(false)` |
| 2 | 到处写 `std::endl` | 每次换行都刷新缓冲区，大数据量明显变慢 | 普通换行用 `"\n"`，确实要立即看到输出才用 `endl` |
| 3 | 以为 `cout << flag` 会打印 `true` | bool 默认打印 `1 / 0`，作业判定不通过 | 先 `std::cout << std::boolalpha;` 或写 `flag ? "true" : "false"` |
| 4 | `for (int i = 0; i < v.size(); i++)` | 有符号与无符号比较，`-Wextra` 报 `comparison of integer expressions of different signedness` | 用 `std::size_t index`，或 `for (int value : v)` |
| 5 | 只写 `int &r;`（声明不绑定） | `error: 'r' declared as reference but not initialized` | 定义即绑定：`int &r = a;`，且不能改绑 |
| 6 | 两个重载函数只改返回类型 | `error: ambiguating new declaration of ...` | 返回类型不参与重载；改参数个数或类型 |
| 7 | 默认参数在声明和定义里各写一遍 | `error: default argument given for parameter 1 of ...` | 默认值**只写一处**（通常写在 .h 的声明里） |
| 8 | 默认参数写在参数表中间 | `error: default argument missing for parameter 2 of ...` | 默认参数一律靠右：`int f(int a, int b = 2)` |
| 9 | 把 `std::string` 直接交给 `printf("%s")` | 输出乱码甚至崩溃（传的是对象，不是 `char *`） | 用 `name.c_str()`，或者干脆用 `cout` |
| 10 | `vector` 用 `v[i]` 越界 | 不报错、读到垃圾值（未定义行为），可能段错误 | 用 `v.at(i)`（会抛异常）或先判 `i < v.size()` |
| 11 | `new[]` 用 `delete` 释放 | 行为未定义（构造/析构不成对），可能崩溃 | `new` ↔ `delete`，`new[]` ↔ `delete[]`，不与 `malloc/free` 混用 |
| 12 | `sort` 的比较函数写成"非严格弱序" | 运行时段错误或结果错乱（用了 `<=`） | 用严格小于：`return lhs.weight < rhs.weight;` |

## 四、复习自查清单

- [ ] 不查资料写出 `cin >> a >> b;` 与 `cout << a << " " << b << "\n";`，并说出与 `scanf/printf` 的差异
- [ ] 说出 `cin >>` 为什么不用取地址符 `&`，并解释 `"\n"` 与 `std::endl` 的区别
- [ ] `bool` 的两种打印形态（`1 / 0` 与 `true / false`）都会写
- [ ] 一句话说清引用与指针的三个区别（是否可空、是否可改绑、要不要解引用）
- [ ] 默写 `swap_values(int &lhs, int &rhs)`，并对比 C 的指针写法
- [ ] 手写两个同名重载函数，并举一个"只改返回类型会编译失败"的例子
- [ ] 用 `std::string` 完成：拼接、取长度、取子串、查子串、转回 `const char *`
- [ ] 用 `std::vector<int>` 完成：追加、取长、下标读、两种遍历、清空
- [ ] 写一个带成员函数的 `struct`，解释成员函数末尾的 `const` 是什么意思
- [ ] 说出 `new/delete` 与 `malloc/free` 的三点不同，并解释为什么不能混用
- [ ] 手写 `std::sort` 的默认用法与自定义比较函数用法各一遍
- [ ] 每次编译都带 `-Wall -Wextra -std=c++11`，并解释 `-std=` 在管什么

## 五、作业预告（开课当天随讲义下发，含通关测试值）

- **HW1（必做 ★★）**：输入输出改造 —— 把 L13 或更早的某个 C 作业改写成 `cin / cout` 版本，行为必须与 C 版完全一致；讲义给出对照判定口径与通关测试值。
- **HW2（必做 ★★）**：用 C++ 重写"数据统计"小工具：`std::vector<int>` 收数据、引用参数做统计函数、`std::sort` 输出有序结果；讲义给出必须跑通的分步验收项。
- **HW3（必做 ★★）**：写一个带成员函数的 `struct`（学生/节点），实现"录入 + 加分 + 判定 + 打印"；要求通过 `const` 正确性检查。
- **HW4（挑战 ★★★）**：`new/delete` 与 `malloc/free` 对照实验 —— 两种方式各建一个动态数组，说明谁泄漏了内存、为什么；含通关测试值。

---

## 参考一：C 写法 ↔ C++ 写法对照表

| 任务 | C 写法 | C++ 写法 |
| --- | --- | --- |
| 头文件 / 输出 | `#include <stdio.h>` + `printf("%d\n", n);` | `#include <iostream>` + `std::cout << n << "\n";` |
| 格式化输出 | `printf("%.2f\n", x);` | `std::cout << std::fixed << std::setprecision(2) << x << "\n";` |
| 输入 | `scanf("%d", &n);` | `std::cin >> n;`（不用 `&`） |
| 布尔 | `int flag = 0;` | `bool flag = false;` |
| 交换两数 | `void swap(int *a, int *b)`，调用写 `&x` | `void swap(int &a, int &b)`，调用直接写 `x` |
| 字符串 | `char s[100];` + `strcpy/strcat/strcmp/strlen` | `std::string s;` + `+=` / `.size()` / `.find()` |
| 字符串转 C 串 | 本来就是 `char *` | `s.c_str()` |
| 动态数组 | `int *p = malloc(n * sizeof(int)); free(p);` | `int *p = new int[n]; delete[] p;` |
| 可变长数组 | 手写 `realloc` 扩容 | `std::vector<int> v; v.push_back(x);` |
| 排序 | 自己写冒泡 / `qsort` + 比较函数 | `std::sort(v.begin(), v.end());` |
| 结构体 | `struct Student s;` + `void print(struct Student *s)` | `Student s;` + 成员函数 `s.print();` |
| 常量 / 空指针 | `#define N 100` / `NULL` | `const int N = 100;` / `nullptr`（C++11，类型安全） |
| 编译 | `gcc -Wall -Wextra a.c -o a.exe` | `g++ -Wall -Wextra -std=c++11 a.cpp -o a.exe` |

## 参考二：`%lld` 这类格式坑怎么办

- C 里 `printf("%lld", x)` 在 MinGW 上要配 `-D__USE_MINGW_ANSI_STDIO=1`（见 L3 坑清单第 8 条）。
- **C++ 路线的最省事做法是改用 `cout`**：`std::cout << x;` 让编译器自己决定类型，格式串相关的警告整类消失。
- 实测补充：**不加任何 `-D` 宏**时，MinGW-W64 8.1.0 的 `g++ -Wall -Wextra -std=c++11` 编译 `%lld` / `%zu` / `%.4f` 的 `printf` 版本同样零警告；模板 6 带了 `-D`，两种都能过，但**能不用格式串就不用**。
