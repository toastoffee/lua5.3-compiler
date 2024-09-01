cmake_minimum_required(VERSION 3.27)
project(lua5_3_compiler)

set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src)

add_executable(lua5_3_compiler main.cpp
        src/types.hpp
        src/lexer/token.hpp
        src/lexer/lexer.hpp
        src/lexer/lexer.cpp
        src/parser/block_parser.cpp
        src/parser/block_parser.hpp
        src/parser/statement_parser.cpp
        src/parser/statement_parser.hpp
        src/parser/parser_nodes.hpp
)
