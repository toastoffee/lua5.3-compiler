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

Lexer::Lexer(std::string chunk, std::string chunkName) {
    m_chunk = chunk;
    m_chunkName = chunk;
    m_chunkScanPos = 0;
    m_line = 1;
}

Token Lexer::NextToken() {
    skipBlankSpaces();
    if(unscannedSize() == 0) {
        return Token {.line = m_line, .id = TokenId::TOKEN_EOF, .tokenStr="EOF"};
    }

    switch(m_chunk[m_chunkScanPos]) {
        case ';':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_SEMI, .tokenStr=";"};
        case ',':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_COMMA, .tokenStr=","};
        case '(':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_LPAREN, .tokenStr="("};
        case ')':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_RPAREN, .tokenStr=")"};
        case ']':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_RBRACK, .tokenStr="]"};
        case '{':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_LCURLY, .tokenStr="{"};
        case '}':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_RCURLY, .tokenStr="}"};
        case '+':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_ADD, .tokenStr="+"};
        case '-':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_MINUS, .tokenStr="-"};
        case '*':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_MUL, .tokenStr="*"};
        case '^':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_POW, .tokenStr="^"};
        case '%':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_MOD, .tokenStr="%"};
        case '&':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_BAND, .tokenStr="&"};
        case '|':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_BOR, .tokenStr="|"};
        case '#':
            next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_LEN, .tokenStr="#"};
        case ':':
            if (test("::")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_SEP_LABEL, .tokenStr="::"};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_COLON, .tokenStr=":"};
            }
        case '/':
            if (test("//")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_IDIV, .tokenStr="//"};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_DIV, .tokenStr="/"};
            }
        case '~':
            if (test("~=")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_NE, .tokenStr="~="};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_WAVE, .tokenStr="~"};
            }
        case '=':
            if (test("==")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_EQ, .tokenStr="=="};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_ASSIGN, .tokenStr="="};
            }
        case '<':
            if (test("<<")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_SHL, .tokenStr="<<"};
            } else if(test("<=")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_LE, .tokenStr="<="};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_LT, .tokenStr="<"};
            }
        case '>':
            if (test(">>")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_SHR, .tokenStr=">>"};
            } else if(test(">=")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_GE, .tokenStr=">="};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_OP_GT, .tokenStr=">"};
            }
        case '.':
            if (test("...")) {
                next(3); return {.line = m_line, .id = TokenId::TOKEN_VARARG, .tokenStr="..."};
            } else if (test("..")) {
                next(2); return {.line = m_line, .id = TokenId::TOKEN_OP_CONCAT, .tokenStr=".."};
            } else if (unscannedSize() == 1 || !isDigit(m_chunk[m_chunkScanPos+1])) {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_DOT, .tokenStr="."};
            }
        case '[':
            if (test("[[") || test("[=")) {
                return {.line = m_line, .id = TokenId::TOKEN_STRING, .tokenStr=scanLongString()};
            } else {
                next(1); return {.line = m_line, .id = TokenId::TOKEN_SEP_LBRACK, .tokenStr="["};
            }
        case '\'':
        case '"':
            return {.line = m_line, .id = TokenId::TOKEN_STRING, .tokenStr=scanShortString()};
            
    }
}

void Lexer::skipBlankSpaces() {
    while(unscannedSize() > 0) {
        if(test("--")) {
            skipComment();
        }
        else if(test("\r\n") || test("\n\r")) {
            next(2);
            m_line += 1;
        }
        else if(isNewLine(m_chunk[m_chunkScanPos])) {
            next(1);
            m_line += 1;
        }
        else if(isWhiteSpace(m_chunk[m_chunkScanPos])) {
            next(1);
        }
        else {
            break;
        }
    }
}

bool Lexer::test(const std::string& prefix) const {
    return m_chunk.compare(m_chunkScanPos, prefix.size(), prefix) == 0;
}

void Lexer::next(int n) {
    m_chunkScanPos += n;
}

void Lexer::skipComment() {
    next(2);    // skip "--"
    if(test("[")) {     // long comment ?
        // find the right bracket
        if(!findOpeningLongBracket(unscannedChunk()).empty()) {
            scanLongString();
            return;
        }
    }
    // short comment
    while(unscannedSize() > 0 && !isNewLine(m_chunk[m_chunkScanPos])) {
        next(1);
    }
}

bool Lexer::isNewLine(char c) {
    return c == '\r' || c == '\n';
}

bool Lexer::isWhiteSpace(char c) {
    switch (c) {
        case '\t':
        case '\n':
        case '\v':
        case '\f':
        case '\r':
        case ' ':
            return true;
        default:
            return false;
    }
}

bool Lexer::isDigit(char c) {
    return false;
}

std::string Lexer::scanLongString() {
    std::string openingLongBracket = findOpeningLongBracket(unscannedChunk());
    assert(!openingLongBracket.empty() && "invalid long string delimiter");

    std::string closingLongBracket = openingLongBracket;
    closingLongBracket[0] = ']';
    closingLongBracket[closingLongBracket.size()-1] = ']';

    int closingLongBracketIdx = 


}

std::string Lexer::scanShortString() {
    return std::string();
}

std::string Lexer::findOpeningLongBracket(std::string s) {
    int matchLen = 0;
    for (int i = 0; i < s.size(); ++i) {
        if(i == 0) {
            if(s[i] != '[') {
                matchLen = 0;
                break;
            } else {
                matchLen++;
            }
        }
        else {
            if(s[i] == ']') {
                matchLen++;
                break;
            } else if(s[i] == '=') {
                matchLen++;
                continue;
            } else {
                matchLen = 0;
                break;
            }
        }
    }
    return matchLen > 0 ? s.substr(matchLen) : "";
}

