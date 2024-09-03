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
    switch(lexer->LookAhead().id) {
        case TokenId::TOKEN_SEP_SEMI:    return ParseEmptyStatement(lexer);
        case TokenId::TOKEN_KW_BREAK:    return ParseBreakStatement(lexer);
        case TokenId::TOKEN_SEP_LABEL:   return ParseLabelStatement(lexer);
        case TokenId::TOKEN_KW_GOTO:     return ParseGotoStatement(lexer);
        case TokenId::TOKEN_KW_DO:       return ParseDoStatement(lexer);
        case TokenId::TOKEN_KW_WHILE:    return ParseWhileStatement(lexer);
        case TokenId::TOKEN_KW_REPEAT:   return ParseRepeatStatement(lexer);
        case TokenId::TOKEN_KW_IF:       return ParseIfStatement(lexer);
        case TokenId::TOKEN_KW_FOR:      return ParseForStatement(lexer);
        case TokenId::TOKEN_KW_FUNCTION: return ParseFuncDefStatement(lexer);
        case TokenId::TOKEN_KW_LOCAL:    return ParseLocalAssignOrFuncDefStatement(lexer);
        default:                         return ParseAssignOrFuncCallStatement(lexer);
    }

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

Statement *Parser::ParseAssignOrFuncCallStatement(Lexer *lexer) {
    return nullptr;
}
