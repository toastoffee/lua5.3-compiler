/**
  ******************************************************************************
  * @file           : token.h
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/27
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_TOKEN_HPP
#define LUA5_3_COMPILER_TOKEN_HPP

#include <string>
#include <map>

#include <types.hpp>

enum class TokenId : u8 {
    TOKEN_EOF         = 0,                  // end-of-file
    TOKEN_VARARG      = 1,                  // ...
    TOKEN_SEP_SEMI    = 2,                  // ;
    TOKEN_SEP_COMMA   = 3,                  // ,
    TOKEN_SEP_DOT     = 4,                  // .
    TOKEN_SEP_COLON   = 5,                  // :
    TOKEN_SEP_LABEL   = 6,                  // ::
    TOKEN_SEP_LPAREN  = 7,                  // (
    TOKEN_SEP_RPAREN  = 8,                  // )
    TOKEN_SEP_LBRACK  = 9,                  // [
    TOKEN_SEP_RBRACK  = 10,                 // ]
    TOKEN_SEP_LCURLY  = 11,                 // {
    TOKEN_SEP_RCURLY  = 12,                 // }
    TOKEN_OP_ASSIGN   = 13,                 // =
    TOKEN_OP_MINUS    = 14,                 // - (sub or unm)
    TOKEN_OP_WAVE     = 15,                 // ~ (bnot or bxor)
    TOKEN_OP_ADD      = 16,                 // +
    TOKEN_OP_MUL      = 17,                 // *
    TOKEN_OP_DIV      = 18,                 // /
    TOKEN_OP_IDIV     = 19,                 // //
    TOKEN_OP_POW      = 20,                 // ^
    TOKEN_OP_MOD      = 21,                 // %
    TOKEN_OP_BAND     = 22,                 // &
    TOKEN_OP_BOR      = 23,                 // |
    TOKEN_OP_SHR      = 24,                 // >>
    TOKEN_OP_SHL      = 25,                 // <<
    TOKEN_OP_CONCAT   = 26,                 // ..
    TOKEN_OP_LT       = 27,                 // <
    TOKEN_OP_LE       = 28,                 // <=
    TOKEN_OP_GT       = 29,                 // >
    TOKEN_OP_GE       = 30,                 // >=
    TOKEN_OP_EQ       = 31,                 // ==
    TOKEN_OP_NE       = 32,                 // ~=
    TOKEN_OP_LEN      = 33,                 // #
    TOKEN_OP_AND      = 34,                 // and
    TOKEN_OP_OR       = 35,                 // or
    TOKEN_OP_NOT      = 36,                 // not
    TOKEN_KW_BREAK    = 37,                 // break
    TOKEN_KW_DO       = 38,                 // do
    TOKEN_KW_ELSE     = 39,                 // else
    TOKEN_KW_ELSEIF   = 40,                 // elseif
    TOKEN_KW_END      = 41,                 // end
    TOKEN_KW_FALSE    = 42,                 // false
    TOKEN_KW_FOR      = 43,                 // for
    TOKEN_KW_FUNCTION = 44,                 // function
    TOKEN_KW_GOTO     = 45,                 // goto
    TOKEN_KW_IF       = 46,                 // if
    TOKEN_KW_IN       = 47,                 // in
    TOKEN_KW_LOCAL    = 48,                 // local
    TOKEN_KW_NIL      = 49,                 // nil
    TOKEN_KW_REPEAT   = 50,                 // repeat
    TOKEN_KW_RETURN   = 51,                 // return
    TOKEN_KW_THEN     = 52,                 // then
    TOKEN_KW_TRUE     = 53,                 // true
    TOKEN_KW_UNTIL    = 54,                 // until
    TOKEN_KW_WHILE    = 55,                 // while
    TOKEN_IDENTIFIER  = 56,                 // identifier
    TOKEN_NUMBER      = 57,                 // number literal
    TOKEN_STRING      = 58,                 // string literal
    TOKEN_OP_UNM      = TOKEN_OP_MINUS,     // unary minus
    TOKEN_OP_SUB      = TOKEN_OP_MINUS,
    TOKEN_OP_BNOT     = TOKEN_OP_WAVE,
    TOKEN_OP_BXOR     = TOKEN_OP_WAVE
};

struct Token {
    int         line;
    TokenId     id;
    std::string tokenStr;
};

std::map<std::string, TokenId> keywords {
        {"and",     TokenId::TOKEN_OP_AND},
        {"break",   TokenId::TOKEN_KW_BREAK},
        {"do",      TokenId::TOKEN_KW_DO},
        {"else",    TokenId::TOKEN_KW_ELSE},
        {"elseif",  TokenId::TOKEN_KW_ELSEIF},
        {"end",     TokenId::TOKEN_KW_END},
        {"false",   TokenId::TOKEN_KW_FALSE},
        {"for",     TokenId::TOKEN_KW_FOR},
        {"function",TokenId::TOKEN_KW_FUNCTION},
        {"goto",    TokenId::TOKEN_KW_GOTO},
        {"if",      TokenId::TOKEN_KW_IF},
        {"in",      TokenId::TOKEN_KW_IN},
        {"local",   TokenId::TOKEN_KW_LOCAL},
        {"nil",     TokenId::TOKEN_KW_NIL},
        {"not",     TokenId::TOKEN_OP_NOT},
        {"or",      TokenId::TOKEN_OP_OR},
        {"repeat",  TokenId::TOKEN_KW_REPEAT},
        {"return",  TokenId::TOKEN_KW_RETURN},
        {"then",    TokenId::TOKEN_KW_THEN},
        {"true",    TokenId::TOKEN_KW_TRUE},
        {"until",   TokenId::TOKEN_KW_UNTIL},
        {"while",   TokenId::TOKEN_KW_WHILE},
};

#endif //LUA5_3_COMPILER_TOKEN_HPP
