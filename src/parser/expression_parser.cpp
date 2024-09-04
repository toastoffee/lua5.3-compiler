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

/*
exp ::=  nil | false | true | Numeral | LiteralString | ‘...’ | functiondef |
     prefixexp | tableconstructor | exp binop exp | unop exp
*/
/*
exp   ::= exp12
exp12 ::= exp11 {or exp11}
exp11 ::= exp10 {and exp10}
exp10 ::= exp9 {(‘<’ | ‘>’ | ‘<=’ | ‘>=’ | ‘~=’ | ‘==’) exp9}
exp9  ::= exp8 {‘|’ exp8}
exp8  ::= exp7 {‘~’ exp7}
exp7  ::= exp6 {‘&’ exp6}
exp6  ::= exp5 {(‘<<’ | ‘>>’) exp5}
exp5  ::= exp4 {‘..’ exp4}
exp4  ::= exp3 {(‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘%’) exp3}
exp2  ::= {(‘not’ | ‘#’ | ‘-’ | ‘~’)} exp1
exp1  ::= exp0 {‘^’ exp2}
exp0  ::= nil | false | true | Numeral | LiteralString
        | ‘...’ | functiondef | prefixexp | tableconstructor
*/
Expression *Parser::ParseExpression(Lexer *lexer) {
    return parseExpression_12(lexer);
}

Expression *Parser::parseExpression_12(Lexer *lexer) { //exp12 ::= exp11 {or exp11}
    Expression* exp = parseExpression_11(lexer);
    while(lexer->LookAhead().id == TokenId::TOKEN_OP_OR) {
        Token op = lexer->NextToken();
        exp = new BinopExpression(op.line, op.id, exp, parseExpression_10(lexer));
    }
    return exp;
}

Expression *Parser::parseExpression_11(Lexer *lexer) { // exp11 ::= exp10 {and exp10}
    Expression *exp = parseExpression_10(lexer);
    while(lexer->LookAhead().id == TokenId::TOKEN_OP_AND) {
        Token op = lexer->NextToken();
        exp = new BinopExpression(op.line, op.id, exp, parseExpression_10(lexer));
    }
    return exp;
}

Expression *Parser::parseExpression_10(Lexer *lexer) { // compare
    Expression *exp = parseExpression_9(lexer);
    Token op;
    while (true) {
        switch (lexer->LookAhead().id) {
            case TokenId::TOKEN_OP_LT:
            case TokenId::TOKEN_OP_GT:
            case TokenId::TOKEN_OP_NE:
            case TokenId::TOKEN_OP_LE:
            case TokenId::TOKEN_OP_GE:
            case TokenId::TOKEN_OP_EQ:
                op = lexer->NextToken();
                exp = new BinopExpression(op.line, op.id, exp, parseExpression_9(lexer));
                break;
            default:
                return exp;
        }
    }
}

Expression *Parser::parseExpression_9(Lexer *lexer) {  // x | y
    Expression *exp = parseExpression_8(lexer);
    while(lexer->LookAhead().id == TokenId::TOKEN_OP_BOR) {
        Token op = lexer->NextToken();
        exp = new BinopExpression(op.line, op.id, exp, parseExpression_8(lexer));
    }
    return exp;
}

Expression *Parser::parseExpression_8(Lexer *lexer) { // x ~ y
    Expression *exp = parseExpression_7(lexer);
    while(lexer->LookAhead().id == TokenId::TOKEN_OP_WAVE) {
        Token op = lexer->NextToken();
        exp = new BinopExpression(op.line, op.id, exp, parseExpression_7(lexer));
    }
    return exp;
}

Expression *Parser::parseExpression_7(Lexer *lexer) {  // x & y
    Expression *exp = parseExpression_6(lexer);
    while(lexer->LookAhead().id == TokenId::TOKEN_OP_BAND) {
        Token op = lexer->NextToken();
        exp = new BinopExpression(op.line, op.id, exp, parseExpression_6(lexer));
    }
    return exp;
}

Expression *Parser::parseExpression_6(Lexer *lexer) { // shift
    Expression *exp = parseExpression_5(lexer);
    Token op;
    while(true) {
        switch (lexer->LookAhead().id) {
            case TokenId::TOKEN_OP_SHL:
            case TokenId::TOKEN_OP_SHR:
                op = lexer->NextToken();
                exp = new BinopExpression(op.line, op.id, exp, parseExpression_5(lexer));
                break;
            default:
                return exp;
        }
    }
}

Expression *Parser::parseExpression_5(Lexer *lexer) { // exp4 {`..` exp4}
    Expression *exp = parseExpression_4(lexer);
    if(lexer->LookAhead().id != TokenId::TOKEN_OP_CONCAT) {
        return exp;
    }

    std::vector<Expression *> exps;
    exps.push_back(exp);
    int line = 0;
    while(lexer->LookAhead().id == TokenId::TOKEN_OP_CONCAT) {
        line = lexer->NextToken().line;
        exps.push_back(parseExpression_4(lexer));
    }
    return new ConcatExpression(line, exps);
}

Expression *Parser::parseExpression_4(Lexer *lexer) { // // x +/- y
    Expression *exp = parseExpression_3(lexer);
    Token op;
    while (true) {
        switch (lexer->LookAhead().id) {
            case TokenId::TOKEN_OP_ADD:
            case TokenId::TOKEN_OP_MINUS:
                op = lexer->NextToken();
                exp = new BinopExpression(op.line, op.id, exp, parseExpression_3(lexer));
                break;
            default:
                return exp;
        }
    }
}

Expression *Parser::parseExpression_3(Lexer *lexer) { // *, %, /, //
    Expression *exp = parseExpression_2(lexer);
    Token op;
    while(true) {
        switch (lexer->LookAhead().id) {
            case TokenId::TOKEN_OP_MUL:
            case TokenId::TOKEN_OP_MOD:
            case TokenId::TOKEN_OP_DIV:
            case TokenId::TOKEN_OP_IDIV:
                op = lexer->NextToken();
                exp = new BinopExpression(op.line, op.id, exp, parseExpression_2(lexer));
                break;
            default:
                return exp;
        }
    }
}

Expression *Parser::parseExpression_2(Lexer *lexer) { // {(`not` | `#` | `-` | `~`)} exp1
    Token op;
    switch (lexer->LookAhead().id) {
        case TokenId::TOKEN_OP_UNM:
        case TokenId::TOKEN_OP_BNOT:
        case TokenId::TOKEN_OP_LEN:
        case TokenId::TOKEN_OP_NOT:
            op = lexer->NextToken();
            auto exp = new UnopExpression(op.line, op.id, parseExpression_2(lexer));
            return exp;
    }
    return parseExpression_1(lexer);
}

Expression *Parser::parseExpression_1(Lexer *lexer) { // exp0 {'^' exp2} (right associated)
    auto exp = parseExpression_0(lexer);
    if(lexer->LookAhead().id == TokenId::TOKEN_OP_POW) {
        auto token = lexer->NextToken();
        exp = new BinopExpression(token.line, token.id, exp, parseExpression_2(lexer));
    }
    return exp;
}

Expression *Parser::parseExpression_0(Lexer *lexer) {
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