# A 线复习笔记总览（C/C++ → 408 数据结构）

> **全线预生成**：14 课复习笔记已全部建档，随时可预习 / 复习。
> **作业按天出**：每课开课当天才随讲义下发题目与「通关测试值」——笔记里只有「作业预告」，不剧透题目。
> 建档日期：2026-09-10 ｜ 学员档案见 `课程总目录.md`，错题见 `错题本.md`

## 一、怎么用这套笔记

1. **开课前预习**：翻当天的笔记，30 秒速览 + 代码模板，混个脸熟；
2. **开课当天**：听讲解 → 拿当天作业（含通关测试值）→ 写代码；
3. **提交前**：用 `tools\check.ps1` 自测到全绿，再交给我读盘批改（判定表 + 星级 + 错题登记）；
4. **复盘时**：只看「坑清单」和「复习自查清单」，能全部打勾说明这课真会了；
5. **参考答案**在各课 `solutions\` 目录，**做完再看**。

## 二、课次总表

| 课次 | 主题 | 复习笔记 | 状态 |
| --- | --- | --- | --- |
| L1 | C 入门：main / 类型 / printf / scanf / 算术 | `lessons/L01_hello_types_io/notes.md` | 🟢 已通关（HW1–HW4 全 5★） |
| L2 | 分支：if / else / switch / 逻辑运算 | `lessons/L02_branching/notes.md` | 🟢 已通关（HW1–HW3 满分） |
| L3 | 循环：while / for / 嵌套 / break·continue | `lessons/L03_loops/notes.md` | 🔵 教学中（作业待交） |
| L4 | 函数：声明 / 形参实参 / 返回 / 作用域 | `lessons/L04_functions/notes.md` | ⬜ 笔记已建档，待开课 |
| L5 | 递归入门：三要素 / 调用栈 / 分治思想 | `lessons/L05_recursion/notes.md` | ⬜ 笔记已建档，待开课 |
| L6 | 数组：一维 / 二维 / 越界 / 冒泡·二分 | `lessons/L06_arrays/notes.md` | ⬜ 笔记已建档，待开课 |
| L7 | 指针（上）：地址 / `&` / `*` / 传址调用 | `lessons/L07_pointers_basics/notes.md` | ⬜ 笔记已建档，待开课 |
| L8 | 指针（下）：指针与数组 / 指针算术 / const | `lessons/L08_pointers_arrays_strings/notes.md` | ⬜ 笔记已建档，待开课 |
| L9 | 字符串：`'\0'` / 手写 strlen·strcpy·strcmp | `lessons/L09_strings/notes.md` | ⬜ 笔记已建档，待开课 |
| L10 | 结构体：`.` 与 `->` / typedef / enum·union | `lessons/L10_structs/notes.md` | ⬜ 笔记已建档，待开课 |
| L11 | 动态内存：malloc / calloc / realloc / free | `lessons/L11_dynamic_memory/notes.md` | ⬜ 笔记已建档，待开课 |
| L12 | **里程碑**：C 手写单链表（增删查改释放） | `lessons/L12_linked_list/notes.md` | ⬜ 笔记已建档，待开课 |
| L13 | 多文件 / 头文件 / 编译链接四步 | `lessons/L13_multi_file/notes.md` | ⬜ 笔记已建档，待开课 |
| L14 | C++ 速览：cin·cout / 引用 / string / vector | `lessons/L14_cpp_overview/notes.md` | ⬜ 笔记已建档，待开课 |

## 三、三个关键分水岭（学长血泪提示）

| 里程碑 | 为什么关键 |
| --- | --- |
| **L7 指针** | 408 教材里链表、树、图全用指针描述；指针不通，后面全是天书。这里慢一点没关系 |
| **L12 单链表** | 408「线性表链式存储」= 第一座大山，也是历年真题的常客；手写三遍不算多 |
| **L14 C++ 选修** | 用 C 答题完全够；但若目标院校/机试偏 C++，`vector` / `string` 能省掉大量指针痛苦 |

## 四、配套工具

| 工具 | 用途 |
| --- | --- |
| `tools/check.ps1` | 批量测试：`.\tools\check.ps1 -Exe .\hw.exe -Tests .\tools\tests\hw.txt` |
| `tools/tests/*.txt` | 各题通关测试值（`输入 => 期望输出`，可自己加行） |
| 各课 `solutions/*.c` | 参考答案（做完再看） |
| `错题本.md` | 批改自动登记，复习时逐条重做、能独立写对两次再打勾 |
