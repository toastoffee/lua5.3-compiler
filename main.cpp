#include <iostream>

#include <lexer/lexer.hpp>


int main() {

    std::string s = "print(\"Hello,World!\")";
    Lexer lexer(s, "");

    auto t1 = lexer.NextToken();
    lexer.LookAhead();
    auto t2 = lexer.NextToken();
    lexer.LookAhead();
    lexer.LookAhead();
    auto t3 = lexer.NextToken();
    auto t4 = lexer.NextToken();
    auto t5 = lexer.NextToken();

//    std::regex re(R"(^[_\d\w]+)");
//    auto b = std::regex_search(s, re);


    // abcde => 5
    // sect(abcde, 1, 4)

    return 0;
}
