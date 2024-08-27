/**
  ******************************************************************************
  * @file           : lexer.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/27
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_LEXER_HPP
#define LUA5_3_COMPILER_LEXER_HPP

#include <lexer/token.hpp>

class Lexer {
private:
    std::string _chunk;
    std::string _chunkName;
    int         _line;

public:
    Lexer(std::string chunk, std::string chunkName);

    Token NextToken();
    void SkipBlankSpaces();
};

#endif //LUA5_3_COMPILER_LEXER_HPP
