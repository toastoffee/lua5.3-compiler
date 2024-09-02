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

std::vector<Statement*> BlockParser::ParseStatements(Lexer *lexer) {
    std::vector<Statement*> stats;
    while(!isReturnOrBlockEnd(lexer->LookAhead())) {
        auto statement = ParseStatement(lexer);
        if (!isInstanceOf<EmptyStatement>(statement)) {
            stats.push_back(statement);
        }
    }
    return stats;
}

std::vector<Expression*> BlockParser::ParseRetExpressions(Lexer *lexer) {
    std::vector<Expression*> ret;

    return ret;
}

bool BlockParser::isReturnOrBlockEnd(const Token& token) {
    if(token.id == TokenId::TOKEN_KW_RETURN
       | token.id == TokenId::TOKEN_EOF
       | token.id == TokenId::TOKEN_KW_END
       | token.id == TokenId::TOKEN_KW_ELSE
       | token.id == TokenId::TOKEN_KW_ELSEIF
       | token.id == TokenId::TOKEN_KW_UNTIL) {
        return true;
    } else {
        return false;
    }
}

Statement* BlockParser::ParseStatement(Lexer *lexer) {
    auto stat = new Statement();


    return stat;
}
