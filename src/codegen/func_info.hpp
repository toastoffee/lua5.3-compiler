/**
  ******************************************************************************
  * @file           : func_info.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/7
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_FUNC_INFO_HPP
#define LUA5_3_COMPILER_FUNC_INFO_HPP

#include <types.hpp>
#include <lua_constants.hpp>

#include <map>

struct FuncInfo {
    std::map<LuaConstant, int> constants;
};


#endif //LUA5_3_COMPILER_FUNC_INFO_HPP
