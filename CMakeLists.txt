cmake_minimum_required(VERSION 3.10)
project(SimpleDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

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

add_executable(SimpleDB ${SOURCES})