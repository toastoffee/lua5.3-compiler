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
    int         m_chunkScanPos;
    int         m_line;

    int  unscannedSize() const { return m_chunk.size() - m_chunkScanPos; }
    void skipBlankSpaces();
    bool test(const std::string& prefix) const;
    void next(int n);
    void skipComment();
    std::string scanLongString();
    std::string scanShortString();
    static bool isLongStringLeftBracket(std::string s);
    static bool isNewLine(char c);
    static bool isWhiteSpace(char c);
    static bool isDigit(char c);


public:
    Lexer(std::string chunk, std::string chunkName);

    Token NextToken();


};

#endif //LUA5_3_COMPILER_LEXER_HPP
