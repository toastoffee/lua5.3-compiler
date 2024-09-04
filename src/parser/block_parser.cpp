/**
  ******************************************************************************
  * @file           : block_parser.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#include "parser.hpp"

//! block ::= {stat} [retStat]
Block *Parser::ParseBlock(Lexer *lexer) {
    auto ret = new Block;
    ret->statements = ParseStatements(lexer);
    ret->expressions = ParseRetExpressions(lexer);
    ret->lastLine = lexer->GetLine();
    return ret;
}

bool Parser::isReturnOrBlockEnd(const Token& token) {
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









