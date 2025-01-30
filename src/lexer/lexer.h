//
// Created by escha on 30.01.25.
//

#pragma once


typedef enum
{
    /*--------BASIC TOKENS--------*/
    TokIllegal,
    TokEOF,

    /*--------OPERATORS: ARITHMETIC--------*/
    TokAssign,
    TokPlus,
    TokMinus,
    TokStar, //Also Pointer
    TokSlash,

    /*--------OPERATORS: ARITHMETIC ASSIGNMENT--------*/
    TokIncrement,
    TokDecrement,
    TokPlusAssign,
    TokMinusAssign,
    TokStarAssign,
    TokSlashAssign,

    /*--------OPERATORS: COMPARISON--------*/
    TokLess,
    TokLT,
    TokGT,
    TokGreater,
    TokEqual,
    TokNotEqual,

    /*--------OPERATORS: LOGICAL--------*/
    TokNegation,
    TokOrOr,
    TokAndAnd,

    /*--------OPERATORS: BITWISE--------*/
    TokOr,
    TokAnd, //Also Address
    TokBitwiseXor,
    TokBitwiseNot,
    TokBitwiseSL,
    TokBitwiseSR,

    /*--------OPERATORS: BITWISE ASSIGNMENT--------*/
    TokBitwiseOrAssign,
    TokBitwiseAndAssign,
    TokBitwiseXorAssign,
    TokBitwiseNotAssign,
    TokBitwiseSLAssign,
    TokBitwiseSRAssign,

    /*--------OPERATORS: CONDITIONAL--------*/
    TokQuestionMark,
    TokColon,

    /*--------DELIMITERS--------*/
    TokComma,
    TokDot,
    TokArrow,
    TokSemicolon,
    TokLParen,
    TokRParen,
    TokLSquirly,
    TokRSquirly,
    TokLBracket,
    TokRBracket,

    /*--------LITERALS--------*/
    TokCharLiteral,
    TokIntLiteral,
    TokFloatLiteral,
    TokDoubleLiteral,
    TokUnsignedIntLiteral,
    TokUnsignedLongLiteral,
    TokUnsignedLongLongLiteral,
    TokStringLiteral,

    /*--------IDENTIFIER--------*/
    TokIdentifier,

    /*--------KEYWORDS: DATA TYPES--------*/
    TokChar,
    TokShort,
    TokInt,
    TokLong,
    TokFloat,
    TokDouble,
    TokVoid,
    TokUnsigned,

    /*--------KEYWORDS: COMPLEX DATA TYPES--------*/
    TokStruct,
    TokUnion,
    TokEnum,
    TokTypedef,

    /*--------KEYWORDS: STORAGE CLASS SPECIFIER--------*/
    TokConst,
    TokVolatile,
    TokRestrict,
    TokAuto,
    TokRegister,
    TokStatic,
    TokExtern,
    TokInline,

    /*--------KEYWORDS: CONTROL FLOW--------*/
    TokIf,
    TokElse,
    TokFor,
    TokWhile,
    TokDo,
    TokSwitch,
    TokCase,
    TokDefault,
    TokReturn,
    TokBreak,
    TokContinue,
    TokGoto,

    /*--------KEYWORDS: OTHER--------*/
    TokSizeof,
} TokenType;

enum
{
    MAX_TOK_LEN = 64
};

typedef struct
{
    TokenType type;
    char literal[MAX_TOK_LEN];
} Token;

typedef struct Lexer Lexer;

Lexer* create_lexer(const char *filename);
int cleanup_lexer(Lexer *lexer);

Token* next_tok(Lexer *lexer);
