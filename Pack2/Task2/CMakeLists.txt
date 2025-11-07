cmake_minimum_required(VERSION 3.10)
project(Pack2_Task1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(main
    src/main.c 
    src/task_funcs.c 
)