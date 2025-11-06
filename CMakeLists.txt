cmake_minimum_required(VERSION 3.10)
project(SimpleDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    src/Database.cpp
    src/Entity.cpp
    src/Exception.cpp
    src/Field.cpp
    src/main.cpp
    src/Manager.cpp
    src/Record.cpp
    src/Schema.cpp
    src/StateMachine.cpp
    src/Table.cpp
    src/Utils.cpp
)

# 可执行文件
add_executable(SimpleDB ${SOURCES})