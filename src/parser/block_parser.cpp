/**
  ******************************************************************************
  * @file           : block_parser.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#include "block_parser.hpp"

//! block ::= {stat} [retStat]
Block *BlockParser::ParseBlock(Lexer *lexer) {
    auto ret = new Block;
    ret->statements = ParseStatements(lexer);
    ret->expressions = ParseRetExpressions(lexer);
    ret->lastLine = lexer->GetLine();
    return ret;
}

std::vector<Statement> BlockParser::ParseStatements(Lexer *lexer) {
    std::vector<Statement> stats;
    while(!isReturnOrBlockEnd(lexer->LookAhead())) {
        auto statement = ParseStatement(lexer);

    }
    return stats;
}

std::vector<Expression> BlockParser::ParseRetExpressions(Lexer *lexer) {
    std::vector<Expression> ret;

    return ret;
}

bool BlockParser::isReturnOrBlockEnd(Token token) {
    return false;
}

Statement BlockParser::ParseStatement(Lexer *lexer) {
    return Statement();
}
