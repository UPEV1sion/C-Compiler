//
// Created by escha on 30.01.25.
//

#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum
{
    MAX_TOK_LEN = 64
};

#define is_data_type(c) ((c) >= TokChar && (c) <= TokULongLong)
#define is_num_literal(c) ((c) >= TokCharLiteral && (c) <= TokUnsignedLongLongLiteral)

struct Lexer
{
    FILE *file;
    size_t pos;
    size_t line;
    size_t column;
    int current_char;
    Token *prev_token;
    Token *next_token;
};

int cleanup_lexer(Lexer *lexer)
{
    if (fclose(lexer->file) != 0)
    {
        fprintf(stderr, "Couldn't close file!");
        return -1;
    }
    free(lexer);

    return 0;
}

Lexer *create_lexer(const char *filename)
{
    Lexer *lexer = malloc(sizeof(Lexer));
    if (lexer == NULL)
    {
        fprintf(stderr, "Couldn't create Lexer!");
        return lexer;
    }

    lexer->file = fopen(filename, "r");
    if (lexer->file == NULL)
    {
        fprintf(stderr, "Couldn't open input file!");
        free(lexer);
        return NULL;
    }

    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;

    lexer->current_char = fgetc(lexer->file);

    return lexer;
}

static Token *create_tok(const TokenType type, const char *literal)
{
    Token *tok = malloc(sizeof(Token));
    if (tok == NULL)
    {
        fprintf(stderr, "Couldn't create Token!");
        return NULL;
    }

    tok->type = type;
    tok->literal = literal;

    return tok;
}

static void skip_spaces(Lexer *lexer)
{
    while (isspace(lexer->current_char))
    {
        if (lexer->current_char == '\n')
        {
            lexer->line++;
            lexer->column = 1;
        }
        else
        {
            lexer->column++;
        }

        lexer->current_char = fgetc(lexer->file);
    }
}

static void next_char(Lexer *lexer)
{
    lexer->current_char = fgetc(lexer->file);
}


static Token *
match_compound_operator(Lexer *lexer, const char base, const TokenType assignment, const TokenType normal)
{
    Token *tok;

    if (lexer->current_char == '=') // Compound Operators like += *= ...
    {
        tok = create_tok(assignment, (char[]) {base, '=', 0});
    }
    else
    {
        tok = create_tok(normal, (char[]) {base, 0});
        ungetc(lexer->current_char, lexer->file);
    }

    return tok;
}

Token *next_tok(Lexer *lexer)
{
    Token *tok = NULL;
    if (lexer->next_token != NULL)
    {
        tok = lexer->next_token;
        lexer->next_token = NULL;
        return tok;
    }

    skip_spaces(lexer);

    switch (lexer->current_char)
    {
        case '=':
            next_char(lexer);
            if (lexer->current_char == '=') // ==
            {
                tok = create_tok(TokEqual, "==");
            }
            else
            {
                tok = create_tok(TokAssign, "=");
                ungetc(lexer->current_char, lexer->file);
            }
            break;
        case '+':
            next_char(lexer);
            if (lexer->current_char == '+') // ++
            {
                tok = create_tok(TokIncrement, "++");
            }
            else
            {
                tok = match_compound_operator(lexer, '+', TokPlusAssign, TokPlus);
            }
            break;
        case '-':
            next_char(lexer);
            if (lexer->current_char == '-') // --
            {
                tok = create_tok(TokDecrement, "--");
            }
            else
            {
                tok = match_compound_operator(lexer, '-', TokMinusAssign, TokMinus);
            }
            break;
        case '*':
            next_char(lexer);
            tok = match_compound_operator(lexer, '*', TokStarAssign, TokStar);
            break;
        case '/':
            next_char(lexer);
            tok = match_compound_operator(lexer, '/', TokSlashAssign, TokSlash);
            break;
        case '<':
            next_char(lexer);
            if (lexer->current_char == '<')
            {
                next_char(lexer);
                if (lexer->current_char == '=')
                {
                    tok = create_tok(TokBitwiseSLAssign, "<<=");
                }
                else
                {
                    tok = create_tok(TokBitwiseSL, "<<");
                    ungetc(lexer->current_char, lexer->file);
                }
            }
            else
            {
                tok = match_compound_operator(lexer, '<', TokLT, TokLess);
            }
            break;
        case '>':
            next_char(lexer);
            if (lexer->current_char == '>')
            {
                next_char(lexer);
                if (lexer->current_char == '=')
                {
                    tok = create_tok(TokBitwiseSRAssign, ">>=");
                }
                else
                {
                    tok = create_tok(TokBitwiseSR, ">>");
                    ungetc(lexer->current_char, lexer->file);
                }
            }
            else
            {
                tok = match_compound_operator(lexer, '>', TokGT, TokGreater);
            }
            break;
        case '!':
            next_char(lexer);
            tok = match_compound_operator(lexer, '!', TokNotEqual, TokNegation);
            break;
        case '|':
            next_char(lexer);
            if (lexer->current_char == '|')
            {
                tok = create_tok(TokOrOr, "||");
            }
            else
            {
                tok = match_compound_operator(lexer, '|', TokBitwiseOrAssign, TokOr);
            }
            break;
        case '&':
            next_char(lexer);
            if (lexer->current_char == '&')
            {
                tok = create_tok(TokAndAnd, "&&");
            }
            else
            {
                tok = match_compound_operator(lexer, '&', TokBitwiseAndAssign, TokAnd);
            }
            break;
        case '^':
            next_char(lexer);
            tok = match_compound_operator(lexer, '^', TokBitwiseXorAssign, TokBitwiseXor);
            break;
        case '~':
            next_char(lexer);
            tok = match_compound_operator(lexer, '~', TokBitwiseNotAssign, TokBitwiseNot);
            break;
        case EOF:
            tok = create_tok(TokEOF, "EOF");
            break;
        default:
            tok = NULL;
            break;
    }

    lexer->prev_token = tok;
    next_char(lexer);

    return tok;
}