/**
  ******************************************************************************
  * @file           : lua_constants.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/7
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_LUA_CONSTANTS_HPP
#define LUA5_3_COMPILER_LUA_CONSTANTS_HPP

#include <types.hpp>

struct LuaConstant {
    virtual ~LuaConstant() = default;
};

struct LuaNil : LuaConstant {

};

struct LuaBool : LuaConstant {
    bool val;
};

struct LuaNumber : LuaConstant {
    i64 i;
    f64 d;
};

#endif //LUA5_3_COMPILER_LUA_CONSTANTS_HPP
