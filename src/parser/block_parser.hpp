/**
  ******************************************************************************
  * @file           : block_parser.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_BLOCK_PARSER_HPP
#define LUA5_3_COMPILER_BLOCK_PARSER_HPP


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

    // expression parser
    static std::vector<Expression *> ParseRetExpressions(Lexer *lexer);
    static std::vector<Expression *> ParseExpressionList(Lexer *lexer);
    static Expression* ParseExpression(Lexer *lexer);


};

#endif //LUA5_3_COMPILER_BLOCK_PARSER_HPP
