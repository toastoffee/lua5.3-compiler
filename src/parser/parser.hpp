/**
  ******************************************************************************
  * @file           : block_parser.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_PARSER_HPP
#define LUA5_3_COMPILER_PARSER_HPP


#include <parser/ast_nodes.hpp>
#include <lexer/lexer.hpp>

// recursive decline parser

class Parser {
private:
    static bool isReturnOrBlockEnd(const Token& token);

public:
    static Block *ParseBlock(Lexer *lexer);

    // statement parser
    static std::vector<Statement *> ParseStatements(Lexer *lexer);
    static Statement* ParseStatement(Lexer *lexer);
    static Statement* ParseEmptyStatement(Lexer *lexer);
    static Statement* ParseBreakStatement(Lexer *lexer);
    static Statement* ParseLabelStatement(Lexer *lexer);
    static Statement* ParseGotoStatement(Lexer *lexer);
    static Statement* ParseDoStatement(Lexer *lexer);
    static Statement* ParseWhileStatement(Lexer *lexer);
    static Statement* ParseRepeatStatement(Lexer *lexer);
    static Statement* ParseIfStatement(Lexer *lexer);
    static Statement* ParseForStatement(Lexer *lexer);
    static Statement* ParseFuncDefStatement(Lexer *lexer);
    static Statement* ParseLocalAssignOrFuncDefStatement(Lexer *lexer);
    static Statement* ParseAssignOrFuncStatement(Lexer *lexer);

    // expression parser
    static std::vector<Expression *> ParseRetExpressions(Lexer *lexer);
    static std::vector<Expression *> ParseExpressionList(Lexer *lexer);
    static Expression* ParseExpression(Lexer *lexer);


};

#endif //LUA5_3_COMPILER_PARSER_HPP
