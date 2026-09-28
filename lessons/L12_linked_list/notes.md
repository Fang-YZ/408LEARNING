# L12 复习笔记 — 里程碑：用 C 手写单链表

> 课程：408 衔接 C/C++ 私教课 ｜ 日期：2026-09-10 ｜ 说明：本笔记为全线预生成，作业在开课当天随讲义下发

## 一、30 秒速览

- 单链表 = 一串**散落在堆上**的结点，靠每个结点里的 `next` 指针串起来。结点在内存里**不连续**，这就是它和数组最本质的区别。
- 结点定义就三行：`struct Node { int data; struct Node *next; };` —— **`next` 里必须写 `struct Node *`**，不能写真名 `Node *`（此时 `Node` 这个名字还不存在，除非先 typedef）。
- 每个结点都来自 `malloc(sizeof(struct Node))`。**绝不能用局部变量当结点**：函数一返回，栈上那块就作废，整条链立刻全是野指针。
- 单链表只能**单向走**：`for (cur = head; cur != NULL; cur = cur->next)`，走到 `NULL` 就是走完了。链表结点不连续，**不能写 `cur++`**。
- 链表由**头指针**代表：`struct Node *head = NULL;` 就是空表。头指针一丢，整条链既找不回也 free 不掉（纯泄漏）。
- 会动到第一个结点的操作（头插、删首结点）**必须把新头还给调用者**：`head = list_insert_head(head, x);`，或者改成传 `struct Node **`。
- **头插法**：`new_node->next = head; head = new_node;` —— 两句顺序**不能反**（实测反了就是 `new_node->next == new_node` 自环 + 老链头丢失），得到的序列与输入**正好逆序**。
- **尾插法**：先走到最后一个结点（`while (tail->next != NULL) tail = tail->next;`），再 `tail->next = new_node;` —— 保持输入顺序，代价是每插一个都要 O(n)。
- **插入先接后断**：`new_node->next = prev->next; prev->next = new_node;` —— 顺序反了就丢掉后半条链，或者造出自环。
- **删除先摘链后 free**：`prev->next = cur->next; free(cur);` —— 顺序反了就是"访问已释放内存"。
- **整表释放要先存 next 再 free 当前**：`next = cur->next; free(cur); cur = next;` —— 释放完还要把调用者的 `head` 置成 NULL，所以形参用 `struct Node **`。
- 王道教材写的是 `typedef struct LNode { ... } LNode, *LinkList;`，`LinkList L` 就是本课的 `struct Node *head`；王道代码**默认带头结点**，对照表见文末。

## 二、代码模板

**模板 1：结点定义与"造结点"（唯一的 malloc 出口）**

```c
#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

/* every node in this program is born here: one place to check malloc */
struct Node *node_create(int value)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL)
    {
        printf("malloc failed: out of memory\n");
        exit(1);
    }
    new_node->data = value;
    new_node->next = NULL;      /* a fresh node points to nothing */
    return new_node;
}
```

**模板 2：头插法 vs 尾插法建表**

```c
/* head insert: O(1), but the list comes out REVERSED */
struct Node *list_insert_head(struct Node *head, int value)
{
    struct Node *new_node = node_create(value);
    new_node->next = head;      /* grab the old first node */
    return new_node;            /* and become the new first node */
}

/* tail insert: keeps the input order, but costs O(n) per insert */
struct Node *list_insert_tail(struct Node *head, int value)
{
    struct Node *new_node = node_create(value);

    if (head == NULL)
    {
        return new_node;        /* empty list: the new node is the head */
    }

    struct Node *tail = head;
    while (tail->next != NULL)
    {
        tail = tail->next;      /* walk to the last node */
    }
    tail->next = new_node;
    return head;                /* the head did not change */
}
```

**模板 3：遍历打印 / 求长度 / 按值查找**

```c
int list_length(const struct Node *head)
{
    int count = 0;
    for (const struct Node *cur = head; cur != NULL; cur = cur->next)
    {
        count++;
    }
    return count;
}

void list_print(const struct Node *head)
{
    printf("List (%d nodes): ", list_length(head));
    for (const struct Node *cur = head; cur != NULL; cur = cur->next)
    {
        printf("%d -> ", cur->data);
    }
    printf("NULL\n");
}

/* return a POINTER to the node, not an index: that is what a list gives you */
struct Node *list_find(struct Node *head, int value)
{
    for (struct Node *cur = head; cur != NULL; cur = cur->next)
    {
        if (cur->data == value)
        {
            return cur;
        }
    }
    return NULL;                /* not found */
}
```

**模板 4：插入到指定位置 / 删除指定值（首结点要特判）**

```c
struct Node *list_insert_at(struct Node *head, int position, int value)
{
    if (position < 1)
    {
        return head;                            /* invalid: do nothing */
    }
    if (position == 1)
    {
        return list_insert_head(head, value);   /* the head changes */
    }

    struct Node *prev = head;                   /* walk to node number position-1 */
    for (int i = 1; i < position - 1 && prev != NULL; i++)
    {
        prev = prev->next;
    }
    if (prev == NULL)
    {
        return head;                            /* past the end: do nothing */
    }

    struct Node *new_node = node_create(value);
    new_node->next = prev->next;    /* link forward FIRST ... */
    prev->next = new_node;          /* ... then backward */
    return head;
}

struct Node *list_delete_value(struct Node *head, int value)
{
    struct Node *cur = head;
    struct Node *prev = NULL;       /* prev == NULL means "cur is the first node" */

    while (cur != NULL && cur->data != value)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL)
    {
        return head;                /* value not found */
    }

    if (prev == NULL)
    {
        head = cur->next;           /* deleting the FIRST node moves the head */
    }
    else
    {
        prev->next = cur->next;     /* unlink first ... */
    }
    free(cur);                      /* ... then free */
    cur = NULL;
    return head;
}
```

**模板 5：整表释放（唯一的指针的指针）**

```c
void list_free(struct Node **head_ref)     /* needs ** to NULL the caller's head */
{
    struct Node *cur = *head_ref;
    while (cur != NULL)
    {
        struct Node *next = cur->next;     /* remember the next node BEFORE free */
        free(cur);
        cur = next;
    }
    *head_ref = NULL;                      /* the caller's head really becomes NULL */
}
```

**模板 6：完整可运行程序（★ 本课里程碑，含建表 / 遍历 / 查找 / 插入 / 删除 / 释放）**

编译：`gcc -Wall -Wextra l12_linked_list.c -o l12_linked_list.exe`（实测**零警告**）

```c
/*
 * L12 milestone program: a singly linked list written by hand.
 *
 * Convention of this file: every function that may change the first node
 * RETURNS the (possibly new) head pointer, so the caller writes
 *     head = list_insert_head(head, value);
 * Only list_free() takes a pointer-to-pointer, because it must also set
 * the caller's head to NULL.
 */
#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

/* ---------- create one node on the heap ---------- */

struct Node *node_create(int value)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL)
    {
        printf("malloc failed: out of memory\n");
        exit(1);
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

/* ---------- build: head insert / tail insert ---------- */

struct Node *list_insert_head(struct Node *head, int value)
{
    struct Node *new_node = node_create(value);
    new_node->next = head;      /* the new node grabs the old first node */
    return new_node;            /* and becomes the new first node */
}

struct Node *list_insert_tail(struct Node *head, int value)
{
    struct Node *new_node = node_create(value);

    if (head == NULL)
    {
        return new_node;        /* empty list: the new node is the head */
    }

    struct Node *tail = head;
    while (tail->next != NULL)
    {
        tail = tail->next;      /* walk to the last node */
    }
    tail->next = new_node;
    return head;
}

/* ---------- traverse ---------- */

int list_length(const struct Node *head)
{
    int count = 0;
    for (const struct Node *cur = head; cur != NULL; cur = cur->next)
    {
        count++;
    }
    return count;
}

void list_print(const struct Node *head)
{
    printf("List (%d nodes): ", list_length(head));
    for (const struct Node *cur = head; cur != NULL; cur = cur->next)
    {
        printf("%d -> ", cur->data);
    }
    printf("NULL\n");
}

/* ---------- search: return a pointer, not an index ---------- */

struct Node *list_find(struct Node *head, int value)
{
    for (struct Node *cur = head; cur != NULL; cur = cur->next)
    {
        if (cur->data == value)
        {
            return cur;
        }
    }
    return NULL;
}

/* ---------- insert at a 1-based position ---------- */

struct Node *list_insert_at(struct Node *head, int position, int value)
{
    if (position < 1)
    {
        printf("insert_at(%d): invalid position, list unchanged\n", position);
        return head;
    }
    if (position == 1)
    {
        return list_insert_head(head, value);
    }

    struct Node *prev = head;                       /* walk to node position-1 */
    for (int i = 1; i < position - 1 && prev != NULL; i++)
    {
        prev = prev->next;
    }
    if (prev == NULL)
    {
        printf("insert_at(%d): past the end, list unchanged\n", position);
        return head;
    }

    struct Node *new_node = node_create(value);
    new_node->next = prev->next;   /* link forward FIRST, then backward */
    prev->next = new_node;
    return head;
}

/* ---------- delete the first node that holds value ---------- */

struct Node *list_delete_value(struct Node *head, int value)
{
    struct Node *cur = head;
    struct Node *prev = NULL;

    while (cur != NULL && cur->data != value)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL)
    {
        printf("delete(%d): value not found, list unchanged\n", value);
        return head;
    }

    if (prev == NULL)
    {
        head = cur->next;          /* deleting the first node moves the head */
    }
    else
    {
        prev->next = cur->next;    /* unlink first ... */
    }
    free(cur);                     /* ... then free */
    cur = NULL;
    return head;
}

/* ---------- free the whole list ---------- */

void list_free(struct Node **head_ref)
{
    struct Node *cur = *head_ref;

    while (cur != NULL)
    {
        struct Node *next = cur->next;   /* remember the next node BEFORE free */
        free(cur);
        cur = next;
    }
    *head_ref = NULL;                    /* the caller's head really becomes NULL */
}

int main(void)
{
    struct Node *head = NULL;

    printf("=== 1. build by head insert: 1 2 3 4 ===\n");
    for (int value = 1; value <= 4; value++)
    {
        head = list_insert_head(head, value);
        printf("  after insert_head(%d): ", value);
        list_print(head);
    }
    printf("head insert reverses the input order.\n");
    list_free(&head);
    printf("after list_free: head = %p\n\n", (void *)head);

    printf("=== 2. build by tail insert: 1 2 3 4 ===\n");
    for (int value = 1; value <= 4; value++)
    {
        head = list_insert_tail(head, value);
        printf("  after insert_tail(%d): ", value);
        list_print(head);
    }
    printf("tail insert keeps the input order.\n\n");

    printf("=== 3. length and search ===\n");
    printf("length = %d\n", list_length(head));
    struct Node *found = list_find(head, 3);
    if (found != NULL)
    {
        printf("find(3)  : found, node address = %p, node->data = %d\n",
               (void *)found, found->data);
    }
    found = list_find(head, 99);
    printf("find(99) : %s\n\n", (found == NULL) ? "not found, returned NULL" : "found");

    printf("=== 4. insert at a position ===\n");
    head = list_insert_at(head, 1, 0);
    printf("  insert_at(1, 0)  : ");
    list_print(head);
    head = list_insert_at(head, 4, 25);
    printf("  insert_at(4, 25) : ");
    list_print(head);
    head = list_insert_at(head, 7, 50);
    printf("  insert_at(7, 50) : ");
    list_print(head);
    head = list_insert_at(head, 99, 777);
    printf("  insert_at(99,..) : ");
    list_print(head);
    printf("\n");

    printf("=== 5. delete by value ===\n");
    head = list_delete_value(head, 0);
    printf("  delete(0) first node : ");
    list_print(head);
    head = list_delete_value(head, 25);
    printf("  delete(25) middle    : ");
    list_print(head);
    head = list_delete_value(head, 50);
    printf("  delete(50) last node : ");
    list_print(head);
    head = list_delete_value(head, 1234);
    printf("  delete(1234) missing : ");
    list_print(head);
    printf("\n");

    printf("=== 6. free the whole list ===\n");
    printf("before free: head = %p\n", (void *)head);
    list_free(&head);
    printf("after  free: head = %p, length = %d\n", (void *)head, list_length(head));

    return 0;
}
```

**真实运行输出**（本机 MinGW-W64 8.1.0，`gcc -Wall -Wextra` 零警告）：

```text
=== 1. build by head insert: 1 2 3 4 ===
  after insert_head(1): List (1 nodes): 1 -> NULL
  after insert_head(2): List (2 nodes): 2 -> 1 -> NULL
  after insert_head(3): List (3 nodes): 3 -> 2 -> 1 -> NULL
  after insert_head(4): List (4 nodes): 4 -> 3 -> 2 -> 1 -> NULL
head insert reverses the input order.
after list_free: head = 0000000000000000

=== 2. build by tail insert: 1 2 3 4 ===
  after insert_tail(1): List (1 nodes): 1 -> NULL
  after insert_tail(2): List (2 nodes): 1 -> 2 -> NULL
  after insert_tail(3): List (3 nodes): 1 -> 2 -> 3 -> NULL
  after insert_tail(4): List (4 nodes): 1 -> 2 -> 3 -> 4 -> NULL
tail insert keeps the input order.

=== 3. length and search ===
length = 4
find(3)  : found, node address = 0000000000B32480, node->data = 3
find(99) : not found, returned NULL

=== 4. insert at a position ===
  insert_at(1, 0)  : List (5 nodes): 0 -> 1 -> 2 -> 3 -> 4 -> NULL
  insert_at(4, 25) : List (6 nodes): 0 -> 1 -> 2 -> 25 -> 3 -> 4 -> NULL
  insert_at(7, 50) : List (7 nodes): 0 -> 1 -> 2 -> 25 -> 3 -> 4 -> 50 -> NULL
insert_at(99): past the end, list unchanged
  insert_at(99,..) : List (7 nodes): 0 -> 1 -> 2 -> 25 -> 3 -> 4 -> 50 -> NULL

=== 5. delete by value ===
  delete(0) first node : List (6 nodes): 1 -> 2 -> 25 -> 3 -> 4 -> 50 -> NULL
  delete(25) middle    : List (5 nodes): 1 -> 2 -> 3 -> 4 -> 50 -> NULL
  delete(50) last node : List (4 nodes): 1 -> 2 -> 3 -> 4 -> NULL
delete(1234): value not found, list unchanged
  delete(1234) missing : List (4 nodes): 1 -> 2 -> 3 -> 4 -> NULL

=== 6. free the whole list ===
before free: head = 0000000000B32440
after  free: head = 0000000000000000, length = 0
```

> 两处 `%p` 地址**每次运行都不一样**（本机再跑一次是 `00000000007C2480` / `00000000007C2440`，开了 ASLR），但链表结构和输出文字完全一致 —— 笔记里贴地址只是为了说明"结点到底在哪"，不是要你对齐这串数字。
> 释放后 `head` 打印成全 0 而不是 `(nil)`：MinGW 的 `%p` 把 NULL 打成 16 个 0，这很正常。

## 三、坑清单（L12 新增）

| # | 坑 | 现象 | 正确写法 |
| --- | --- | --- | --- |
| 1 | `next` 写成 `Node *next`（没 typedef） | `unknown type name 'Node'` | 写全 `struct Node *next;`，或先 `typedef struct Node Node;` |
| 2 | 用局部变量当结点：`struct Node n; return &n;` | 函数返回后栈作废，调用者拿到野指针，数据是垃圾 | 结点一律 `malloc`，要活得比函数长就放堆上 |
| 3 | 头插后忘了接收返回值：`list_insert_head(head, x);` | 新结点丢失（顺带泄漏），`head` 还是老的 | `head = list_insert_head(head, x);` |
| 4 | 头插两句顺序写反：`head = new; new->next = head;` | 实测 `new->next == new` 自环，遍历永远到不了 NULL，老链头也丢了（泄漏） | `new->next = head; head = new;` |
| 5 | 尾插忘了 `new_node->next = NULL` | 新尾巴指向垃圾，遍历不终止（死循环 / 崩溃） | `node_create` 里统一 `new_node->next = NULL;` |
| 6 | 插入时先 `prev->next = new` 再接后面 | 后半条链整段丢失（内存泄漏） | 先 `new->next = prev->next;`，再 `prev->next = new;` |
| 7 | 删除时先 `free(cur)` 再摘链 | 读已释放结点的 `next`（use after free），可能崩 | 先 `prev->next = cur->next;`，再 `free(cur);` |
| 8 | 删首结点没更新 `head` | `head` 还指着已 free 的结点，下次遍历就崩 | 特判 `prev == NULL` 时 `head = cur->next;`（或改用头结点） |
| 9 | 整表释放时 `free(cur); cur = cur->next;` | 释放后再读它的 `next`，行为未定义 | 先存 `next = cur->next;` 再 `free(cur);` |
| 10 | 释放完没把 `head` 置 NULL | 悬空头指针，后面误用就崩；也说不清表还在不在 | 传 `struct Node **` 或让函数返回 NULL：`head = list_free(head);` |
| 11 | 遍历条件写成 `cur->next != NULL` | 最后一个结点的 `data` 永远处理不到 | 处理每个结点用 `cur != NULL`；只有"找前驱/找尾"才看 `next` |
| 12 | 想用 `cur++` 走下一个结点 | 编译错或踩到无关内存 | 链表结点不连续，只能 `cur = cur->next;` |

## 四、复习自查清单

- [ ] 不查资料默写结点定义，并说清为什么 `next` 要写 `struct Node *`
- [ ] 白纸上画出"头插 30 到 10→20"的两步图，标出每一步谁指向谁
- [ ] 白纸上画出"删除中间结点"的两步图，说明为什么必须先摘链再 free
- [ ] 说清头插法为什么逆序、尾插法为什么保持顺序，各自的时间复杂度
- [ ] 说不清 `head = list_insert_head(head, x)` 里那个赋值会怎样？（提示：新结点去哪了）
- [ ] 解释为什么 `list_free` 要用 `struct Node **`，而别的函数用返回值就够
- [ ] 解释 `list_length` / `list_print` 的形参为什么要加 `const`
- [ ] 不看笔记写出：遍历打印、求长度、按值查找、按位置插入、按值删除、整表释放
- [ ] 手算一遍模板 6 的第 4 步：`insert_at(7, 50)` 为什么是插在末尾，`insert_at(99, 777)` 为什么不改表
- [ ] 说出头指针与头结点的区别，以及带头结点后"删首结点特判"为什么消失了
- [ ] 手抄一遍王道 `typedef struct LNode { ... } LNode, *LinkList;`，并指出 `L` 是什么类型
- [ ] 全部代码 `-Wall -Wextra` 零警告跑通，且跑完后每块内存都被 free（自己讲一遍释放顺序）

## 五、作业预告（开课当天随讲义下发，含通关测试值）

> 本笔记不剧透具体规格与通关测试值：题号、输入范围、输出格式、判定用测试值全部在开课当天讲义里给出。

- **HW1 主题：建表与遍历** —— 两种建表法各写一遍，按指定格式把整条链打印出来，注意空表的输出。
- **HW2 主题：查找 / 插入 / 删除** —— 在指定位置插入、按值删除（含首结点、尾结点、值不存在三种情况）。
- **HW3 主题：求长度与整表释放** —— 遍历计数，最后把整条链完整释放并让头指针归零，重复运行不残留。
- **HW4（挑战 ★★★）主题：带头结点的版本 + 有序插入** —— 改成王道风格的 `LinkList` / `LNode`，并把插入改造成"保持递增有序"。

## 参考 1：链表操作文字内存图

```text
a node in memory (28 bytes on this machine)
+----------------+----------------+
|  data (4)      |  next (8)      |  + 16 bytes of padding/alignment
+----------------+----------------+
   ^ the element    ^ how to reach the next element

an empty list
    head = NULL

a list of three nodes (nodes are NOT next to each other in memory)
    head
     |
     v
   +----+----+     +----+----+     +----+----+
   | 10 |  *-|---->| 20 |  *-|---->| 30 |NULL|
   +----+----+     +----+----+     +----+----+
```

```text
HEAD INSERT (put 30 in front of 10 -> 20)

step 1:  new_node->next = head;
             head ---> [10|*] -> [20|NULL]
                        ^
             new  ---> [30|*]

step 2:  head = new_node;
             head ---> [30|*] -> [10|*] -> [20|NULL]

result: the list is reversed compared with the input order.
```

```text
TAIL INSERT (append 40 after 10 -> 20 -> 30)

step 1:  walk to the last node
             head -> [10|*] -> [20|*] -> [30|NULL]
                                          ^
                                         tail

step 2:  tail->next = new_node;    (new_node->next is already NULL)
             head -> [10|*] -> [20|*] -> [30|*] -> [40|NULL]
```

```text
INSERT AFTER prev (put X between B and C)

WRONG order (link backward first):
    prev->next = new_node;        [B|*] -> [X|?]        and C is LOST
    new_node->next = prev->next;  [X|*] -> [X]          self loop!

RIGHT order (link forward first):
    new_node->next = prev->next;  [X|*] -> [C|...]
    prev->next = new_node;        [B|*] -> [X|*] -> [C|...]
```

```text
DELETE the middle node C

before:  head -> [A|*] -> [B|*] -> [C|*] -> [D|NULL]
                          prev      cur

step 1:  prev->next = cur->next;      [B|*] ---------> [D|NULL]
                                      [C|*]  unlinked but still allocated

step 2:  free(cur);                   the block goes back to the heap
         cur = NULL;                  and the pointer stops existing

DELETE THE FIRST node A
         head = cur->next;      head -> [B|*] -> [C|*] -> ...
         free(cur);             (without this line, head would dangle)
```

## 参考 2：头指针 vs 头结点

| | 头指针 head pointer | 头结点 dummy head node |
| --- | --- | --- |
| 是什么 | 指向第一个结点的指针，链表的名字 | 第一个数据结点**之前**额外放的一个结点，`data` 不用（或存表长） |
| 空表长什么样 | `head == NULL` | 头结点存在，`head->next == NULL` |
| 能否省 | **不能**，省了就没法访问整条链 | 可以省（本课模板 6 就没用） |
| 好处 | 省 4/28 字节，概念最直观 | 插入/删除第一个位置**不用特判**；空表非空表处理统一；头指针永不改变 |
| 代价 | 头插、删首结点必须把新 head 还回去 | 多一个结点；遍历要从 `head->next` 开始 |

一句话：**头指针是"必须有的名字"，头结点是"为了少写 if 而加的哨兵"**。王道教材默认带头结点，考试答题时看题目给的是 `LinkList L` 还是 `LNode *L`，跟着题目的约定走。

## 参考 3：与 408 王道 `LinkList` / `LNode` 的对应关系

王道标准写法就一行 typedef 解决两个名字：

```c
typedef struct LNode
{
    int data;                 /* the textbook writes ElemType data; */
    struct LNode *next;
} LNode, *LinkList;           /* LNode = the node type, LinkList = the pointer type */
```

| 王道写法 | 本课模板 6 的写法 | 说明 |
| --- | --- | --- |
| `LinkList L;` | `struct Node *head;` | `LinkList` 本身就是**指针类型**，`L` 是头指针 |
| `LNode *p = L->next;` | `struct Node *cur = head;` | 带头结点时，第一个数据结点是 `L->next` |
| `L->next = NULL;` 建空表 | `head = NULL;` | 带头结点时"空表"是头结点还在、`next` 为 NULL |
| `s = (LNode *)malloc(sizeof(LNode));` | `node_create(value)` | 结点统一在堆上生 |
| 头插 `s->next = L->next; L->next = s;` | `new_node->next = head; head = new_node;` | 都是"先接后断" |
| 尾插 `r->next = s; r = s;` | `tail->next = new_node;` | 王道用**尾指针 r** 记住尾巴，避免每次 O(n) 找尾 |
| `ListLength(L)` | `list_length(head)` | 遍历计数 |
| `LocateElem(L, e)` | `list_find(head, value)` | 王道常按位查找（`GetElem`），本课先按值 |
| `ListInsert(&L, i, e)` | `head = list_insert_at(head, pos, value);` | 不带头结点必须把新 head 传回去 |
| `ListDelete(&L, i, &e)` | `head = list_delete_value(head, value);` | 同上；被删的值用 `&e` 带回 |
| `while (p != NULL) { p = p->next; }` | 完全相同 | 遍历的唯一写法，背下来 |

**带头结点版本长什么样**（与本课模板 6 的差别只有三处，实测输出见下方）：

```c
typedef struct LNode
{
    int data;
    struct LNode *next;
} LNode, *LinkList;

LinkList list_init(void)              /* create the dummy head: LinkList L = list_init(); */
{
    LinkList head = (LinkList)malloc(sizeof(LNode));
    if (head == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
    head->data = 0;                   /* the dummy node holds no real data */
    head->next = NULL;
    return head;
}

void list_insert_head(LinkList head, int value)   /* void return: head never changes */
{
    LNode *new_node = (LNode *)malloc(sizeof(LNode));
    if (new_node == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
    new_node->data = value;
    new_node->next = head->next;      /* same two lines as before ... */
    head->next = new_node;            /* ... but there is no "new head" to return */
}

int list_delete_value(LinkList head, int value)   /* no "first node" special case */
{
    LNode *prev = head;               /* prev starts AT the dummy head */
    while (prev->next != NULL && prev->next->data != value)
    {
        prev = prev->next;
    }
    if (prev->next == NULL)
    {
        return 0;                     /* not found */
    }
    LNode *victim = prev->next;
    prev->next = victim->next;        /* unlink ... */
    free(victim);                     /* ... then free */
    return 1;
}
```

实测输出（`gcc -Wall -Wextra` 零警告）：

```text
tail insert 1 2 3 : List (3 nodes): 1 -> 2 -> 3 -> NULL
head insert 0     : List (4 nodes): 0 -> 1 -> 2 -> 3 -> NULL
length = 4
delete 2: ok
after delete      : List (3 nodes): 0 -> 1 -> 3 -> NULL
delete 1234: not found
after delete      : List (3 nodes): 0 -> 1 -> 3 -> NULL
list itself (the dummy node) address = 0000000000BC2440
after free list = 0000000000000000
```

> 对照着看最清楚：**带头结点后，`list_insert_head` 连返回值都不用要了**（头指针永远不变），删除也不用特判第一个结点。这就是王道教材爱用头结点的原因；但"不带头结点"版本更能逼你搞懂"head 什么时候会变"。两个都要会。

> 下一课预告方向：把单链表升级成**双向链表 / 循环链表**，以及 408 最爱考的**顺序表 vs 链表**取舍分析 —— 今天这份代码就是地基。
