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
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_SEMI);
    return new EmptyStatement;
}

// break
Statement *Parser::ParseBreakStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_BREAK);

    auto stat = new BreakStatement;
    stat->line = lexer->GetLine();
    return stat;
}

// ‘::’ Name ‘::’
Statement *Parser::ParseLabelStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LABEL); // ::
    auto identifier = lexer->NextIdentifier();  // name
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_LABEL); // ::

    auto stat = new LabelStatement;
    stat->name = identifier.tokenStr;
    return stat;
}

// goto Name
Statement *Parser::ParseGotoStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_GOTO);   // goto
    auto identifier = lexer->NextIdentifier();  // name

    auto stat = new GotoStatement;
    stat->name = identifier.tokenStr;
    return stat;
}

// do block end
Statement *Parser::ParseDoStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);     // do
    Block* block = ParseBlock(lexer);                   // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end

    auto stat = new DoStatement;
    stat->block = block;
    return stat;
}

// while exp do block end
Statement *Parser::ParseWhileStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_WHILE);  // while
    Expression *exp = ParseExpression(lexer);           // exp
    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);     // do
    Block *block = ParseBlock(lexer);                   // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end

    auto stat = new WhileStatement;
    stat->exp = exp;
    stat->block = block;
    return stat;
}

// repeat block until exp
Statement *Parser::ParseRepeatStatement(Lexer *lexer) {
    lexer->NextTokenOfId(TokenId::TOKEN_KW_REPEAT); // repeat
    Block *block = ParseBlock(lexer);                   // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_UNTIL);  // until
    Expression *exp = ParseExpression(lexer);           // exp

    auto stat = new RepeatStatement;
    stat->block = block;
    stat->exp = exp;
    return stat;
}

// if exp then block {elseif exp then block} [else block] end
Statement *Parser::ParseIfStatement(Lexer *lexer) {
    std::vector<Expression *> exps;
    std::vector<Block *> blocks;

    lexer->NextTokenOfId(TokenId::TOKEN_KW_IF);     // if
    exps.push_back(ParseExpression(lexer));             // exp
    lexer->NextTokenOfId(TokenId::TOKEN_KW_THEN);   // then
    blocks.push_back(ParseBlock(lexer));                // block

    while(lexer->LookAhead().id == TokenId::TOKEN_KW_ELSEIF) {
        lexer->NextToken();                                 // elseif
        exps.push_back(ParseExpression(lexer));             // exp
        lexer->NextTokenOfId(TokenId::TOKEN_KW_THEN);   // then
        blocks.push_back(ParseBlock(lexer));                // block
    }

    // else block => elseif true then block
    if(lexer->LookAhead().id == TokenId::TOKEN_KW_ELSE) {
        lexer->NextToken();                         // else

        auto trueExp = new TrueExpression;          // true
        trueExp->line = lexer->GetLine();
        exps.push_back(trueExp);
        blocks.push_back(ParseBlock(lexer));        // block
    }

    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);    // end
    auto stat = new IfStatement;
    stat->exps = exps;
    stat->blocks = blocks;
    return stat;
}

// for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end
// for namelist in explist do block end
Statement *Parser::ParseForStatement(Lexer *lexer) {
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
Statement *Parser::parseForNumStatement(Lexer *lexer, std::string name, int lineOfFor) {
    auto stat = new ForNumStatement;
    stat->lineOfFor = lineOfFor;                            // for
    stat->varName = name;                                   // name

    lexer->NextTokenOfId(TokenId::TOKEN_OP_ASSIGN);     // =
    stat->initExp = ParseExpression(lexer);                 // exp
    lexer->NextTokenOfId(TokenId::TOKEN_SEP_COMMA);     // ,
    stat->limitExp = ParseExpression(lexer);                // exp

    if(lexer->LookAhead().id == TokenId::TOKEN_SEP_COMMA) {
        lexer->NextToken();                                 // ,
        stat->stepExp = ParseExpression(lexer);             // exp
    } else {
        auto stepExp = new IntegerExpression;
        stepExp->line = lexer->GetLine();
        stepExp->val = 1;
        stat->stepExp = stepExp;
    }

    lexer->NextTokenOfId(TokenId::TOKEN_KW_DO);         // do
    stat->lineOfDo = lexer->GetLine();
    stat->block = ParseBlock(lexer);                        // block
    lexer->NextTokenOfId(TokenId::TOKEN_KW_END);        // end

    return stat;
}

Statement *Parser::parseForInStatement(Lexer *lexer, std::string name) {
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
