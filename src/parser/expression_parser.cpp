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
#include <parser/lua_number.hpp>

// explist ::= exp {‘,’ exp}
std::vector<Expression *> Parser::parseExpressionList(Lexer *lexer) {
    std::vector<Expression *> exps;

    exps.push_back(parseExpression(lexer));
    while(lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();
        exps.push_back(parseExpression(lexer));
    }
    return exps;
}

std::vector<Expression *> Parser::parseRetExpressions(Lexer *lexer) {
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
        ret = parseExpressionList(lexer);
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
Expression *Parser::parseExpression(Lexer *lexer) {
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
    UnopExpression* exp;
    switch (lexer->LookAhead().id) {
        case TokenId::TOKEN_OP_UNM:
        case TokenId::TOKEN_OP_BNOT:
        case TokenId::TOKEN_OP_LEN:
        case TokenId::TOKEN_OP_NOT:
            op = lexer->NextToken();
            exp = new UnopExpression(op.line, op.id, parseExpression_2(lexer));
            return exp;
        default:
            break;
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
    Token token;
    switch (lexer->LookAhead().id) {
        case TokenId::TOKEN_VARARG:         // ...
            return new VarargExpression(lexer->NextToken().line);
        case TokenId::TOKEN_KW_NIL:         // nil
            return new NilExpression(lexer->NextToken().line);
        case TokenId::TOKEN_KW_TRUE:        // true
            return new TrueExpression(lexer->NextToken().line);
        case TokenId::TOKEN_KW_FALSE:       // false
            return new FalseExpression(lexer->NextToken().line);
        case TokenId::TOKEN_STRING:         // literalString
            token = lexer->NextToken();
            return new StringExpression(token.line, token.tokenStr);
        case TokenId::TOKEN_NUMBER:         // numeral
            return parseNumberExpression(lexer);
        case TokenId::TOKEN_SEP_LCURLY:     // tableConstructor
            return parseTableConstructorExpression(lexer);
        case TokenId::TOKEN_KW_FUNCTION:    // functionDef
            lexer->NextToken();
            return parseFuncDefExpression(lexer);
        default:    // prefixExp
            return parsePrefixExp(lexer);
    }
}

Expression *Parser::parseNumberExpression(Lexer *lexer) {
    Token token = lexer->NextToken();
    auto i = LuaNumber::ParseInteger(token.tokenStr);
    if(i != nullptr) {
        return new IntegerExpression(token.line, *i);
    }
    auto d = LuaNumber::ParseFloat(token.tokenStr);
    if(d != nullptr) {
        return new FloatExpression(token.line, *d);
    }
    throw std::runtime_error("not a number:" + token.tokenStr);
}

// tableconstructor ::= ‘{’ [fieldlist] ‘}’
Expression *Parser::parseTableConstructorExpression(Lexer *lexer) {
    int line = lexer->GetLine();
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LCURLY);    // {
    auto pair = parseFieldList(lexer);             // [fieldList]
    auto keyExps = pair.first;
    auto valExps = pair.second;
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_RCURLY);    // }
    auto lastLine = lexer->GetLine();
    return new TableConstructorExpression(line, lastLine, keyExps, valExps);
}

// fieldlist ::= field {fieldsep field} [fieldsep]
std::pair<std::vector<Expression *>, std::vector<Expression *>> Parser::parseFieldList(Lexer *lexer) {
    std::vector<Expression *> ks, vs;
    if(lexer->LookAhead().id != TokenId::TOKEN_SEP_RCURLY) {
        auto pair = parseField(lexer);                          // field
        ks.push_back(pair.first);
        vs.push_back(pair.second);

        while(isFieldSep(lexer->LookAhead().id)) {              // {
            lexer->NextToken();                                         // fieldSep
            if(lexer->LookAhead().id != TokenId::TOKEN_SEP_RCURLY) {
                pair = parseField(lexer);                               // field
                ks.push_back(pair.first);
                vs.push_back(pair.second);
            } else {
                break;                                                  // [fieldSep]
            }

        }
    }

    return std::make_pair(ks, vs);
}

// field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
std::pair<Expression *, Expression *> Parser::parseField(Lexer *lexer) {
    Expression *k, *v;
    if(lexer->LookAhead().id == TokenId::TOKEN_SEP_LBRACK) {
        lexer->NextToken();                                     // '['
        k = parseExpression(lexer);                             // exp
        lexer->NextTokenOfId(TokenId::TOKEN_SEP_RBRACK);    // ']'
        lexer->NextTokenOfId(TokenId::TOKEN_OP_ASSIGN);     // =
        v = parseExpression(lexer);                             // exp
        return std::make_pair(k, v);
    }

    auto exp = parseExpression(lexer);
    if(isInstanceOf<NameExpression>(exp)) {
        if(lexer->LookAhead().id == TokenId::TOKEN_OP_ASSIGN) {
            // Name ‘=’ exp => ‘[’ LiteralString ‘]’ = exp
            lexer->NextToken();
            auto nameExp = dynamic_cast<NameExpression *>(exp);
            k = new StringExpression(nameExp->line, nameExp->name);
            v = parseExpression(lexer);
            return std::make_pair(k, v);
        }
    }

    return std::make_pair(nullptr, exp);
}

bool Parser::isFieldSep(TokenId tokenId) {
    return tokenId == TokenId::TOKEN_SEP_COMMA || tokenId == TokenId::TOKEN_SEP_SEMI;
}

// functiondef ::= function funcbody
// funcbody ::= ‘(’ [parlist] ‘)’ block end
Expression *Parser::parseFuncDefExpression(Lexer *lexer) {
    int line = lexer->GetLine();                            // function
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LPAREN);    // (
    auto pair = parseParList(lexer);       // [parList]
    auto parList = pair.first;
    auto isVararg = pair.second;
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_RPAREN);    // )
    Block *block = parseBlock(lexer);                       // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);        // end
    int lastLine = lexer->GetLine();

    auto fdExp = new FuncDefExpression;
    fdExp->line = line;
    fdExp->lastLine = lastLine;
    fdExp->isVararg = isVararg;
    fdExp->parList = parList;
    fdExp->block = block;

    return fdExp;
}

std::pair<std::vector<std::string>, bool> Parser::parseParList(Lexer *lexer) {
    std::vector<std::string> names;
    bool isVararg = false;
    switch (lexer->LookAhead().id) {
        case TokenId::TOKEN_SEP_RPAREN:
            return std::make_pair(names, false);
        case TokenId::TOKEN_VARARG:
            lexer->NextToken();
            return std::make_pair(names, true);
        default:
            break;
    }
    Token token = lexer->NextIdentifier();
    names.push_back(token.tokenStr);

    while(lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();
        if(lexer->LookAhead().id == TokenId::TOKEN_IDENTIFIER) {
            names.push_back(lexer->NextIdentifier().tokenStr);
        } else {
            lexer->NextTokenOfId(TokenId::TOKEN_VARARG);
            isVararg = true;
            break;
        }
    }

    return std::make_pair(names, isVararg);
}

// prefixexp ::= Name
//      | ‘(’ exp ‘)’
//      | prefixexp ‘[’ exp ‘]’
//      | prefixexp ‘.’ Name
//      | prefixexp [‘:’ Name] args
Expression *Parser::parsePrefixExp(Lexer *lexer) {
    Expression *exp;
    if(lexer->LookAhead().id == TokenId::TOKEN_IDENTIFIER) {
        Token token = lexer->NextIdentifier();      // name
        exp = new NameExpression(token.line, token.tokenStr);
    } else {
        exp = parseParensExpression(lexer);
    }
    return finishPrefixExpression(lexer, exp);
}

Expression *Parser::parseParensExpression(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LPAREN);    // '('
    auto exp = parseExpression(lexer);          // exp
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_RPAREN);    // ')'

    if(isInstanceOf<VarargExpression>(exp)
            || isInstanceOf<FuncCallExpression>(exp)
            || isInstanceOf<NameExpression>(exp)
            || isInstanceOf<TableAccessExpression>(exp)) {
        return new ParensExpression(exp);
    }

    // no need to keep parens
    return exp;
}

Expression *Parser::finishPrefixExpression(Lexer *lexer, Expression *exp) {
    Expression *keyExp;
    Token token;
    while(true) {
        TokenId t = lexer->LookAhead().id;
        switch (t) {
            case TokenId::TOKEN_SEP_LBRACK:
                lexer->NextToken();                                     // '['
                keyExp = parseExpression(lexer);                        // exp
                lexer->NextTokenOfId(TokenId::TOKEN_SEP_RBRACK);    // ']'
                exp = new TableAccessExpression(lexer->GetLine(), exp, keyExp);
                break;
            case TokenId::TOKEN_SEP_DOT:
                lexer->NextToken();                     // '.'
                token = lexer->NextIdentifier();        // name
                keyExp = new StringExpression(token.line, token.tokenStr);
                exp = new TableAccessExpression(token.line, exp, keyExp);
                break;
            case TokenId::TOKEN_SEP_COLON:
            case TokenId::TOKEN_SEP_LPAREN:
            case TokenId::TOKEN_SEP_LCURLY:
            case TokenId::TOKEN_STRING:
                exp = finishFuncCallExpression(lexer, exp);     // [':' name] args
                break;
            default:
                return exp;
        }
        return exp;
    }
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

Expression *Parser::finishFuncCallExpression(Lexer *lexer, Expression *exp) {
    auto fcExp = new FuncCallExpression;
    fcExp->prefixExp = exp;
    fcExp->nameExp = parseNameExpression(lexer);    // [':' name]
    fcExp->line = lexer->GetLine();                 //
    fcExp->args = parseArgs(lexer);                 // args
    fcExp->lastLine = lexer->GetLine();             //
    return fcExp;
}

StringExpression *Parser::parseNameExpression(Lexer *lexer) {
    if(lexer->LookAhead().id == TokenId::TOKEN_SEP_COLON) {
        lexer->NextToken();
        Token token = lexer->NextIdentifier();
        return new StringExpression(token.line, token.tokenStr);
    }
    return nullptr;
}

std::vector<Expression *> Parser::parseArgs(Lexer *lexer) {
    std::vector<Expression *> args;
    switch (lexer->LookAhead().id) {
        case TokenId::TOKEN_SEP_LPAREN: // '(' [expList] ')'
            lexer->NextToken();
            if(lexer->LookAhead().id != TokenId::TOKEN_SEP_RPAREN) {
                args = parseExpressionList(lexer);
            }
            lexer->NextTokenOfId(TokenId::TOKEN_SEP_RPAREN);
            return args;
        case TokenId::TOKEN_SEP_LCURLY: // '{' [fieldList] '}'
            return std::vector<Expression *>{ parseTableConstructorExpression(lexer) };
        default:                        // literal string
            auto token = lexer->NextTokenOfId(TokenId::TOKEN_STRING);
            return std::vector<Expression *>{ new StringExpression(token.line, token.tokenStr) };
    }
}
