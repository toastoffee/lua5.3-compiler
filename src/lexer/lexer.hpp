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
    std::string m_chunk;
    std::string m_chunkName;
    int         m_line;

    void SkipBlankSpaces();
    bool Test(const std::string& prefix) const;
    void Next(int n);
    void SkipComment();
    bool IsNewLine(char chr) const;
    bool IsWhiteSpace(char chr) const;

public:
    Lexer(std::string chunk, std::string chunkName);

    Token NextToken();


};

#endif //LUA5_3_COMPILER_LEXER_HPP
