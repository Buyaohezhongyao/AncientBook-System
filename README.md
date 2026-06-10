# 基于 C 语言链表与文件存储的中外古籍二手书管理系统

## 1. 项目简介

本项目是 C 语言程序设计课程综合实验项目，题目为《基于 C 语言链表与文件存储的中外古籍二手书管理系统设计与实现》。系统面向小型古籍二手书店或古籍流通书屋，使用单向链表管理内存中的古籍数据，并使用文本文件保存数据，实现古籍信息维护、查询、库存、销售、排序和统计等功能。

项目使用纯 C 语言编写，适合在 Dev-C++、Code::Blocks、VS Code + MinGW、Linux/macOS gcc 等常见环境中编译运行。

## 2. 功能列表

- 添加古籍信息：检查编号重复、价格、库存、销量和日期格式。
- 浏览全部古籍：以表格形式显示主要字段。
- 查询古籍信息：支持按编号、书名关键词、作者、类别、国别/地区、收录日期、来源和架位号查询。
- 修改古籍信息：根据编号定位后修改书名、作者、类别、年代、品相、日期、来源、架位、价格、库存等信息。
- 删除古籍信息：删除前显示目标记录并要求确认。
- 销售管理：销售时扣减库存、增加销量，并可追加写入 `data/sales_log.txt`。
- 进货管理：根据编号增加库存。
- 排序显示：支持按价格、库存、销量和编号升序或降序排序。
- 统计分析：统计种类数、库存量、库存估值、销量、估算销售额、销量最高、库存最低、低库存预警、按来源和类别统计、按日期筛选新收录古籍。
- 文件读写：启动读取 `data/books.txt`，支持手动保存和退出前保存。
- 输入容错：对菜单、数字、日期和空输入进行基础检查。

## 3. 目录结构

```text
AncientBookSystem/
├── README.md
├── src/
│   ├── main.c
│   ├── book.h
│   ├── book.c
│   ├── fileio.h
│   ├── fileio.c
│   ├── utils.h
│   └── utils.c
├── data/
│   ├── books.txt
│   └── sales_log.txt
├── tests/
│   ├── test_input.txt
│   ├── test_output_expected.txt
│   ├── run_test.bat
│   └── run_test.sh
├── report/
│   ├── main.tex
│   ├── references.bib
│   ├── figures/
│   └── sections/
└── build/
```

## 4. 编译环境

推荐环境：

- C 标准：C99 或 C11
- 编译器：gcc / MinGW gcc / clang
- 操作系统：Windows、Linux、macOS 均可

本项目未使用 C++ 特性，不依赖第三方库。

## 5. 编译命令

请在项目根目录 `AncientBookSystem` 下执行：

```bash
gcc -std=c99 -Wall -Wextra src/main.c src/book.c src/fileio.c src/utils.c -o build/AncientBookSystem
```

Windows MinGW 可使用：

```bat
gcc -std=c99 -Wall -Wextra src\main.c src\book.c src\fileio.c src\utils.c -o build\AncientBookSystem.exe
```

如需使用更简洁的编译命令，也可以执行：

```bash
gcc src/main.c src/book.c src/fileio.c src/utils.c -o AncientBookSystem
```

## 6. 运行方式

Linux/macOS：

```bash
./build/AncientBookSystem
```

Windows：

```bat
build\AncientBookSystem.exe
```

注意：程序默认从相对路径 `data/books.txt` 读取数据，因此建议始终在项目根目录运行程序。

## 7. 数据文件格式说明

`data/books.txt` 使用 UTF-8 文本格式，每行一条古籍记录，字段使用竖线 `|` 分隔：

```text
id|title|author|country|category|period|condition|collectionDate|source|shelfCode|price|stock|sold
```

示例：

```text
B001|史记|司马迁|中国|史部|汉代|影印本|2026-05-01|私人旧藏|A-01-01|128.00|5|2
```

为避免文件解析错误，输入字段中不要包含竖线 `|`。

## 8. 测试方式

自动化测试脚本位于 `tests/` 目录，会编译程序并使用 `tests/test_input.txt` 作为输入，输出到 `tests/test_output_actual.txt`。

Linux/macOS：

```bash
sh tests/run_test.sh
```

Windows：

```bat
tests\run_test.bat
```

脚本会在测试前备份 `data/books.txt`，测试结束后恢复原始示例数据，方便重复测试。

## 9. 常见问题

### 中文乱码

本项目源码和数据文件建议统一保存为 UTF-8 编码。

Windows 控制台如出现中文乱码，可尝试在运行前执行：

```bat
chcp 65001
```

如果 Dev-C++ 或旧版 Code::Blocks 显示异常，请检查编辑器源码编码和控制台编码是否一致。

### 文件路径错误

程序使用相对路径 `data/books.txt` 和 `data/sales_log.txt`。如果直接在 `build/` 目录内运行可执行文件，可能找不到数据文件。建议进入项目根目录后再运行程序。

### 表格没有完全对齐

中英文混排时，不同终端对中文字符宽度的处理可能不同，因此表格列宽可能略有差异。系统同时提供单条记录的详细显示，便于检查完整信息。
