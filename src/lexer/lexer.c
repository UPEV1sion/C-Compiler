//
// Created by escha on 30.01.25.
//

#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Lexer
{
    FILE *file;
    size_t pos;
    size_t line;
    size_t column;
    int current_char;
    Token *current_tok;
    Token *next_tok;
};

struct keyword
{
    const char *keyword;
    TokenType type;
};

static const struct keyword KEYWORDS[] = {
        /*--------KEYWORDS: DATA TYPES--------*/
        {"char",     TokChar},
        {"short",    TokShort},
        {"int",      TokInt},
        {"long",     TokLong},
        {"float",    TokFloat},
        {"double",   TokDouble},
        {"void",     TokVoid},
        {"unsigned", TokUnsigned},
        {"struct",   TokStruct},
        {"union",    TokUnion},
        {"enum",     TokEnum},
        {"typedef",  TokTypedef},

        /*--------KEYWORDS: STORAGE CLASS SPECIFIERS--------*/
        {"const",    TokConst},
        {"volatile", TokVolatile},
        {"restrict", TokRestrict},
        {"auto",     TokAuto},
        {"register", TokRegister},
        {"static",   TokStatic},
        {"extern",   TokExtern},
        {"inline",   TokInline},

        /*--------KEYWORDS: CONTROL FLOW--------*/
        {"if",       TokIf},
        {"else",     TokElse},
        {"for",      TokFor},
        {"while",    TokWhile},
        {"do",       TokDo},
        {"switch",   TokSwitch},
        {"case",     TokCase},
        {"default",  TokDefault},
        {"return",   TokReturn},
        {"break",    TokBreak},
        {"continue", TokContinue},
        {"goto",     TokGoto},

        /*--------KEYWORDS: OTHER--------*/
        {"sizeof",   TokSizeof},
};

static Token *create_tok(const TokenType type, const char *literal)
{
    Token *tok = malloc(sizeof(Token));
    if (tok == NULL)
    {
        fprintf(stderr, "Couldn't create Token!");
        return NULL;
    }

    tok->type = type;
    strncpy(tok->literal, literal, MAX_TOK_LEN);
    tok->literal[MAX_TOK_LEN - 1] = 0;

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


static void skip_comments(Lexer *lexer);

static int next_char(Lexer *lexer)
{

    lexer->current_char = fgetc(lexer->file);
    if (lexer->current_char == '\n')
    {
        lexer->column = 1;
        lexer->line++;
    }
    else
    {
        lexer->column++;
    }
    skip_comments(lexer);

    return lexer->current_char;
}

static void skip_line(Lexer *lexer)
{
    while ((lexer->current_char = fgetc(lexer->file)) != '\n' && lexer->current_char != EOF);
    if (lexer->current_char == '\n')
    {
        lexer->current_char = fgetc(lexer->file);
        lexer->column = 1;
        lexer->line++;
    }
}

static void skip_multiline_comment(Lexer *lexer)
{
    do
    {
        lexer->current_char = fgetc(lexer->file);
        if (lexer->current_char == '\n')
        {
            lexer->line++;
            lexer->column = 1;
        }
        else
        {
            lexer->column++;
        }
    } while (lexer->current_char != '*' && lexer->current_char != EOF);

    if (lexer->current_char == '*')
    {
        lexer->current_char = fgetc(lexer->file);
        if (lexer->current_char == '/')
        {
            lexer->current_char = fgetc(lexer->file);
        }
    }
}

static void skip_comments(Lexer *lexer)
{
    if (lexer->current_char == '/')
    {
        lexer->current_char = fgetc(lexer->file);
        if (lexer->current_char == '/') // Single line comment like this one
        {
            skip_line(lexer);
        }
        else if (lexer->current_char == '*')
        {
            skip_multiline_comment(lexer);
        }
    }
    else
    {
        return;
    }

    skip_comments(lexer);
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

static Token *get_next_token(Lexer *lexer)
{

    skip_spaces(lexer);
    Token *tok = NULL;


    if (isdigit(lexer->current_char))
    {
        char number[MAX_TOK_LEN];
        size_t i = 0;
        while (lexer->current_char != EOF && (isdigit(lexer->current_char) || lexer->current_char == '.'))
        {
            if (i >= MAX_TOK_LEN - 1) break;
            number[i++] = (char) lexer->current_char;
            next_char(lexer);
        }
        number[i] = 0;
        if (lexer->current_char == EOF)
        {
            tok = create_tok(TokEOF, "EOF");
            goto END;
        }
        else
        {
            ungetc(lexer->current_char, lexer->file);
        }

        //TODO more refined.
        tok = strchr(number, '.') ? create_tok(TokDoubleLiteral, number) : create_tok(TokIntLiteral, number);
        goto END;
    }
    else if (isalpha(lexer->current_char) || lexer->current_char == '_')
    {
        char buffer[MAX_TOK_LEN];
        size_t i = 0;
        while (lexer->current_char != EOF && (isalnum(lexer->current_char) || lexer->current_char == '_'))
        {
            if (i >= MAX_TOK_LEN - 1) break;
            buffer[i++] = (char) lexer->current_char;
            next_char(lexer);
        }
        buffer[i] = 0;
        if (lexer->current_char == EOF)
        {
            tok = create_tok(TokEOF, "EOF");
            goto END;
        }
        else
        {
            ungetc(lexer->current_char, lexer->file);
        }

        for (size_t j = 0; j < sizeof KEYWORDS / sizeof KEYWORDS[0]; ++j)
        {
            if (strcmp(buffer, KEYWORDS[j].keyword) == 0)
            {
                tok = create_tok(KEYWORDS[j].type, KEYWORDS[j].keyword);
                goto END;
            }
        }

        tok = create_tok(TokIdentifier, buffer);
        goto END;
    }

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
            else if (lexer->current_char == '>')
            {
                tok = create_tok(TokArrow, "->");
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
        case '%':
            next_char(lexer);
            tok = match_compound_operator(lexer, '%', TokPercentAssign, TokPercent);
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
        case '?':
            tok = create_tok(TokQuestionMark, "?");
            break;
        case ':':
            tok = create_tok(TokColon, ",");
            break;
        case ',':
            tok = create_tok(TokComma, ",");
            break;
        case '.':
            tok = create_tok(TokDot, ".");
            break;
        case ';':
            tok = create_tok(TokSemicolon, ";");
            break;
        case '(':
            tok = create_tok(TokLParen, "(");
            break;
        case ')':
            tok = create_tok(TokRParen, ")");
            break;
        case '{':
            tok = create_tok(TokLSquirly, "{");
            break;
        case '}':
            tok = create_tok(TokRSquirly, "}");
            break;
        case '[':
            tok = create_tok(TokLBracket, "[");
            break;
        case ']':
            tok = create_tok(TokRBracket, "]");
            break;
        case '"':
        {
            char string_literal[MAX_TOK_LEN] = {0};
            size_t i = 0;
            next_char(lexer);
            while (lexer->current_char != EOF && lexer->current_char != '"')
            {
                if (i >= MAX_TOK_LEN - 1) break;
                string_literal[i++] = (char) lexer->current_char;
                next_char(lexer);
            }
            string_literal[i] = 0;

            if (lexer->current_char == EOF)
            {
                tok = create_tok(TokEOF, "EOF");
                break;
            }
            if (lexer->current_char == '"' && i <= MAX_TOK_LEN - 1)
            {
                tok = create_tok(TokStringLiteral, string_literal);
            }

            break;
        }
        case '\'':
        {
            char char_literal[MAX_TOK_LEN] = {0};
            size_t i = 0;

            next_char(lexer);

            while (lexer->current_char != EOF && lexer->current_char != '\'')
            {
                if (i >= MAX_TOK_LEN - 1)
                {
                    break;
                }
                if (lexer->current_char == '\\')
                {
                    next_char(lexer);
                    switch (lexer->current_char)
                    {
                        case 'n':
                            char_literal[i++] = '\n';
                            break;
                        case 't':
                            char_literal[i++] = '\t';
                            break;
                        case '\\':
                            char_literal[i++] = '\\';
                            break;
                        case '\'':
                            char_literal[i++] = '\'';
                            break;
                        case '0':
                            char_literal[i++] = '\0';
                            break;
                        default:
                            char_literal[i] = 0;
                            tok = create_tok(TokIllegal, char_literal);
                            goto CHAR_END;
                    }
                }
                else
                {
                    char_literal[i++] = (char) lexer->current_char;
                }

                next_char(lexer);
            }

            if (lexer->current_char == '\'')
            {
                char_literal[i] = 0;
                tok = create_tok(TokCharLiteral, char_literal);
            }
            else
            {
//                fprintf(stderr, "Character literal not closed properly.\n");
                tok = create_tok(TokIllegal, char_literal);
            }
            CHAR_END:
            break;
        }

        case EOF:
            tok = create_tok(TokEOF, "EOF");
            break;
    }

    END:
    next_char(lexer);

    return tok;
}

int cleanup_lexer(Lexer *lexer)
{
    if (fclose(lexer->file) != 0)
    {
        fprintf(stderr, "Couldn't close file!");
        return -1;
    }

    free(lexer->next_tok);
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

    lexer->current_char = next_char(lexer);

    lexer->current_tok = NULL;
    lexer->next_tok = get_next_token(lexer);

    return lexer;
}

Token *peek_tok(Lexer *lexer)
{
    return lexer->next_tok;
}

Token *consume_tok(Lexer *lexer)
{
    lexer->current_tok = lexer->next_tok;
    lexer->next_tok = get_next_token(lexer);

    return lexer->current_tok;
}

