/**
  ******************************************************************************
  * @file           : parser_nodes.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_AST_NODES_HPP
#define LUA5_3_COMPILER_AST_NODES_HPP

#include <string>
#include <vector>

#include <types.hpp>
#include <lexer/token.hpp>

struct Node{
    virtual ~Node() = default;
};

struct Block;
struct Statement;
struct Expression;

// chunk := block
// block := {stat} [retstat]    ( {}:any times, []:0 or 1 time )
// retstat := return [explist] [';']
// explist := exp {',' exp }
struct Block : Node {
    int lastLine;
    std::vector<Statement*> statements;
    std::vector<Expression*> expressions;
};

struct Statement : Node {};

struct Expression : Node {};

//! simple statements
struct EmptyStatement : Statement {};   // ';'

struct BreakStatement : Statement {     // break
    int line{};
};

struct LabelStatement : Statement {     // '::' Name '::'
    std::string name;
};

struct GotoStatement : Statement {      // goto Name
    std::string name;
};

struct DoStatement : Statement {
    Block *block{};
};

//! while statement
// while exp do block end
struct WhileStatement : Statement {
    Expression *exp{};
    Block *block{};
};

//! repeat statement
// repeat block until exp
struct RepeatStatement : Statement {
    Block *block{};
    Expression *exp{};
};

//! if statement
// if exp then block {elseif exp then block} [else block] end
// simplified: if exp then block {elseif exp then block} [elseif true then block] end
struct IfStatement : Statement {
    std::vector<Expression*> exps;
    std::vector<Block*> blocks;
};

//! value for-loop statement
// for Name '=’ exp ',’ exp [',’ exp] do block end
struct  ForNumStatement : Statement {
    int lineOfFor;
    int lineOfDo;
    std::string varName;
    Expression *initExp;
    Expression *limitExp;
    Expression *stepExp;
    Block *block;
};

//! common for-loop statement
// for namelist in explist do block end
// namelist ::= Name {',' Name}
// explist ::= exp {',' exp}
struct ForInStatement : Statement {
    int lineOfDo;
    std::vector<std::string> nameList;
    std::vector<Expression*> exps;
    Block *block;
};

//! local variable declaration statement
// local namelist ['=' explist]
// namelist ::= Name {',' Name}
// explist ::= exp {',' exp}
struct LocalVarDeclStatement : Statement {
    int lastLine;
    std::vector<std::string> nameList;
    std::vector<Expression*>  expList;
};

//! assignment statement
// varlist '=' explist
// varlist ::= var {',' var}
// var ::=  Name | prefixexp '[' exp ']' | prefixexp '.' Name
// explist ::= exp {',' exp}
struct AssignStatement : Statement {
    int lastLine;
    std::vector<Expression*> varList;
    std::vector<Expression*> expList;
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

struct FuncCallStatement : Statement {
    Expression *exp{};
};

//! simple expressions
// exp ::=  nil | false | true | Numeral | LiteralString | '...'
//           | functiondef | prefixexp | tableconstructor
//           | exp binop exp | unop exp
struct NilExpression : Expression {
    int line{};
};

struct TrueExpression : Expression {
    int line{};
};

struct FalseExpression : Expression {
    int line{};
};

struct VarargExpression : Expression {
    int line{};
};

struct IntegerExpression : Expression {
    int line{};
    i64 val{};
};

struct FloatExpression : Expression {
    int line{};
    f64 val{};
};

struct StringExpression : Expression {
    int line;
    std::string str;
};

struct NameExpression : Expression {
    int line;
    std::string name;

    NameExpression(int line, std::string name)
    : line(line), name(name) { }
};

//! un-op & bin-op algorithm expressions
struct unopExpression : Expression {
    int line{};   // line of operator
    TokenId op{};     // operator
    Expression *exp{};
};

struct BinopExpression : Expression {
    BinopExpression(int line, TokenId op, Expression *expL, Expression *expR)
    : line(line), op(op), expL(expL), expR(expR) {}

    int line{};
    TokenId op{};
    Expression *expL{};
    Expression *expR{};
};

struct ConcatExpression : Expression {
    int line;
    std::vector<Expression*> exps;
};

//! table constructor expressions
// tableconstructor ::= '{' [fieldlist] '}'
// fieldlist ::= field {fieldsep field} [fieldsep]
// field ::= '[' exp ']' '=' exp | Name '=' exp | exp
// fieldsep ::= ',' | ';”
struct TableConstructorExpression : Expression {
    int line;       // line of "{"
    int lastLine;   // line of "}"
    std::vector<Expression*> keyExpressions;
    std::vector<Expression*> valExpressions;
};

//! function definition expressions
// functiondef ::= function funcbody
// funcbody ::= '(' [parlist] ')' block end
// parlist ::= namelist [',' '...'] | '...'
// namelist ::= Name {',' Name}
struct FuncDefExpression : Expression {
    int line;
    int lastLine;
    std::vector<std::string> parList;
    bool isVararg;
    Block *block;
};

//! prefix expressions
// prefixexp ::= var | functioncall | '(' exp ')'
// var ::=  Name | prefixexp '[' exp ']' | prefixexp '.' Name
// functioncall ::=  prefixexp args | prefixexp ':' Name args

// prefixexp ::= Name
//           | '(' exp ')'
//           | prefixexp '[' exp ']'
//           | prefixexp '.' Name
//           | prefixexp [':' Name] args


//! Paren expressions
struct ParensExpression : Expression {
    Expression *exp{};
};

//! table access expressions
struct TableAccessExpression : Expression {
    TableAccessExpression() = default;
    TableAccessExpression(int lastLine, Expression* prefixExp, Expression* keyExp)
    : lastLine(lastLine), prefixExpression(prefixExp), keyExpression(keyExp) { }

    int lastLine{};   // line of ']'
    Expression *prefixExpression{};
    Expression *keyExpression{};


};

//! function call expressions
// functioncall ::=  prefixexp [':' Name] args
// args ::=  '(' [explist] ')' | tableconstructor | LiteralString
struct FuncCallExpression : Expression {
    int line;       // line of '('
    int lastLine;   // line of ')'
    Expression *prefixExp;
    StringExpression *nameExp;
    std::vector<Expression*> args;
};

#endif //LUA5_3_COMPILER_AST_NODES_HPP
