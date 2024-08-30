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

    const static std::regex  s_regexNewLine;
    const static std::regex  s_regexShortStr;
    const static std::regex  s_regexDecEscapeSeq;
    const static std::regex  s_regexHexEscapeSeq;
    const static std::regex  s_regexUnicodeEscapeSeq;
    const static std::regex  s_regexNumber;


    inline std::string unscannedChunk() const { return m_chunk.substr(m_chunkScanPos); };
    inline int  unscannedSize() const { return m_chunk.size() - m_chunkScanPos; }
    void skipBlankSpaces();
    bool test(const std::string& prefix) const;
    void next(int n);
    void skipComment();
    std::string scanLongString();
    std::string scanShortString();
    std::string scanNumber();
    std::string scan(const std::regex& regex);

    static std::string findOpeningLongBracket(std::string s);
    static std::string escape(std::string s);
    static bool isNewLine(char c);
    static bool isWhiteSpace(char c);
    static bool isDigit(char c);


public:
    Lexer(std::string chunk, std::string chunkName);

    Token NextToken();

};

const std::regex Lexer::s_regexNewLine("\r\n|\n\r|\n|\r");
const std::regex Lexer::s_regexShortStr("(?s)(^'(\\\\\\\\|\\\\’|\\\\\\n|\\\\z\\s*|[^’\\n])*’)|(^\"(\\\\\\\\|\\\\\"|\\\\\\n|\\\\z\\s*|[^\"\\n])*\")");
const std::regex Lexer::s_regexDecEscapeSeq("^\\\\[0-9]{1,3}");
const std::regex Lexer::s_regexHexEscapeSeq("^\\\\x[0-9a-fA-F]{2}");
const std::regex Lexer::s_regexUnicodeEscapeSeq("^\\\\u\\{[0-9a-fA-F]+\\}");
const std::regex Lexer::s_regexNumber("^0[xX][0-9a-fA-F]*(\\.[0-9a-fA-F]*)?([pP][+\\-]?[0-9]+)?|^[0-9]*(\\.[0-9]*)?([eE][+\\-]?[0-9]+)?");



#endif //LUA5_3_COMPILER_LEXER_HPP
