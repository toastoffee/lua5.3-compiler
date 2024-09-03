#include <iostream>

#include <lexer/lexer.hpp>
#include <parser/ast_nodes.hpp>

struct A {
    virtual ~A() = default;
    int a;
};

struct B {
    virtual ~B() = default;
    int b;
};

int main() {

    // load file
    FILE* file = fopen("../lua_tests/closure.lua", "rb");

    assert(file && "failed to open file!");

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = new char[filesize];
    fread(source, sizeof(char), filesize, file);
    fclose(file);

    std::string sourceCode(source);


    // test lexer
    Lexer::TestLexer(sourceCode, "max.lua");

    return 0;
}
