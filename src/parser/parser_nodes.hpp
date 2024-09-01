/**
  ******************************************************************************
  * @file           : parser_nodes.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_PARSER_NODES_HPP
#define LUA5_3_COMPILER_PARSER_NODES_HPP

#include <string>
#include <vector>

struct Block;
struct Statement;
struct Expression;

struct Block {
    int lastLine;
};

struct Expression {

};

struct EmptyStatement {};   // ';'

struct BreakStatement {     // break
    int line;
};

struct LabelStatement {     // '::' Name '::'
    std::string name;
};

struct GotoStatement {      // goto Name
    std::string name;
};

struct DoStatement {
    Block *block;
};

// while exp do block end
struct WhileStatement {
    Expression exp;
    Block *block;
};

// repeat block until exp
struct RepeatStatement {
    Block *block;
    Expression *exp;
};

// if exp then block {elseif exp then block} [else block] end
// simplified: if exp then block {elseif exp then block} [elseif true then block] end
struct IfStatement {
    std::vector<Expression> exps;
    std::vector<Block*> blocks;
};

#endif //LUA5_3_COMPILER_PARSER_NODES_HPP
