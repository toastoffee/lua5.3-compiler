#include <iostream>

#include <lexer/lexer.hpp>
#include <parser/ast_nodes.hpp>
#include <parser/parser.hpp>


int main() {

    // load file
    FILE* file = fopen("../lua_tests/variable.lua", "rb");

    assert(file && "failed to open file!");

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = new char[filesize];
    fread(source, sizeof(char), filesize, file);
    fclose(file);

    std::string sourceCode(source);


    // test lexer
//    Lexer::TestLexer(sourceCode, "max.lua");

    // test parser
//    auto block = Parser::Parse(sourceCode, "max.lua");


    std::map<int, int> testMap;
    testMap[1] = 1;
    std::cout << testMap[2];
    std::cout << testMap.size();
    std::cout << (testMap.find(3) == testMap.end());
    std::cout << (testMap.find(2) == testMap.end());

    return 0;
}
