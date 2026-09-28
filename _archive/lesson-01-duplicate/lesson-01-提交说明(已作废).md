# B 线 N2 · 作业提交与自动预检说明（预备稿）

> ⚠️ **尚未生效**：N2 的作业要等 **N1 通关**后才下发。当前请先做 N1（`networks/N01_intro_metrics/notes.md` 的 H1–H4）。
> 下文的预检脚本与流程在 N2 开课时直接启用；教练已用满分卷自测过（24/24 PASS）。

## 一、你要做的全部事情（4 步）

在 PowerShell 里依次执行：

```powershell
# 第 1 步：进入工作目录
cd E:\learn408

# 第 2 步：让教练的预检脚本检查你的作业（它会读答题卡 + 自动连你的服务器）
powershell -ExecutionPolicy Bypass -File .\grade\check-lesson-01.ps1

# 第 3 步：把屏幕上打印的全部内容复制下来
# 第 4 步：把内容贴给我，并说一句"第 1 课作业提交"
```

如果提示"禁止运行脚本"，就用上面这种 `powershell -ExecutionPolicy Bypass -File` 的写法；
本机策略不允许时改执行：

```powershell
Get-Content .\grade\check-lesson-01.ps1 -Raw | Invoke-Expression
```

---

## 二、预检脚本在检查什么

| 检查项 | 对应题目 | 说明 |
|---|---|---|
| 答题卡是否填过 | 0–4 | 只报"空白字段数量"，用来提醒你漏填 |
| 你的答案 vs 标准答案 | 题 2 | 校验 `tcp/TCP/a/network 408/空行` 的 n = 4/4/2/12/1 |
| 服务器能否启动 | 题 0 | 启动你编译出的 `server_echo.exe` |
| **真机端到端回归** | 题 1/2 | 脚本自己写 socket 连你的服务器，发已知数据、比对回显 |
| 大写版服务器 | 题 4 | 启动 `server_echo_upper.exe`，校验 `AbC 123!` → `ACK 9: ABC 123!` |

预检只做**机械可判定**的部分；概念题（题 3）由我人工批改。

---

## 三、预检前必须先准备好的文件

| 文件 | 位置 | 谁生成 |
|---|---|---|
| `server_echo.exe` | `code\lesson-01\` | 你（题 0 编译） |
| `client_echo.exe` | `code\lesson-01\` | 你（题 1 补完 TODO 后编译） |
| `server_echo_upper.c` | `homework\` | 你（题 4 写的代码） |
| `server_echo_upper.exe` | `homework\` | 你（题 4 编译） |
| `lesson-01-答题卡.txt` | `homework\` | 你（填答案） |

> 预检会占用 **9000 / 9001 / 9101** 三个端口，跑之前请先关掉你手动开的服务器窗口。

---

## 四、你的预期值（先自查，再看脚本结论）

| 输入 | n 的标准答案 | 完整回显 |
|---|---|---|
| `tcp` | 4 | `ACK 4: tcp` |
| `TCP` | 4 | `ACK 4: TCP` |
| `a` | 2 | `ACK 2: a` |
| `network 408` | 12 | `ACK 12: network 408` |
| 空行 | **1** | `ACK 1: ` |
| `AbC 123!`（大写版） | 9 | `ACK 9: ABC 123!` |

**为什么空行是 1 而不是 0**：客户端发送的是 `"\n"`，这一个 `\n` 也是**要传输的字节**。
本课协议规定 `n = 文本字节数 + 1`（结尾的 `\n` 计入），所以：
- 文本 `tcp`（3 字节）+ `\n` = **4**
- 空文本行（0 字节）+ `\n` = **1**

这个设计让服务器的 `ACK n` 与客户端的 `SENT n bytes` **完全一致**，两边可以互相验证。
记忆点：**空行也是一次有效传输，长度是 1**。

---

## 五、参考答案在哪

`_solutions/lesson-01/server_echo_upper.c` 是我写的标准答案。
**先自己写完再看**——直接抄会让第 4 课开始你寸步难行。批改通过后我会把它移到
`code/lesson-01/`，并在笔记里标注"官方参考实现"。
