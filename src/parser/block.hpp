/**
  ******************************************************************************
  * @file           : block.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_BLOCK_HPP
#define LUA5_3_COMPILER_BLOCK_HPP

// chunk := block
// block := {stat} [retstat]    ( {}:any times, []:0 or 1 time )
// retstat := return [explist] [';']
// explist := exp {',' exp }

//struct Block {
//    int lastLine;
//    Stat[] stats;
//    Exp[] retExps;
//};

#endif //LUA5_3_COMPILER_BLOCK_HPP
