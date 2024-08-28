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
    m_line = 1;
}

Token Lexer::NextToken() {

    return Token{.line = m_line};
}

void Lexer::SkipBlankSpaces() {
    while(!m_chunk.empty()) {
        if(Test("--")) {
            SkipComment();
        }
        else if(Test("\r\n") || Test("\n\r")) {
            Next(2);
            m_line += 1;
        }
        else if(IsNewLine(m_chunk[0])) {
            Next(1);
            m_line += 1;
        }
        else if(IsWhiteSpace(m_chunk[0])) {
            Next(1);
        }
        else {
            break;
        }
    }
}

bool Lexer::Test(const std::string& prefix) const {
    return m_chunk.compare(0, prefix.size(), prefix) == 0;
}

void Lexer::Next(int n) {

}

void Lexer::SkipComment() {

}

bool Lexer::IsNewLine(char chr) const {
    return false;
}

bool Lexer::IsWhiteSpace(char chr) const {
    return false;
}

