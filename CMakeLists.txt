cmake_minimum_required(VERSION 3.22)
project(ecc C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(tests)

add_executable(ecc
        src/main.c
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/parser/ast.c
        src/parser/ast.h
        src/helper/helper.c
        src/helper/helper.h
)

target_compile_options(ecc PRIVATE -Wall -Wextra -Werror -pedantic)
