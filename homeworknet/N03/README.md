# homeworknet/N03 — B 线 N3（物理层·通信基础）作业存档目录

> 状态：**尚未启用**。N3 要先通过 N1（性能指标）与 N2（分层体系）才解锁。
> 题目「开课当天」随讲义下发（教学规则第 8 条），本目录先占位。

## 提交时请把文件放在这里

| 文件名 | 对应题目 | 来源 |
| --- | --- | --- |
| `channel_capacity.c` | 程序题：信道容量计算器（奈奎斯特 + 香农） | `networks/N03_physical_layer/examples/channel_capacity.c` |
| `switch_compare.c` | 程序题：三种交换方式时延对比 | `networks/N03_physical_layer/examples/switch_compare.c` |
| `channel_capacity_ext.c` | 挑战题 ★★★：反推所需信噪比(dB) 或最少电平数 V | 自己写 |

## 提交后教练怎么批改

```powershell
# 在 E:\learn408 下执行（编译 + 逐组已知答案对拍）
powershell -ExecutionPolicy Bypass -File .\tools\check-n3.ps1
```

教练侧自测记录：**32 PASS / 0 FAIL**（channel_capacity 18 组 + switch_compare 14 组）。
计算题、简答题由教练人工核对要点；错题统一登记到根目录 [错题本.md](../../错题本.md)（来源标注 `N3`）。

## 本课要特别小心的地方

- 奈奎斯特公式里的 `V` 是**码元状态数**，香农公式里**没有 V**——两个定理别混用参数。
- 给的是 `dB` 时必须先换算成线性 `S/N` 才能代入香农公式。
- 自测顺序：**先手算 → 再跑程序 → 对不上就定位是哪一步错**（不要反过来"跑出什么就信什么"）。
