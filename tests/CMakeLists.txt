find_package(PkgConfig REQUIRED)
pkg_check_modules(CUnit REQUIRED cunit)

add_executable(my_tests
        test_lexer.c
        ../src/lexer/lexer.c
        ../src/lexer/lexer.h
)

set(TESTS_DIR "${CMAKE_SOURCE_DIR}/tests")
add_definitions(-DTEST_DIR="${TESTS_DIR}")

target_include_directories(my_tests PRIVATE ${CUnit_INCLUDE_DIRS})

target_link_libraries(my_tests PRIVATE ${CUnit_LIBRARIES})

add_test(NAME LexerTest COMMAND my_tests)
