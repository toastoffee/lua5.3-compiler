/**
  ******************************************************************************
  * @file           : statement_parser.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_STATEMENT_PARSER_HPP
#define LUA5_3_COMPILER_STATEMENT_PARSER_HPP

#include <string>

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



#endif //LUA5_3_COMPILER_STATEMENT_PARSER_HPP
