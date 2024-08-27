/**
  ******************************************************************************
  * @file           : lexer.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/27
  ******************************************************************************
  */

#include <lexer/lexer.hpp>
#include <utility>

Lexer::Lexer(std::string chunk, std::string chunkName) {
    _chunk = chunk;
    _chunkName = chunk;
    _line = 1;
}

Token Lexer::NextToken() {

    return Token{.line = this->_line};
}

void Lexer::SkipBlankSpaces() {
    while(_chunk.length() > 0) {
        
    }
}
