//
// Created by escha on 30.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/ast.h"

int main(const int argc, const char *argv[])
{
    if (argc < 2) return 1;

    Lexer *lexer = create_lexer(argv[1]);
    Token *tok;
    while((tok = consume_tok(lexer))->type != TokEOF)
    {
        puts(tok->literal);
        free(tok);
    }

    ASTNode *root = build_AST(lexer);
    (void) root;

    cleanup_lexer(lexer);

    return 0;
}
