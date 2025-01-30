cmake_minimum_required(VERSION 3.30)
project(ecc C)

set(CMAKE_C_STANDARD 11)

add_executable(ecc src/main.c
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/parser/ast.c
        src/parser/ast.h)
