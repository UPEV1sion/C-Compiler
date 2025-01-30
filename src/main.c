//
// Created by escha on 30.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"

int main(const int argc, const char *argv[])
{
    if (argc < 2) return 1;

    Lexer *lexer = create_lexer(argv[1]);
    Token *tok;
    while((tok = next_tok(lexer))->type != TokEOF)
    {
        puts(tok->literal);
        free(tok);
    }

    cleanup_lexer(lexer);

    return 0;
}
