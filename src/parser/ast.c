//
// Created by escha on 30.01.25.
//

#include "ast.h"
#include "../helper/helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define create_node(node) do { \
    node = malloc(sizeof(ASTNode)); \
    if(node == NULL) { \
        fprintf(stderr, "Couldn't malloc node"); \
        return NULL; \
    }}while(0)

typedef struct
{
    Lexer *lexer;
//    Token *cur;
//    Token *peek;
    ASTNode *root;
} Parser;


#define is_numeric_literal(tok) ((tok) >= TokCharLiteral && (tok) <= TokUnsignedLongLongLiteral)
#define is_unary_operator(tok) ((tok) == TokMinus || (tok) == TokNegation || (tok) == TokDecrement || (tok) == TokIncrement)

typedef enum //TODO group
{
    ASTNodeIllegal,
    ASTNodeEOF,
    ASTNodeLiteral,
    ASTNodeIdentifier,
    AstNodeUnaryOp,
    ASTNodeBinaryOp,
    ASTNodeTernaryOp,
    ASTNodeIncrement,
    ASTNodeDecrement,
    ASTNodeAssign,
    ASTNodeCall,
    ASTNodeIf,
    ASTNodeWhile,
    ASTNodeDoWhile,
    ASTNodeFor,
    ASTNodeBreak,
    ASTNodeContinue,
    ASTNodeGoto,
    ASTNodeLabel,
    ASTNodeSwitch,
    ASTNodeCase,
    ASTNodeDefault,
    ASTNodeReturn,
    ASTNodeVarDecl,
    ASTNodeCharDecl,
    ASTNodeIntDecl,
    ASTNodeShortDecl,
    ASTNodeLongDecl,
    ASTNodeLongLongDecl,
    ASTNodeULongDecl,
    ASTNodeULongLongDecl,
    ASTNodeFloatDecl,
    ASTNodeDoubleDecl,
    ASTNodeVoidDecl,
    ASTNodeFuncDecl,
    ASTNodeStructDecl,
    ASTNodeEnumDecl,
    ASTNodeUnionDecl,
    ASTNodeArrayDecl,
    ASTNodeCast,
    ASTNodeParen,
    ASTNodeSquirly,
    ASTNodeSizeof,
    ASTNodePointerDecl,
    ASTNodePointerDeref,
    ASTNodeAddressOf,
    ASTNodeUnaryNegation,
    ASTNodeLogicalNegation,
} ASTNodeType;


struct ASTNode
{
    ASTNodeType type;
    union
    {
        struct
        {
            TokenType type;
            union
            {
                char char_val;
                int int_val;
                long long_val;
                long long longlong_val;
                float float_val;
                double double_val;
                long double longdouble_val;
                unsigned int uint_val;
                unsigned long ulong_val;
                unsigned long long ulonglong_val;
                char str_val[MAX_TOK_LEN];
            };
        } literal;

        struct
        {
            char name[MAX_TOK_LEN];
        } identifier;

        struct
        {
            TokenType type;
            ASTNode *op;
        } unary_op;

        struct
        {
            TokenType type;
            ASTNode *left;
            ASTNode *right;
        } binary_op;

        struct
        {
            TokenType type;
            ASTNode *condition;
            ASTNode *true_branch;
            ASTNode *false_branch;
        } ternary_op;

        struct
        {
            ASTNode *left;
            ASTNode *right;
        } assign;

        struct
        {
            char func_name[MAX_TOK_LEN];
            ASTNode *args;
        } function_call;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } struct_decl;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } union_decl;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } enum_decl;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
            ASTNode *body;
        } function_decl;

        struct
        {
            ASTNode *condition;
            ASTNode *body;
            ASTNode *alternate_body;
            ASTNode *next;
        } if_statement;

        struct
        {
            ASTNode *condition;
            ASTNode *body;
        } while_loop;

        struct
        {
            ASTNode *condition;
            ASTNode *body;
        } do_while_loop;

        struct
        {
            ASTNode *condition;
            ASTNode *var;
            ASTNode *step;
            ASTNode *body;
        } for_loop;

        struct
        {
            ASTNode *label;
        } goto_statement;

        struct
        {
            ASTNode *elements;
        } array_decl;

        struct
        {
            ASTNode *var;
            ASTNode *index;
        } array_access;

        struct
        {
            ASTNode *var_type;
            char var_name[MAX_TOK_LEN]; //only the last one
            ASTNode *init_value;
        } var_decl;

        struct
        {
            ASTNode *element;
            ASTNode *cases;
            ASTNode *default_case;
        } switch_statement;

        struct
        {
            ASTNode *expr;
            ASTNode *statement;
            ASTNode *next;
        } case_statement;

        struct
        {
            ASTNode *param;
        } sizeof_keyword;

        struct
        {
            ASTNode *ret;
        } return_keyword;
    };
};

static int parse_numeric_literal(Token *tok, ASTNode *node)
{
    node->type = ASTNodeLiteral;
    node->literal.type = tok->type;

    int ret = -1;

    switch (tok->type)
    {
        case TokCharLiteral:
            node->literal.char_val = *tok->literal;
            ret = 0;
            break;
        case TokIntLiteral:
            ret = atoi_s(tok->literal, &node->literal.int_val);
            break;
        case TokLongLiteral:
            ret = atol_s(tok->literal, &node->literal.long_val);
            break;
        case TokLongLongLiteral:
            ret = atoll_s(tok->literal, &node->literal.longlong_val);
            break;
        case TokFloatLiteral:
            ret = atof_s(tok->literal, &node->literal.float_val);
            break;
        case TokDoubleLiteral:
            ret = atod_s(tok->literal, &node->literal.double_val);
            break;
        case TokLongDoubleLiteral:
            ret = atold_s(tok->literal, &node->literal.longdouble_val);
            break;
        case TokUnsignedIntLiteral:
            ret = atoui_s(tok->literal, &node->literal.uint_val);
            break;
        case TokUnsignedLongLiteral:
            ret = atoul_s(tok->literal, &node->literal.ulong_val);
            break;
        case TokUnsignedLongLongLiteral:
            ret = atoull_s(tok->literal, &node->literal.ulonglong_val);
            break;
        default:
            fprintf(stderr, "Invalid Token!");
            break;
    }

    if (ret < 0)
    {
        fprintf(stderr, "Invalid literal!\n");
    }

    return ret;
}

static ASTNode *parse_primary(Parser *parser)
{
    Token *tok = peek_tok(parser->lexer);
    if (!tok) return NULL;

    ASTNode *node;
    create_node(node);

    if (is_numeric_literal(tok->type))
    {
        consume_tok(parser->lexer);
        if (parse_numeric_literal(tok, node) != 0)
        {
            goto end;
        }

        return node;
    }
    else if (tok->type == TokIdentifier)
    {
        consume_tok(parser->lexer);
        node->type = ASTNodeIdentifier;
        strncpy(node->identifier.name, tok->literal, MAX_TOK_LEN);
        node->identifier.name[MAX_TOK_LEN - 1] = 0;

        return node;
    }
    else
    {
        //TODO
    }

    end:
//    free(node);

    return NULL;
}

static ASTNode* parse_unary(Parser *parser)
{
    Token *tok = peek_tok(parser->lexer);
    if (!tok) return NULL;

    ASTNode *node;
    create_node(node);

    switch (tok->type)
    {
        case TokStar:
            do
            {
                ASTNode *deref;
                create_node(deref);
                deref->type = ASTNodePointerDeref;
                deref->unary_op.op = node;

                node = deref;

                consume_tok(parser->lexer);
            } while(peek_tok(parser->lexer)->type == TokStar); //TODO null check
            break;
        case TokMinus:
            node->type = ASTNodeUnaryNegation;
            break;
        case TokNegation:
            node->type = ASTNodeLogicalNegation;
            break;
        case TokIncrement:
            node->type = ASTNodeIncrement;
            break;
        case TokDecrement:
            node->type = ASTNodeDecrement;
            break;
        case TokAnd:
            node->type = ASTNodeAddressOf;
            break;
        default:
            fprintf(stderr, "Invalid Tok!");
            free(node);
            node = NULL;
            goto end;
    }

    consume_tok(parser->lexer);
    node->unary_op.op = parse_primary(parser);

    end:
    return node;
}

static ASTNode* parse_binary(Parser *parser)
{
    (void) parser;

    return NULL;
}

ASTNode* build_AST(Lexer *lexer)
{
    (void) lexer;
    (void) parse_binary;
    (void) parse_unary;

    return NULL;
}
