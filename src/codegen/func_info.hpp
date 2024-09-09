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

#include <string>
#include <map>
#include <vector>

const int MAX_ARG_Bx = (1 << 18) - 1;
const int MAX_ARG_sBx = MAX_ARG_Bx >> 1;

struct LocVarInfo {
    std::string name;
    LocVarInfo* prev;
    int scopeLv;
    int slot;
    bool captured;
};

class FuncInfo {
private:
    std::map<LuaConstant *, int>        m_constants;
    int                                 m_usedRegs;
    int                                 m_maxRegs;
    int                                 m_scopeLv;
    std::vector<LocVarInfo *>           m_locVars;
    std::map<std::string, LocVarInfo *> m_locNames;
    std::vector<std::vector<int> *>     m_breaks;
    std::vector<int>                    m_insts;

public:
    int IndexOfConstant(LuaConstant* k);

    int AllocReg();
    void FreeReg();
    int AllocRegs(int n);
    void FreeRegs(int n);

    void EnterScope(bool breakable);
    int AddLocVar(const std::string& name);
    int SlotOfLocVar(const std::string& name);
    void ExitScope();
    void RemoveLocVar(LocVarInfo* locVar);
    void AddBreakJmp(int pc);

    int Pc();
    int GetJmpArgA();
};


#endif //LUA5_3_COMPILER_FUNC_INFO_HPP
