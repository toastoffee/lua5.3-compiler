cmake_minimum_required(VERSION 3.27)
project(lua5_3_compiler)

set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src)

add_executable(lua5_3_compiler main.cpp
        src/types.hpp
        src/lexer/token.hpp
        src/lexer/lexer.hpp
        src/lexer/lexer.cpp

        src/parser/parser.hpp
        src/parser/block_parser.cpp
        src/parser/statement_parser.cpp
        src/parser/ast_nodes.hpp
        src/parser/expression_parser.cpp
        src/parser/lua_number.cpp
        src/parser/lua_number.hpp
        src/parser/expression_optimize.cpp
)
