//
// Created by escha on 30.01.25.
//

#pragma once

#include "../lexer/lexer.h"

typedef struct ASTNode ASTNode;

ASTNode* build_AST(Lexer *lexer);
