//
// Created by escha on 30.01.25.
//

#include "ast.h"

#include <stdlib.h>

#define new_node(node) do { \
    node = malloc(sizeof(ASTNode)); \
    if(node == NULL) return NULL; \
while(0)

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
            ASTNode *next;
            char name[MAX_TOK_LEN];
        } identifier;

        struct
        {
            TokenType type;
            ASTNode *op;
        } unaryOp;

        struct
        {
            TokenType type;
            ASTNode *left;
            ASTNode *right;
        } binaryOp;

        struct
        {
            TokenType type;
            ASTNode *left;
            ASTNode *middle;
            ASTNode *right;
        } ternaryOp;

        struct
        {
            ASTNode *left;
            ASTNode *right;
        } assign;

        struct
        {
            char func_name[MAX_TOK_LEN];
            ASTNode *args;
        } functionCall;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } structDecl;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } unionDecl;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } enumDecl;

        struct
        {
            char name[MAX_TOK_LEN];
            ASTNode *args;
        } functionDecl;

        struct
        {
            ASTNode *condition;
            ASTNode *body;
            ASTNode *alternate_body;
        } branch;

        struct
        {
            ASTNode *condition;
            ASTNode *body;
        } whileLoop;

        struct
        {
            ASTNode *condition;
            ASTNode *body;
        } doWhileLoop;

        struct
        {
            ASTNode *condition;
            ASTNode *var;
            ASTNode *step;
            ASTNode *body;
        } forLoop;

        struct
        {
            //TODO later
        } breakKeyword;

        struct
        {
            ASTNode *label;
        } gotoStatement;

        struct
        {
            char name[MAX_TOK_LEN];
        } label;

        struct
        {
            //TODO later
        } array;

        struct
        {
            ASTNode *element;
            char var_name[MAX_TOK_LEN]; //only the last one
        } varDecl;

        struct
        {
            ASTNode *element;
            ASTNode *cases;
            ASTNode *default_case;
        } switchStatement;

        struct
        {
            union
            {
                int case_val;
                char case_str[MAX_TOK_LEN];
            };
            ASTNode *expr;
            ASTNode *statement;
            ASTNode *next;
        } caseStatement;

        struct
        {
            char brace;
        } brace;

        struct
        {
            ASTNode *param;
        } sizeofKeyword;

        struct
        {
            ASTNode *val;
        } returnKeyword;
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

static int evaluate(ASTNode *node)
{
    if(node->type == ASTNodeLiteral)
}

ASTNode *build_AST(Lexer *lexer)
{
    ASTNode node;
    while((node = next_node(lexer)).type != ASTNodeEOF)
    {

    }
}
