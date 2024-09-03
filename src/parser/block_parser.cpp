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

std::vector<Statement *> Parser::ParseStatements(Lexer *lexer) {
    std::vector<Statement*> stats;
    while(!isReturnOrBlockEnd(lexer->LookAhead())) {
        auto statement = ParseStatement(lexer);
        if (!isInstanceOf<EmptyStatement>(statement)) {
            stats.push_back(statement);
        }
    }
    return stats;
}

std::vector<Expression *> Parser::ParseRetExpressions(Lexer *lexer) {
    std::vector<Expression*> ret;

    if(lexer->LookAhead().id != TokenId::TOKEN_KW_RETURN) {
        return ret;
    }

    lexer->NextToken();

    if(lexer->LookAhead().id == TokenId::TOKEN_EOF
       | lexer->LookAhead().id == TokenId::TOKEN_KW_END
       | lexer->LookAhead().id == TokenId::TOKEN_KW_ELSE
       | lexer->LookAhead().id == TokenId::TOKEN_KW_ELSEIF
       | lexer->LookAhead().id == TokenId::TOKEN_KW_UNTIL) {
        return ret;     // empty list
    } else if(lexer->LookAhead().id == TokenId::TOKEN_SEP_SEMI) {
        lexer->NextToken();
        return ret;     // empty list
    } else {
        ret = ParseExpressionList(lexer);
        if(lexer->LookAhead().id == TokenId::TOKEN_SEP_SEMI) {
            lexer->NextToken();
        }
        return ret;
    }
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

Statement* Parser::ParseStatement(Lexer *lexer) {
    auto stat = new Statement();


    return stat;
}

// explist ::= exp {‘,’ exp}
std::vector<Expression *> Parser::ParseExpressionList(Lexer *lexer) {
    std::vector<Expression *> exps;

    exps.push_back(ParseExpression(lexer));
    while(lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();
        exps.push_back(ParseExpression(lexer));
    }
    return exps;
}



