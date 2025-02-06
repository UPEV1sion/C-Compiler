find_package(PkgConfig REQUIRED)
pkg_check_modules(CUnit REQUIRED cunit)

add_executable(my_tests
        test_lexer.c
        ../src/lexer/lexer.c
        ../src/lexer/lexer.h
)

target_include_directories(my_tests PRIVATE ${CUnit_INCLUDE_DIRS})

target_link_libraries(my_tests PRIVATE ${CUnit_LIBRARIES})

add_test(NAME LexerTest COMMAND my_tests)
