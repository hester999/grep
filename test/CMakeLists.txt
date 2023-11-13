cmake_minimum_required(VERSION 3.26)
project(grep C)

set(CMAKE_C_STANDARD 11)

add_executable(grep main.c
        parse.c
        grep.c
        grep.h)
