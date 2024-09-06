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

    static Statement* parseForNumStatement(Lexer *lexer, std::string varName, int lineOfFor);
    static Statement* parseForInStatement(Lexer *lexer, std::string name0);
    static std::vector<std::string> finishNameList(Lexer *lexer, std::string name0);
    static Statement* parseLocalFuncDefStatement(Lexer *lexer);
    static Statement* parseLocalVarDeclStatement(Lexer *lexer);
    static Statement* parseAssignStatement(Lexer *lexer, Expression *var0);


    static Expression* parseFuncDefExpression(Lexer *lexer);
    static Expression* parsePrefixExp(Lexer *lexer);
    static std::vector<Expression *> parseVarList(Lexer *lexer, Expression *var0);
    static Expression* checkVar(Lexer *lexer, Expression *exp);
    static std::pair<Expression *, bool> parseFuncName(Lexer *lexer);

    static Expression* parseExpression_12(Lexer *lexer);
    static Expression* parseExpression_11(Lexer *lexer);
    static Expression* parseExpression_10(Lexer *lexer);
    static Expression* parseExpression_9(Lexer *lexer);
    static Expression* parseExpression_8(Lexer *lexer);
    static Expression* parseExpression_7(Lexer *lexer);
    static Expression* parseExpression_6(Lexer *lexer);
    static Expression* parseExpression_5(Lexer *lexer);
    static Expression* parseExpression_4(Lexer *lexer);
    static Expression* parseExpression_3(Lexer *lexer);
    static Expression* parseExpression_2(Lexer *lexer);
    static Expression* parseExpression_1(Lexer *lexer);
    static Expression* parseExpression_0(Lexer *lexer);

    static Expression* parseNumberExpression(Lexer *lexer);
    static Expression* parseTableConstructorExpression(Lexer *lexer);
    static Expression* parseParensExpression(Lexer *lexer);
    static Expression* finishPrefixExpression(Lexer *lexer, Expression *exp);
    static Expression* finishFuncCallExpression(Lexer *lexer, Expression *exp);
    static StringExpression* parseNameExpression(Lexer *lexer);
    static std::vector<Expression*> parseArgs(Lexer *lexer);

    static std::pair<std::vector<std::string>, bool> parseParList(Lexer *lexer);
    static std::pair<std::vector<Expression *>, std::vector<Expression *>> parseFieldList(Lexer *lexer);
    static std::pair<Expression*, Expression*> parseField(Lexer *lexer);
    static bool isFieldSep(TokenId tokenId);

    static Block *parseBlock(Lexer *lexer);

    // statement parser
    static std::vector<Statement *> parseStatements(Lexer *lexer);
    static Statement* parseStatement(Lexer *lexer);
    static Statement* parseEmptyStatement(Lexer *lexer);
    static Statement* parseBreakStatement(Lexer *lexer);
    static Statement* parseLabelStatement(Lexer *lexer);
    static Statement* parseGotoStatement(Lexer *lexer);
    static Statement* parseDoStatement(Lexer *lexer);
    static Statement* parseWhileStatement(Lexer *lexer);
    static Statement* parseRepeatStatement(Lexer *lexer);
    static Statement* parseIfStatement(Lexer *lexer);
    static Statement* parseForStatement(Lexer *lexer);
    static Statement* parseFuncDefStatement(Lexer *lexer);
    static Statement* parseLocalAssignOrFuncDefStatement(Lexer *lexer);
    static Statement* parseAssignOrFuncCallStatement(Lexer *lexer);

    // expression parser
    static std::vector<Expression *> parseRetExpressions(Lexer *lexer);
    static std::vector<Expression *> parseExpressionList(Lexer *lexer);
    static Expression* parseExpression(Lexer *lexer);

    static Expression* optimizeUnaryOp(UnopExpression* exp);
    static Expression* optimizeUnm(UnopExpression* exp);
    static Expression* optimizeNot(UnopExpression* exp);
    static Expression* optimizeBnot(UnopExpression* exp);


public:

    static Block* Parse(const std::string& chunk, const std::string& chunkName);

};

#endif //LUA5_3_COMPILER_PARSER_HPP
