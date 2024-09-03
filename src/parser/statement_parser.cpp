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

Statement* Parser::ParseStatement(Lexer *lexer) {
    auto stat = new Statement();

    if(lexer->LookAhead().id == TokenId::TOKEN_SEP_SEMI) {
        return ParseEmptyStatement(lexer);
    }
    else if(lexer->LookAhead().id == TokenId::TOKEN_SEP_SEMI)

    return stat;
}

Statement *Parser::ParseEmptyStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseBreakStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseLabelStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseGotoStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseDoStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseWhileStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseRepeatStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseIfStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseForStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseFuncDefStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseLocalAssignOrFuncDefStatement(Lexer *lexer) {
    return nullptr;
}

Statement *Parser::ParseAssignOrFuncStatement(Lexer *lexer) {
    return nullptr;
}
