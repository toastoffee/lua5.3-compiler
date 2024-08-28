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
    SkipBlankSpaces();
    if(m_chunk.size() == m_chunkScanPos) {
        return Token {.line = m_line, .id = TokenId::TOKEN_EOF, .tokenStr="EOF"};
    }

    switch(m_chunk[m_chunkScanPos]) {
        case ';':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_SEMI, .tokenStr=";"};
        case ',':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_COMMA, .tokenStr=","};
        case '(':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_LPAREN, .tokenStr="("};
        case ')':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_RPAREN, .tokenStr=")"};
        case ']':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_RBRACK, .tokenStr="]"};
        case '{':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_LCURLY, .tokenStr="{"};
        case '}':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_RCURLY, .tokenStr="}"};
        case '+':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_ADD, .tokenStr="+"};
        case '-':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_MINUS, .tokenStr="-"};
        case '*':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_MUL, .tokenStr="*"};
        case '^':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_POW, .tokenStr="^"};
        case '%':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_MOD, .tokenStr="%"};
        case '&':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_BAND, .tokenStr="&"};
        case '|':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_BOR, .tokenStr="|"};
        case '#':
            Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_LEN, .tokenStr="#"};
        case ':':
            if(Test("::")) {
                Next(2); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_LABEL, .tokenStr="::"};
            } else {
                Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_SEP_COLON, .tokenStr=":"};
            }
        case '/':
            if(Test("//")) {
                Next(2); return Token {.line = m_line, .id = TokenId::TOKEN_OP_IDIV, .tokenStr="//"};
            } else {
                Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_DIV, .tokenStr="/"};
            }
        case '~':
            if(Test("~=")) {
                Next(2); return Token {.line = m_line, .id = TokenId::TOKEN_OP_NE, .tokenStr="~="};
            } else {
                Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_WAVE, .tokenStr="~"};
            }
        case '=':
            if(Test("==")) {
                Next(2); return Token {.line = m_line, .id = TokenId::TOKEN_OP_EQ, .tokenStr="=="};
            } else {
                Next(1); return Token {.line = m_line, .id = TokenId::TOKEN_OP_ASSIGN, .tokenStr="="};
            }


    }
}

void Lexer::SkipBlankSpaces() {
    while(m_chunk.size() > m_chunkScanPos) {
        if(Test("--")) {
            SkipComment();
        }
        else if(Test("\r\n") || Test("\n\r")) {
            Next(2);
            m_line += 1;
        }
        else if(IsNewLine(m_chunk[m_chunkScanPos])) {
            Next(1);
            m_line += 1;
        }
        else if(IsWhiteSpace(m_chunk[m_chunkScanPos])) {
            Next(1);
        }
        else {
            break;
        }
    }
}

bool Lexer::Test(const std::string& prefix) const {
    return m_chunk.compare(m_chunkScanPos, prefix.size(), prefix) == 0;
}

void Lexer::Next(int n) {
    m_chunkScanPos += n;
}

void Lexer::SkipComment() {
    Next(2);    // skip "--"
    if(Test("[")) {     // long comment ?
        // find the right bracket
        while(m_chunk.size() > m_chunkScanPos && !Test("]")) {
            Next(1);
        }
        if(m_chunk.size() > m_chunkScanPos) {
            Next(1);
        }
    }
    // short comment
    while(m_chunk.size() > m_chunkScanPos && !IsNewLine(m_chunk[m_chunkScanPos])) {
        Next(1);
    }
}

bool Lexer::IsNewLine(char chr) {
    return chr == '\r' || chr == '\n';
}

bool Lexer::IsWhiteSpace(char chr) {
    switch (chr) {
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

