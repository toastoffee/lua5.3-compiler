/**
  ******************************************************************************
  * @file           : opcodes.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/9
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_OPCODES_HPP
#define LUA5_3_COMPILER_OPCODES_HPP


const int OP_MOVE       = 0;
const int OP_LOADK      = 1;
const int OP_LOADKX     = 2;
const int OP_LOADBOOL   = 3;
const int OP_LOADNIL    = 4;
const int OP_GETUPVAL   = 5;
const int OP_GETTABUP   = 6;
const int OP_GETTABLE   = 7;
const int OP_SETTABUP   = 8;
const int OP_SETUPVAL   = 9;
const int OP_SETTABLE   = 10;
const int OP_NEWTABLE   = 11;
const int OP_SELF       = 12;
const int OP_ADD        = 13;
const int OP_SUB        = 14;
const int OP_MUL        = 15;
const int OP_MOD        = 16;
const int OP_POW        = 17;
const int OP_DIV        = 18;
const int OP_IDIV       = 19;
const int OP_BAND       = 20;
const int OP_BOR        = 21;
const int OP_BXOR       = 22;
const int OP_SHL        = 23;
const int OP_SHR        = 24;
const int OP_UNM        = 25;
const int OP_BNOT       = 26;
const int OP_NOT        = 27;
const int OP_LEN        = 28;
const int OP_CONCAT     = 29;
const int OP_JMP        = 30;
const int OP_EQ         = 31;
const int OP_LT         = 32;
const int OP_LE         = 33;
const int OP_TEST       = 34;
const int OP_TESTSET    = 35;
const int OP_CALL       = 36;
const int OP_TAILCALL   = 37;
const int OP_RETURN     = 38;
const int OP_FORLOOP    = 39;
const int OP_FORPREP    = 40;
const int OP_TFORCALL   = 41;
const int OP_TFORLOOP   = 42;
const int OP_SETLIST    = 43;
const int OP_CLOSURE    = 44;
const int OP_VARARG     = 45;
const int OP_EXTRAARG   = 46;


#endif //LUA5_3_COMPILER_OPCODES_HPP
