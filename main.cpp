#include <iostream>

#include <lexer/lexer.hpp>


int main() {

    Lexer lexer("print(\"Hello,World!\")", "");

    lexer.NextToken();
    lexer.NextToken();
    lexer.NextToken();
    lexer.NextToken();
    lexer.NextToken();


    return 0;
}
