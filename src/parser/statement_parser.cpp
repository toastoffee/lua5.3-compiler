/**
  ******************************************************************************
  * @file           : statement_parser.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#include "parser.hpp"

/*! Lua statement EBNF
    (mainly classified into 3 classes: control, declaration, assign statement)
    stat ::=  ‘;’
    | break
    | ‘::’ Name ‘::’
    | goto Name
    | do block end
    | while exp do block end
    | repeat block until exp
    | if exp then block {elseif exp then block} [else block] end
    | for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end
    | for namelist in explist do block end
    | function funcname funcbody
    | local function Name funcbody
    | local namelist [‘=’ explist]
    | varlist ‘=’ explist
    | functioncall
*/

std::vector<Statement *> Parser::parseStatements(Lexer *lexer) {
    std::vector<Statement*> stats;
    while(!isReturnOrBlockEnd(lexer->LookAhead())) {
        auto statement = parseStatement(lexer);
        if (!isInstanceOf<EmptyStatement>(statement)) {
            stats.push_back(statement);
        }
    }
    return stats;
}

Statement* Parser::parseStatement(Lexer *lexer) {
    switch(lexer->LookAhead().id) {
        case TokenId::TOKEN_SEP_SEMI:    return parseEmptyStatement(lexer);
        case TokenId::TOKEN_KW_BREAK:    return parseBreakStatement(lexer);
        case TokenId::TOKEN_SEP_LABEL:   return parseLabelStatement(lexer);
        case TokenId::TOKEN_KW_GOTO:     return parseGotoStatement(lexer);
        case TokenId::TOKEN_KW_DO:       return parseDoStatement(lexer);
        case TokenId::TOKEN_KW_WHILE:    return parseWhileStatement(lexer);
        case TokenId::TOKEN_KW_REPEAT:   return parseRepeatStatement(lexer);
        case TokenId::TOKEN_KW_IF:       return parseIfStatement(lexer);
        case TokenId::TOKEN_KW_FOR:      return parseForStatement(lexer);
        case TokenId::TOKEN_KW_FUNCTION: return parseFuncDefStatement(lexer);
        case TokenId::TOKEN_KW_LOCAL:    return parseLocalAssignOrFuncDefStatement(lexer);
        default:                         return parseAssignOrFuncCallStatement(lexer);
    }

}

Statement *Parser::parseEmptyStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_SEMI);
    return new EmptyStatement;
}

// break
Statement *Parser::parseBreakStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_BREAK);

    auto stat = new BreakStatement;
    stat->line = lexer->GetLine();
    return stat;
}

// ‘::’ Name ‘::’
Statement *Parser::parseLabelStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LABEL); // ::
    auto identifier = lexer->NextIdentifier();  // name
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LABEL); // ::

    auto stat = new LabelStatement;
    stat->name = identifier.tokenStr;
    return stat;
}

// goto Name
Statement *Parser::parseGotoStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_GOTO);   // goto
    auto identifier = lexer->NextIdentifier();  // name

    auto stat = new GotoStatement;
    stat->name = identifier.tokenStr;
    return stat;
}

// do block end
Statement *Parser::parseDoStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);     // do
    Block* block = parseBlock(lexer);                   // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end

    auto stat = new DoStatement;
    stat->block = block;
    return stat;
}

// while exp do block end
Statement *Parser::parseWhileStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_WHILE);  // while
    Expression *exp = parseExpression(lexer);           // exp
    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);     // do
    Block *block = parseBlock(lexer);                   // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end

    auto stat = new WhileStatement;
    stat->exp = exp;
    stat->block = block;
    return stat;
}

// repeat block until exp
Statement *Parser::parseRepeatStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_REPEAT); // repeat
    Block *block = parseBlock(lexer);                   // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_UNTIL);  // until
    Expression *exp = parseExpression(lexer);           // exp

    auto stat = new RepeatStatement;
    stat->block = block;
    stat->exp = exp;
    return stat;
}

// if exp then block {elseif exp then block} [else block] end
Statement *Parser::parseIfStatement(Lexer *lexer) {
    std::vector<Expression *> exps;
    std::vector<Block *> blocks;

    lexer->NextTokenOfId(TokenId::TOKEN_KW_IF);     // if
    exps.push_back(parseExpression(lexer));             // exp
    lexer->NextTokenOfId(TokenId::TOKEN_KW_THEN);   // then
    blocks.push_back(parseBlock(lexer));                // block

    while(lexer->LookAhead().id == TokenId::TOKEN_KW_ELSEIF) {
        lexer->NextToken();                                 // elseif
        exps.push_back(parseExpression(lexer));             // exp
        lexer->NextTokenOfId(TokenId::TOKEN_KW_THEN);   // then
        blocks.push_back(parseBlock(lexer));                // block
    }

    // else block => elseif true then block
    if(lexer->LookAhead().id == TokenId::TOKEN_KW_ELSE) {
        lexer->NextToken();                         // else

        auto trueExp = new TrueExpression;          // true
        trueExp->line = lexer->GetLine();
        exps.push_back(trueExp);
        blocks.push_back(parseBlock(lexer));        // block
    }

    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end
    auto stat = new IfStatement;
    stat->exps = exps;
    stat->blocks = blocks;
    return stat;
}

// for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end
// for namelist in explist do block end
Statement *Parser::parseForStatement(Lexer *lexer) {
    int lineOfFor = lexer->NextTokenOfId(TokenId::TOKEN_KW_FOR).line;
    auto name = lexer->NextIdentifier().tokenStr;
    if(lexer->LookAhead().id == TokenId::TOKEN_OP_ASSIGN) {
        return parseForNumStatement(lexer, name, lineOfFor);
    }
    else {
        return parseForInStatement(lexer, name);
    }
}

// for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end
Statement *Parser::parseForNumStatement(Lexer *lexer, std::string varName, int lineOfFor) {
    auto stat = new ForNumStatement;
    stat->lineOfFor = lineOfFor;                            // for
    stat->varName = varName;                                // name

    lexer->NextTokenOfId(TokenId::TOKEN_OP_ASSIGN);     // =
    stat->initExp = parseExpression(lexer);                 // exp
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_COMMA);     // ,
    stat->limitExp = parseExpression(lexer);                // exp

    if(lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();                                 // ,
        stat->stepExp = parseExpression(lexer);             // exp
    } else {
        auto stepExp = new IntegerExpression;
        stepExp->line = lexer->GetLine();
        stepExp->val = 1;
        stat->stepExp = stepExp;
    }

    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);         // do
    stat->lineOfDo = lexer->GetLine();
    stat->block = parseBlock(lexer);                        // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);        // end

    return stat;
}

// for namelist in explist do block end
// namelist ::= Name {‘,’ Name}
// explist ::= exp {‘,’ exp}
Statement *Parser::parseForInStatement(Lexer *lexer, std::string name0) {
    auto stat = new ForInStatement;

    stat->nameList = finishNameList(lexer, name0);      // namelist
    lexer->NextTokenOfId(TokenId::TOKEN_KW_IN);     // in
    stat->exps = parseExpressionList(lexer);            // explist
    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);     // do
    stat->lineOfDo = lexer->GetLine();                  //
    stat->block = parseBlock(lexer);                    // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end

    return stat;
}

std::vector<std::string> Parser::finishNameList(Lexer *lexer, std::string name0) {
    std::vector<std::string> names{name0};

    while (lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();                                     // ,
        names.push_back(lexer->NextIdentifier().tokenStr);      // name
    }
    return names;
}

/*
http://www.lua.org/manual/5.3/manual.html#3.4.11

function f() end          =>  f = function() end
function t.a.b.c.f() end  =>  t.a.b.c.f = function() end
function t.a.b.c:f() end  =>  t.a.b.c.f = function(self) end
local function f() end    =>  local f; f = function() end

The statement `local function f () body end`
translates to `local f; f = function () body end`
not to `local f = function () body end`
(This only makes a difference when the body of the function
 contains references to f.)
*/
// local function Name funcbody
Statement *Parser::parseFuncDefStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_FUNCTION);   // function
    auto pair = parseFuncName(lexer);  // funcName
    auto fnExp = pair.first;
    auto hasColon = pair.second;
    auto *fdExp = dynamic_cast<FuncDefExpression*>(parseFuncDefExpression(lexer)); // funcBody

    if(hasColon) {      // v:name(args) => v.name(self, args)
        fdExp->parList.insert(fdExp->parList.begin(), "self");
    }

    auto stat = new AssignStatement;
    stat->lastLine = fdExp->lastLine;
    stat->varList = std::vector<Expression *>{fnExp};
    stat->expList = std::vector<Expression *>{fdExp};
    return stat;
}

// local function Name funcbody
// local namelist [‘=’ explist]
Statement *Parser::parseLocalAssignOrFuncDefStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_LOCAL);
    if(lexer->LookAhead().id == TokenId::TOKEN_KW_FUNCTION) {
        return parseLocalFuncDefStatement(lexer);
    } else {
        return parseLocalVarDeclStatement(lexer);
    }
}

// local function Name funcbody
Statement *Parser::parseLocalFuncDefStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_FUNCTION);           // local function
    auto name = lexer->NextIdentifier().tokenStr;            // name
    Expression* funcDefExpression = parseFuncDefExpression(lexer);  // funcbody

    auto stat = new LocalFuncDefStatement;
    stat->name = name;
    stat->funcDefExp = funcDefExpression;

    return stat;
}

// local namelist [‘=’ explist]
Statement *Parser::parseLocalVarDeclStatement(Lexer *lexer) {
    auto name0 = lexer->NextIdentifier().tokenStr;          // local name
    auto nameList = finishNameList(lexer, name0);    // { , name }

    std::vector<Expression *> expList;
    if(lexer->LookAhead().id == TokenId::TOKEN_OP_ASSIGN) {
        lexer->NextToken();                         // ==
        expList = parseExpressionList(lexer);       // explist
    }
    int lastLine = lexer->GetLine();

    auto stat = new LocalVarDeclStatement;
    stat->lastLine = lastLine;
    stat->nameList = nameList;
    stat->expList = expList;

    return stat;
}

Statement *Parser::parseAssignOrFuncCallStatement(Lexer *lexer) {
    Expression *prefixExp = parsePrefixExp(lexer);
    if(isInstanceOf<FuncCallExpression>(prefixExp)) {
        auto stat = new FuncCallStatement;
        stat->exp = prefixExp;

        return stat;
    } else {
        return parseAssignStatement(lexer, prefixExp);
    }
}

Statement *Parser::parseAssignStatement(Lexer *lexer, Expression *var0) {
    auto varList = parseVarList(lexer, var0);    // varList
    lexer->NextTokenOfId(TokenId::TOKEN_OP_ASSIGN);             // =
    auto expList = parseExpressionList(lexer);   // expList
    int lastLine = lexer->GetLine();

    auto stat = new AssignStatement;
    stat->lastLine = lastLine;
    stat->varList = varList;
    stat->expList = expList;
    return stat;
}

