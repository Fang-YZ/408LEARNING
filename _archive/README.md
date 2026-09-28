# ⚠️ 归档区（_archive）— 内容已作废，仅留档备查

本目录保存 **2026-09-28 建档重复** 产生的文件。它们**不是**当前教学体系的组成部分，请勿据此做作业。

| 归档文件 | 原位置 | 为什么作废 | 替代品 |
| --- | --- | --- | --- |
| `lesson-01-体系结构与分层(已被N02笔记取代).md` | `course/notes/` | 建档时把"分层"编成第 1 课，与现有体系（N1=性能指标、**N2=分层**）冲突 | [networks/N02_layered_arch/notes.md](../networks/N02_layered_arch/notes.md) |
| `lesson-01-作业(已作废).md` | `homework/` | 同上；且违反"作业按天出"规则（提前下发） | N2 正式作业在 N1 通关后随讲义下发 |
| `lesson-01-答题卡(已作废).txt` | `homework/` | 同上 | — |
| `lesson-01-提交说明(已作废).md` | `homework/` | 同上 | [tools/check-n2.ps1](../tools/check-n2.ps1) 的使用说明（待 N2 开课时给出） |
| `check-lesson-01(已被check-n2取代).ps1` | `grade/` | 编号与路径都指向错误的 lesson-01 目录 | [tools/check-n2.ps1](../tools/check-n2.ps1) |

**同时一并作废（内容仍保留在原位置，但顶部已加废弃声明）**：

- `course/00-课程总目录.md` —— 与根目录 [课程总目录.md](../课程总目录.md) 重复，以后者为准
- `course/错题本.md` —— 与根目录 [错题本.md](../错题本.md) 重复，以后者为准

**保留有效的内容**（已搬家，不是归档）：

- N2 笔记 → `networks/N02_layered_arch/notes.md`
- N2 示例源码 → `networks/N02_layered_arch/examples/`（ex1 服务器 / ex2 客户端 / ex3 封装演示）
- N2 参考解 → `solutions/N02_layered_arch/ex3_upper_server.c`
- N2 机械预检器 → `tools/check-n2.ps1`
