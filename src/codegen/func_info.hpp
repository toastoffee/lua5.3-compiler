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

class FuncInfo {
private:
    std::map<LuaConstant *, int> m_constants;
    int                          m_usedRegs;
    int                          m_maxRegs;


public:
    int IndexOfConstant(LuaConstant* k);

    int AllocReg();
    void FreeReg();
    int AllocRegs(int n);
    void FreeRegs(int n);
};


#endif //LUA5_3_COMPILER_FUNC_INFO_HPP
