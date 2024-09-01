/**
  ******************************************************************************
  * @file           : block_parser.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/1
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_BLOCK_PARSER_HPP
#define LUA5_3_COMPILER_BLOCK_PARSER_HPP

// chunk := block
// block := {stat} [retstat]    ( {}:any times, []:0 or 1 time )
// retstat := return [explist] [';']
// explist := exp {',' exp }



class block_parser {

};


#endif //LUA5_3_COMPILER_BLOCK_PARSER_HPP
