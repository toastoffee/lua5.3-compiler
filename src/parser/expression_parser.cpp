/**
  ******************************************************************************
  * @file           : expression_parser.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/3
  ******************************************************************************
  */

#include "parser.hpp"

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

Expression *Parser::ParseExpression(Lexer *lexer) {
    return nullptr;
}

Expression *Parser::parseFuncDefExpression(Lexer *lexer) {
    return nullptr;
}

Expression *Parser::parsePrefixExp(Lexer *lexer) {
    return nullptr;
}

// varlist ::= var {‘,’ var}
std::vector<Expression *> Parser::parseVarList(Lexer *lexer, Expression *var0) {
    std::vector<Expression *> vars;

    vars.push_back(checkVar(lexer, var0));                  // var
    while(lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();                                      // ,
        Expression *exp = parsePrefixExp(lexer);                 // var
        vars.push_back(checkVar(lexer, exp));
    }
    return vars;
}

// var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name
Expression *Parser::checkVar(Lexer *lexer, Expression *exp) {
    if(isInstanceOf<NameExpression>(exp) || isInstanceOf<TableAccessExpression>(exp)) {
        return exp;
    }
    // throw error
    throw std::runtime_error("unreachable!");
}

// funcname ::= Name {‘.’ Name} [‘:’ Name]
std::pair<Expression *, bool> Parser::parseFuncName(Lexer *lexer) {
    auto token = lexer->NextIdentifier();
    Expression *exp = new NameExpression(token.line, token.tokenStr);
    bool hasColon = false;

    while(lexer->LookAhead().id == TokenId::TOKEN_SEP_DOT) {
        lexer->NextToken();
        token = lexer->NextIdentifier();

        auto idx = new StringExpression;
        idx->line = token.line;
        idx->str = token.tokenStr;

        exp = new TableAccessExpression(token.line, exp, idx);
    }

    if(lexer->LookAhead().id == TokenId::TOKEN_SEP_COLON) {
        lexer->NextToken();
        token = lexer->NextIdentifier();
        auto idx = new StringExpression;
        idx->line = token.line;
        idx->str = token.tokenStr;

        exp = new TableAccessExpression(token.line, exp, idx);
        hasColon = true;
    }

    return std::make_pair(exp, hasColon);
}