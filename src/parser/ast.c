//
// Created by escha on 30.01.25.
//

#include "ast.h"

#include <stdlib.h>

typedef struct
{
    Lexer *lexer;
    Token cur;
    Token peek;
} Parser;

typedef enum
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
    ASTNodeFuncDecl,
    ASTNodeStructDecl,
    ASTNodeEnumDecl,
    ASTNodeUnionDecl,
    ASTNodeArrayDecl,
    ASTNodeCast,
    ASTNodeParen,
    ASTNodeSquirly,
    ASTNodeSizeof,
} ASTNodeType;


struct ASTNode
{
    ASTNodeType type;
    union
    {
        struct
        {
            TokenType type;
            char val[MAX_TOK_LEN];
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

static const ASTNode AST_NODE_ERROR_DUMMY = {0};


static ASTNode next_node(Lexer *lexer)
{
    Token *tok = next_tok(lexer);
    if (tok == NULL) return AST_NODE_ERROR_DUMMY;

    ASTNode node = {0};

    switch (tok->type)
    {
        case TokAssign:
            node.type = ASTNodeAssign;
            break;
        case TokIllegal:
            break;
        case TokEOF:
            break;
        case TokPlus:
            break;
        case TokMinus:
            break;
        case TokStar:
            break;
        case TokSlash:
            break;
        case TokIncrement:
            break;
        case TokDecrement:
            break;
        case TokPlusAssign:
            break;
        case TokMinusAssign:
            break;
        case TokStarAssign:
            break;
        case TokSlashAssign:
            break;
        case TokLess:
            break;
        case TokLT:
            break;
        case TokGT:
            break;
        case TokGreater:
            break;
        case TokEqual:
            break;
        case TokNotEqual:
            break;
        case TokNegation:
            break;
        case TokOrOr:
            break;
        case TokAndAnd:
            break;
        case TokOr:
            break;
        case TokAnd:
            break;
        case TokBitwiseXor:
            break;
        case TokBitwiseNot:
            break;
        case TokBitwiseSL:
            break;
        case TokBitwiseSR:
            break;
        case TokBitwiseOrAssign:
            break;
        case TokBitwiseAndAssign:
            break;
        case TokBitwiseXorAssign:
            break;
        case TokBitwiseNotAssign:
            break;
        case TokBitwiseSLAssign:
            break;
        case TokBitwiseSRAssign:
            break;
        case TokQuestionMark:
            break;
        case TokColon:
            break;
        case TokComma:
            break;
        case TokDot:
            break;
        case TokArrow:
            break;
        case TokSemicolon:
            break;
        case TokLParen:
            break;
        case TokRParen:
            break;
        case TokLSquirly:
            break;
        case TokRSquirly:
            break;
        case TokLBracket:
            break;
        case TokRBracket:
            break;
        case TokCharLiteral:
            break;
        case TokIntLiteral:
            break;
        case TokFloatLiteral:
            break;
        case TokDoubleLiteral:
            break;
        case TokUnsignedIntLiteral:
            break;
        case TokUnsignedLongLiteral:
            break;
        case TokUnsignedLongLongLiteral:
            break;
        case TokStringLiteral:
            break;
        case TokIdentifier:
            break;
        case TokChar:
            break;
        case TokShort:
            break;
        case TokInt:
            break;
        case TokLong:
            break;
        case TokFloat:
            break;
        case TokDouble:
            break;
        case TokVoid:
            break;
        case TokUnsigned:
            break;
        case TokStruct:
            break;
        case TokUnion:
            break;
        case TokEnum:
            break;
        case TokTypedef:
            break;
        case TokConst:
            break;
        case TokVolatile:
            break;
        case TokRestrict:
            break;
        case TokAuto:
            break;
        case TokRegister:
            break;
        case TokStatic:
            break;
        case TokExtern:
            break;
        case TokInline:
            break;
        case TokIf:
            break;
        case TokElse:
            break;
        case TokFor:
            break;
        case TokWhile:
            break;
        case TokDo:
            break;
        case TokSwitch:
            break;
        case TokCase:
            break;
        case TokDefault:
            break;
        case TokReturn:
            break;
        case TokBreak:
            break;
        case TokContinue:
            break;
        case TokGoto:
            break;
        case TokSizeof:
            break;
    }

    return node;
}


ASTNode *build_AST(Lexer *lexer)
{
    ASTNode node;
    while((node = next_node(lexer)).type != ASTNodeEOF)
    {

    }
}
