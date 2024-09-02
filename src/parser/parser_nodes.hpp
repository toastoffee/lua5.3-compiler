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
    std::vector<Statement> statements;
    std::vector<Expression> expressions;
};

struct Statement {};

struct Expression { };

//! simple statements
struct EmptyStatement : Statement {};   // ';'

struct BreakStatement : Statement {     // break
    int line;
};

struct LabelStatement : Statement {     // '::' Name '::'
    std::string name;
};

struct GotoStatement : Statement {      // goto Name
    std::string name;
};

struct DoStatement : Statement {
    Block *block;
};

//! while statement
// while exp do block end
struct WhileStatement : Statement {
    Expression exp;
    Block *block;
};

//! repeat statement
// repeat block until exp
struct RepeatStatement : Statement {
    Block *block;
    Expression *exp;
};

//! if statement
// if exp then block {elseif exp then block} [else block] end
// simplified: if exp then block {elseif exp then block} [elseif true then block] end
struct IfStatement : Statement {
    std::vector<Expression> exps;
    std::vector<Block*> blocks;
};

//! value for-loop statement
// for Name '=’ exp ',’ exp [',’ exp] do block end
struct  ForNumStatement : Statement {
    int lineOfFor;
    int lineOfDo;
    std::string varName;
    Expression initExp;
    Expression limitExp;
    Expression stepExp;
    Block *block;
};

//! common for-loop statement
// for namelist in explist do block end
// namelist ::= Name {',' Name}
// explist ::= exp {',' exp}
struct ForInStatement : Statement {
    int lineOfDo;
    std::vector<std::string> nameList;
    std::vector<Expression> exps;
    Block *bloc;
};

//! local variable declaration statement
// local namelist ['=' explist]
// namelist ::= Name {',' Name}
// explist ::= exp {',' exp}
struct LocalVarDeclStatement : Statement {
    int lastLine;
    std::vector<std::string> nameList;
    std::vector<Expression>  expList;
};

//! assignment statement
// varlist '=' explist
// varlist ::= var {',' var}
// var ::=  Name | prefixexp '[' exp ']' | prefixexp '.' Name
// explist ::= exp {',' exp}
struct AssignStatement : Statement {
    int lastLine;
    std::vector<Expression> varList;
    std::vector<Expression> expList;
};

//! non-local function definition statement
// function funcname funcbody
// funcname ::= Name {'.' Name} [':' Name]
// funcbody ::= '(' [parlist] ')' block end
// parlist ::= namelist [',' '...'] | '...'
// namelist ::= Name {',' Name}

//! local function definition statement
// local function Name funcbody
struct LocalFuncDefStatement : Statement {
    std::string name;
    Expression *funcDefExp;
};


#endif //LUA5_3_COMPILER_PARSER_NODES_HPP
