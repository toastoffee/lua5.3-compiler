/**
  ******************************************************************************
  * @file           : lua_number.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/5
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_LUA_NUMBER_HPP
#define LUA5_3_COMPILER_LUA_NUMBER_HPP

#include <types.hpp>
#include <string>

class LuaNumber {
public:
    static i64* ParseInteger(const std::string& s);
    static f64* ParseFloat(const std::string& s);

    static bool IsInteger(f64 d);
};


#endif //LUA5_3_COMPILER_LUA_NUMBER_HPP
