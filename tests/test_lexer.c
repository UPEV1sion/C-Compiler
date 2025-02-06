#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lexer/lexer.h"

#define test_tok(lexer, tok_type) { \
    Token *tok = consume_tok(lexer); \
    CU_ASSERT_PTR_NOT_NULL((tok)); \
    CU_ASSERT(tok->type == (tok_type)); \
    free((tok)); \
}

void test_math(void)
{
    const char *math_file = "math.c";
    FILE *file = fopen(math_file, "w");
    CU_ASSERT_PTR_NOT_NULL(file)
    fclose(file);

    Lexer *lexer = create_lexer(TEST_DIR "/math.c");
    CU_ASSERT_PTR_NOT_NULL(lexer)

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokInt))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokIdentifier))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokAssign))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokIntLiteral))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokPlus))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokIntLiteral))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokSemicolon))
        free((tok));
    }

    {
        Token *tok = consume_tok(lexer);
        CU_ASSERT_PTR_NOT_NULL((tok))
        CU_ASSERT(tok->type == (TokEOF))
        free((tok));
    }

    cleanup_lexer(lexer);
}

void test_empty_file(void)
{
    const char *empty_file = "empty.c";
    FILE *file = fopen(empty_file, "w");
    CU_ASSERT_PTR_NOT_NULL(file)
    fclose(file);

    Lexer *lexer = create_lexer(empty_file);

    test_tok(lexer, TokEOF)

    cleanup_lexer(lexer);
}

void test_malformed_characters(void)
{
    const char *malformed_file = "malformed.c";
    FILE *file = fopen(malformed_file, "w");
    CU_ASSERT_PTR_NOT_NULL(file)
    fprintf(file, "'\n");
    fclose(file);

    Lexer *lexer = create_lexer(malformed_file);

    test_tok(lexer, TokIllegal)

    cleanup_lexer(lexer);
}

void test_comments(void)
{
    const char *comments_file = "comments.c";
    FILE *file = fopen(comments_file, "w");
    CU_ASSERT_PTR_NOT_NULL(file)
    fprintf(file, "int main(void) { // This is a comment\n int x = 0; /* Another comment */ }\n");
    fclose(file);

    //TODO

}

void test_string_literals(void)
{
    const char *string_file = "string_literals.c";
    FILE *file = fopen(string_file, "w");
    CU_ASSERT_PTR_NOT_NULL(file)
    fprintf(file, "\"Hello, World!\";\n");
    fclose(file);

    Lexer *lexer = create_lexer(string_file);

    Token *tok = consume_tok(lexer);
    CU_ASSERT_PTR_NOT_NULL(tok)
    CU_ASSERT(tok->type == TokStringLiteral)
    CU_ASSERT(strcmp(tok->literal, "Hello, World!") == 0)
    free(tok);

    cleanup_lexer(lexer);
}

void test_edge_cases(void)
{
    const char edge_cases_file[] = "edge_cases.c";
    FILE *file = fopen(edge_cases_file, "w");
    CU_ASSERT_PTR_NOT_NULL(file)
    fprintf(file, "int a = 5; // Single-line comment\n /* multi-line\n comment */");
    fclose(file);

    //TODO
}

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("Lexer Test Suite", 0, 0);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Test Empty File", test_empty_file);
    CU_add_test(suite, "Test Malformed Characters", test_malformed_characters);
    CU_add_test(suite, "Test Lexer", test_math);
    CU_add_test(suite, "Test Comments", test_comments);
    CU_add_test(suite, "Test String Literals", test_string_literals);
    CU_add_test(suite, "Test Edge Cases", test_edge_cases);

    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}
