# SimpleDB - 简易数据库管理系统

## 项目简介

SimpleDB 是一个基于 C++ 开发的简易数据库管理系统，支持多级数据管理（Manager -> Database -> Table -> Record -> Field），提供基本的数据库操作功能。

## 功能特性

- 🗄️ **多级数据管理**：Manager -> Database -> Table -> Record -> Field
- 📝 **完整的 CRUD 操作**：创建、查询、更新、删除
- 💾 **数据持久化**：自动保存到文件系统
- 🎯 **类型安全**：支持 INT、BOOL、FLOAT、STRING 数据类型
- 🚀 **交互式命令行界面**：友好的用户交互体验

## 系统架构

```
Manager (顶级管理)
  ↓
Database (数据库)
  ↓
Table (数据表)
  ↓
Record (记录)
  ↓
Field (字段)
```

## 快速开始

### 编译项目

```bash
# 使用 CMake
mkdir build && cd build
cmake ..
make

# 或直接使用 g++
g++ -std=c++17 -Iinclude src/*.cpp -o SimpleDB
```

### 运行程序

```bash
./SimpleDB
```

## 命令使用指南

### 全局命令

| 命令     | 说明       | 示例     |
| -------- | ---------- | -------- |
| `EXIT` | 退出系统   | `EXIT` |
| `BACK` | 返回上一级 | `BACK` |

### Manager 级别命令

在系统启动时的顶级管理界面使用：

| 命令                      | 语法                                     | 说明           | 示例                                 |
| ------------------------- | ---------------------------------------- | -------------- | ------------------------------------ |
| **CREATE DATABASE** | `CREATE DATABASE <db_name>`            | 创建数据库     | `CREATE DATABASE school`           |
| **DROP DATABASE**   | `DROP DATABASE <db_name>`              | 删除数据库     | `DROP DATABASE school`             |
| **ALTER DATABASE**  | `ALTER DATABASE <old_name> <new_name>` | 重命名数据库   | `ALTER DATABASE school university` |
| **USE DATABASE**    | `USE DATABASE <db_name>`               | 进入数据库     | `USE DATABASE school`              |
| **SELECT DATABASE** | `SELECT DATABASE`                      | 查看所有数据库 | `SELECT DATABASE`                  |

### Database 级别命令

在数据库界面使用：

| 命令                   | 语法                                              | 说明           | 示例                                                 |
| ---------------------- | ------------------------------------------------- | -------------- | ---------------------------------------------------- |
| **CREATE TABLE** | `CREATE TABLE <table_name> <schema_definition>` | 创建数据表     | `CREATE TABLE students id INT name STRING age INT` |
| **DROP TABLE**   | `DROP TABLE <table_name>`                       | 删除数据表     | `DROP TABLE students`                              |
| **ALTER TABLE**  | `ALTER TABLE <old_name> <new_name>`             | 重命名数据表   | `ALTER TABLE students pupils`                      |
| **USE TABLE**    | `USE TABLE <table_name>`                        | 进入数据表     | `USE TABLE students`                               |
| **SELECT TABLE** | `SELECT TABLE`                                  | 查看所有数据表 | `SELECT TABLE`                                     |

**Schema 定义格式：**

```
<field1_name> <data_type> <field2_name> <data_type> ...
```

支持的数据类型：`INT`, `BOOL`, `FLOAT`, `STRING`

### Table 级别命令

在数据表界面使用：

| 命令                    | 语法                                                                                     | 说明               | 示例                                            |
| ----------------------- | ---------------------------------------------------------------------------------------- | ------------------ | ----------------------------------------------- |
| **INSERT RECORD** | `INSERT RECORD <field1_value> <field2_value> ...`                                      | 插入记录           | `INSERT RECORD 1 "Alice" 20`                  |
| **REMOVE RECORD** | `REMOVE RECORD WHERE <field_name> <value>`                                             | 删除符合条件的记录 | `REMOVE RECORD WHERE id 1`                    |
| **UPDATE RECORD** | `UPDATE RECORD SET <field_name> <new_value> WHERE <condition_field> <condition_value>` | 更新记录           | `UPDATE RECORD SET age 21 WHERE name "Alice"` |
| **SELECT RECORD** | `SELECT RECORD WHERE <field_name> <value>`                                             | 查询符合条件的记录 | `SELECT RECORD WHERE age 20`                  |
| **SELECT RECORD** | `SELECT RECORD *`                                                                      | 查询所有记录       | `SELECT RECORD *`                             |

## 完整使用示例

### 1. 启动系统

```bash
./SimpleDB
```

### 2. 创建数据库

```
Manager > CREATE DATABASE school
Database school created

Manager > USE DATABASE school
```

### 3. 创建数据表

```
Database:school > CREATE TABLE students id INT name STRING age INT score FLOAT
Table students created

Database:school > USE TABLE students
```

### 4. 插入记录

```
Table:students > INSERT RECORD 1 "Alice" 20 95.5
Record inserted successfully

Table:students > INSERT RECORD 2 "Bob" 21 88.0
Record inserted successfully

Table:students > INSERT RECORD 3 "Charlie" 19 92.5
Record inserted successfully
```

### 5. 查询记录

```
Table:students > SELECT RECORD *
TABLE VIEW:

       >> id                 |name               |age                |score               |
       >> ---------------------------------------------------------------------------------
         1 >> 1                 |Alice              |20                 |95.5                |
         2 >> 2                 |Bob                |21                 |88                  |
         3 >> 3                 |Charlie            |19                 |92.5                |
       >> ---------------------------------------------------------------------------------
```

### 6. 条件查询

```
Table:students > SELECT RECORD WHERE age 20
RECORDS SELECTED:

       >> id                 |name               |age                |score               |
       >> ---------------------------------------------------------------------------------
         1 >> 1                 |Alice              |20                 |95.5                |
       >> ---------------------------------------------------------------------------------
```

### 7. 更新记录

```
Table:students > UPDATE RECORD SET score 96.0 WHERE name "Alice"
Records modified successfully
```

### 8. 删除记录

```
Table:students > REMOVE RECORD WHERE name "Bob"
Records removed successfully
```

### 9. 返回上级

```
Table:students > BACK

Database:school > BACK

Manager > SELECT DATABASE
DATABASE VIEW:

         1 >> school
```

### 10. 退出系统

```
Manager > EXIT
System exited with everything saved
```

## 数据类型说明

| 数据类型   | 说明       | 示例值                   |
| ---------- | ---------- | ------------------------ |
| `INT`    | 整数类型   | `123`, `-45`         |
| `BOOL`   | 布尔类型   | `TRUE`, `FALSE`      |
| `FLOAT`  | 浮点数类型 | `3.14`, `-2.5`       |
| `STRING` | 字符串类型 | `"Hello"`, `"Alice"` |

## 文件存储结构

系统数据保存在 `./db_storage/` 目录下：

```
db_storage/
├── school/                 # 数据库目录
│   ├── students.txt       # 数据表文件
│   └── teachers.txt
├── company/
│   └── employees.txt
└── ...
```

## 错误处理

系统提供详细的错误信息，常见错误包括：

- 语法错误：命令格式不正确
- 名称冲突：数据库/表已存在
- 找不到对象：数据库/表不存在
- 类型不匹配：字段数据类型错误
- 文件操作错误：读写权限问题

## 开发说明

### 项目结构

```
SimpleDB/
├── include/           # 头文件
├── src/              # 源文件
├── build/            # 编译输出
└── db_storage/       # 数据存储
```

### 核心类说明

- `Manager`: 顶级管理器，管理所有数据库
- `Database`: 数据库，管理所有数据表
- `Table`: 数据表，包含表结构和记录
- `Record`: 记录，包含多个字段
- `Field`: 字段，包含具体数据和类型
- `Schema`: 表结构定义
- `StateMachine`: 状态机，管理命令执行流程

## 许可证

Copyright (c) 2025 Jilin University Software Institute

## 技术支持

如遇到问题，请检查：

1. 命令语法是否正确
2. 数据类型是否匹配
3. 文件读写权限
4. 存储空间是否充足

享受使用 SimpleDB！ 🎉
