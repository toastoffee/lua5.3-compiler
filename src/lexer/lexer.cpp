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
#include <iostream>

std::regex Lexer::s_regexNewLine(R"(\r\n|\n\r|\n|\r)");
std::regex Lexer::s_regexShortStr(R"((^'(\\\\|\\'|\\\n|\\z\s*|[^'\n])*')|(^"(\\\\|\\"|\\\n|\\z\s*|[^"\n])*"))");
std::regex Lexer::s_regexNumber(R"(^0[xX][0-9a-fA-F]*(\.[0-9a-fA-F]*)?([pP][+\-]?[0-9]+)?|^[0-9]*(\.[0-9]*)?([eE][+\-]?[0-9]+)?)");
std::regex Lexer::s_regexIdentifier(R"(^[_\d\w]+)");
std::regex Lexer::s_regexOpeningLongBracket(R"(^\[=*\[)");

std::regex Lexer::s_regexDecEscapeSeq(R"(^\\[0-9]{1,3})");
std::regex Lexer::s_regexHexEscapeSeq(R"(^\\x[0-9a-fA-F]{2})");
std::regex Lexer::s_regexUnicodeEscapeSeq(R"(^\\u\{[0-9a-fA-F]+\})");


Lexer::Lexer(std::string chunk, std::string chunkName) :
    m_chunk(chunk),
    m_chunkName(chunkName),
    m_chunkScanPos(0),
    m_line(1),
    m_nextToken({.line = 0, .id = TokenId::TOKEN_EOF, .tokenStr = ""})
    { }


Token Lexer::NextToken() {

    // is next token is recorded, then read it and clear it
    if(m_nextToken.line > 0) {

        // set the line status as the next token
        m_line = m_nextToken.line;

        // clear the next token
        m_nextToken.line = 0;

        // return next token
        return m_nextToken;
    }

    skipBlankSpaces();
    if(unscannedSize() == 0) {
        return Token {.line = m_line, .id = TokenId::TOKEN_EOF, .tokenStr="EOF"};
    }

    switch(unscannedChunk()[0]) {
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

    char c = unscannedChunk()[0];
    // recognize digits
    if(c == '.' || isDigit(c)) {
        return {.line = m_line, .id = TokenId::TOKEN_NUMBER, .tokenStr=scanNumber()};
    }
    if(c == '_' || isAlpha(c)) {
        auto tokenStr = scanIdentifier();

        // is lua keyword:
        if(keywords.find(tokenStr) != keywords.end()) {
            return {.line = m_line, .id = keywords[tokenStr], .tokenStr=tokenStr};
        } else {
            return {.line = m_line, .id = TokenId::TOKEN_IDENTIFIER, .tokenStr=tokenStr};
        }
    }

    assert(false && "unexpected symbol!");
}

Token Lexer::NextTokenOfId(TokenId id) {
    auto token = NextToken();
    assert(token.id == id && "syntax error!");
    return token;
}

Token Lexer::NextIdentifier() {
    return NextTokenOfId(TokenId::TOKEN_IDENTIFIER);
}

Token Lexer::LookAhead() {
    if(m_nextToken.line > 0) {
        return m_nextToken;
    }

    // record current stat, (DON'T RESET THE SCAN POS)
    int currentLine = m_line;

    // get next token
    m_nextToken = NextToken();

    // reset the stat as before
    m_line = currentLine;

    return m_nextToken;
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
        else if(isNewLine(unscannedChunk()[0])) {
            next(1);
            m_line += 1;
        }
        else if(isWhiteSpace(unscannedChunk()[0])) {
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
        if(std::regex_search(unscannedChunk(), s_regexOpeningLongBracket)) {
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
    return c >= '0' && c <= '9';
}

std::string Lexer::scanLongString() {
    std::string openingLongBracket = findOpeningLongBracket(unscannedChunk());
    assert(!openingLongBracket.empty() && "invalid long string delimiter");

    std::string closingLongBracket = openingLongBracket;
    closingLongBracket[0] = ']';
    closingLongBracket[closingLongBracket.size()-1] = ']';

    int closingLongBracketIdx = unscannedChunk().find(closingLongBracket);
    assert(closingLongBracketIdx >= 0 && "unfinished long string or comment");

    std::string str = strSect(unscannedChunk(), openingLongBracket.size(), closingLongBracketIdx);
    next(closingLongBracketIdx + closingLongBracket.size());

    str = std::regex_replace(str, s_regexNewLine, "\n");
    for (char c : str) {
        if(c == '\n') {
            ++m_line;
        }
    }

    if(str.size() > 0 && str[0] == '\n') {
        str = str.substr(1);
    }

    return str;
}

std::string Lexer::scanShortString() {
    std::smatch matches;

    auto s = unscannedChunk();

    if(std::regex_search(unscannedChunk(), matches, s_regexShortStr)) {
        auto str = matches[0].str();
        next(str.size());
        str = strSect(str, 1, str.size()-1);
        if(str.find('\\') >= 0) {
            std::regex_search(str, matches, s_regexNewLine);
            m_line += matches.size();

            str = escape(str);
        }
        return str;
    }
    std::cout << "unfinished string" << std::endl;
    return "";
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

std::string Lexer::escape(std::string s) {
    std::string ret;

    std::smatch matches;

    while(!s.empty()) {
        if(s[0] != '\\') {
            ret += s[0];
            s = s.substr(1);
            continue;
        }
        if(s.size() == 1) {
            std::cout << "unfinished string" << std::endl;
        }
        switch (s[1]) {
            case 'a': ret += '\a'; s = s.substr(2);
                continue;
            case 'b': ret += '\b'; s = s.substr(2);
                continue;
            case 'f': ret += '\f'; s = s.substr(2);
                continue;
            case 'n': ret += '\n'; s = s.substr(2);
                continue;
            case '\n': ret += '\n'; s = s.substr(2);
                continue;
            case 'r': ret += '\r'; s = s.substr(2);
                continue;
            case 't': ret += '\t'; s = s.substr(2);
                continue;
            case 'v': ret += '\v'; s = s.substr(2);
                continue;
            case '"': ret += '"'; s = s.substr(2);
                continue;
            case '\'': ret += '\''; s = s.substr(2);
                continue;
            case '\\': ret += '\\'; s = s.substr(2);
                continue;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':   // \ddd
                if(std::regex_search(s, matches, s_regexDecEscapeSeq)) {
                    auto decStr = matches[0].str().substr(1);
                    int d = std::stoi(decStr, nullptr, 10);

                    if(d <= 0xFF) {
                        ret += (char)d;
                        s = s.substr(matches[0].str().size());
                        continue;
                    }
                    assert(false && "decimal escape too large!");
                }
                continue;
            case 'x': // ... \xXX
                if(std::regex_search(s, matches, s_regexHexEscapeSeq)) {
                    auto hexStr = '0' + matches[0].str().substr(1);
                    int d = std::stoi(hexStr, nullptr, 16);
                    ret += (char)d;
                    s = s.substr(matches[0].str().size());
                    continue;
                }
                continue;
            case 'u': // ... \u{XXX}
                if(std::regex_search(s, matches, s_regexUnicodeEscapeSeq)) {
                    auto unicodeStr = strSect(matches[0].str(), 3, matches[0].str().size() - 1);
                    int d = std::stoi(unicodeStr, nullptr, 16);

                    if(d <= 0x10ffff) {
                        ret += (char)d;
                        s = s.substr(matches[0].str().size());
                        continue;
                    }
                    assert(false && "UTF-8 value too large!");
                }
                continue;
            case 'z': // ...
                s = s.substr(2);
                while(!s.empty() && isWhiteSpace(s[0])) {
                    s = s.substr(1);
                }
                continue;
        }
        assert(false && "invalid escape sequence!");
    }

    return ret;
}

std::string Lexer::scanNumber() {
    return scan(s_regexNumber);
}

std::string Lexer::scan(const std::regex& regex) {
    std::smatch matches;
    auto s = unscannedChunk();
    auto c = s[0];
    if(std::regex_search(s, matches, regex)) {
        next(matches[0].str().size());
        return matches[0].str();
    }
    assert(false && "unreachable!");
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

std::string Lexer::scanIdentifier() {
    return scan(s_regexIdentifier);
}

std::string Lexer::strSect(const std::string& s, int start, int end) {
    return s.substr(start, end - start);
}

