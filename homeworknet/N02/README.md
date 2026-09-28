# homeworknet/N02 — B 线 N2（分层体系结构）作业存档目录

> 状态：**尚未启用**。N2 要先通过 N1（`networks/N01_intro_metrics`）才解锁。
> 作业题目「开课当天」由教练随讲义下发（教学规则第 8 条），本目录先占位。

## 提交时请把文件放在这里

| 文件名 | 对应题目 | 来源 |
| --- | --- | --- |
| `echo_server.c` | 程序题：让课堂示例的 TCP 回显服务器在你机器上跑通 | `networks/N02_layered_arch/examples/ex1_echo_server.c` |
| `encap_demo.c` | 程序题：封装演示，能复现已知的字节数 | `networks/N02_layered_arch/examples/ex3_encapsulation_demo.c` |
| `ex3_upper_server.c` | 挑战题 ★★★：大小写转换后回显（**字节数必须保持原样**） | 自己写；参考解在 `solutions/N02_layered_arch/` |

## 提交后教练怎么批改

```powershell
# 在 E:\learn408 下执行
powershell -ExecutionPolicy Bypass -File .\tools\check-n2.ps1
```

预检器自测记录（教练侧，非学员成绩）：

- 满分卷场景：**23 PASS / 0 FAIL**（课堂示例 + 5 组已知载荷真机回归 + 客户端输出逐行比对）
- 未提交场景：**22 PASS / 2 FAIL**，并明确报出"缺哪个文件"（不会误判成通过）

概念题、简答题由教练人工批改；错题统一登记到根目录 [错题本.md](../../错题本.md)（来源标注 `N2`）。
