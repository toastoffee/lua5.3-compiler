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
#include <regex>

class Lexer {
private:
    std::string m_chunk;
    std::string m_chunkName;
    int         m_chunkScanPos;
    int         m_line;

    static std::regex  s_regexNewLine;
    static std::regex  s_regexShortStr;
    static std::regex  s_regexDecEscapeSeq;
    static std::regex  s_regexHexEscapeSeq;
    static std::regex  s_regexUnicodeEscapeSeq;
    static std::regex  s_regexNumber;
    static std::regex  s_regexIdentifier;


    inline std::string  unscannedChunk() const { return m_chunk.substr(m_chunkScanPos); };
    inline int          unscannedSize() const { return m_chunk.size() - m_chunkScanPos; }
    void skipBlankSpaces();
    bool test(const std::string& prefix) const;
    void next(int n);
    void skipComment();
    std::string scanLongString();
    std::string scanShortString();
    std::string scanNumber();
    std::string scanIdentifier();
    std::string scan(const std::regex& regex);

    static std::string findOpeningLongBracket(std::string s);
    static std::string escape(std::string s);
    static std::string strSect(const std::string& s, int start, int end);
    static bool isNewLine(char c);
    static bool isWhiteSpace(char c);
    static bool isDigit(char c);
    static bool isAlpha(char c);


public:
    Lexer(std::string chunk, std::string chunkName);

    Token NextToken();

};

#endif //LUA5_3_COMPILER_LEXER_HPP
