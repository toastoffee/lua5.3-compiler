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
Block *Parser::parseBlock(Lexer *lexer) {
    auto ret = new Block;
    ret->statements = parseStatements(lexer);
    ret->retExpressions = parseRetExpressions(lexer);
    ret->lastLine = lexer->GetLine();
    return ret;
}

bool Parser::isReturnOrBlockEnd(const Token& token) {
    if(token.id == TokenId::TOKEN_KW_RETURN
       || token.id == TokenId::TOKEN_EOF
       || token.id == TokenId::TOKEN_KW_END
       || token.id == TokenId::TOKEN_KW_ELSE
       || token.id == TokenId::TOKEN_KW_ELSEIF
       || token.id == TokenId::TOKEN_KW_UNTIL) {
        return true;
    } else {
        return false;
    }
}

Block *Parser::Parse(const std::string& chunk, const std::string& chunkName) {
    auto *lexer = new Lexer(chunk, chunkName);
    auto *block = parseBlock(lexer);
    lexer->NextTokenOfId(TokenId::TOKEN_EOF);
    return block;
}









