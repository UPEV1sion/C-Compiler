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
    TokQuestionColon,

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
    TokBooleanLiteral,
    TokNullLiteral,

    /*--------IDENTIFIER--------*/
    TokIdentifier,

    /*--------COMMENTS--------*/
    TokSingleLineComment,
    TokMultiLineComment,

    /*--------KEYWORDS: DATA TYPES--------*/
    TokChar,
    TokShort,
    TokInt,
    TokLong,
    TokLongLong,
    TokFloat,
    TokDouble,
    TokLongDouble,
    TokBool,
    TokVoid,
    TokUnsigned,
    TokUChar,
    TokUShort,
    TokUInt,
    TokULong,
    TokULongLong,

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

typedef struct
{
    TokenType type;
    const char *literal;
} Token;

typedef struct Lexer Lexer;

Lexer* create_lexer(const char *filename);
int cleanup_lexer(Lexer *lexer);

Token* next_tok(Lexer *lexer);
