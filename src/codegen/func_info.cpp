/**
  ******************************************************************************
  * @file           : func_info.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/9
  ******************************************************************************
  */

#include "func_info.hpp"
#include "opcodes.hpp"

int FuncInfo::IndexOfConstant(LuaConstant *k) {
    if(m_constants.find(k) == m_constants.end()) {
        int idx = m_constants.size();
        m_constants[k] = idx;
        return idx;
    } else {
        return m_constants[k];
    }
}

int FuncInfo::AllocReg() {
    m_usedRegs++;
    if (m_usedRegs >= 255) {
        assert(false && "function or expression needs too many registers");
    }

    if(m_usedRegs > m_maxRegs) {
        m_maxRegs = m_usedRegs;
    }

    return m_usedRegs - 1;
}

void FuncInfo::FreeReg() {
    m_usedRegs--;
}

int FuncInfo::AllocRegs(int n) {
    for (int i = 0; i < n; ++i) {
        AllocReg();
    }
    return m_usedRegs - n;
}

void FuncInfo::FreeRegs(int n) {
    for (int i = 0; i < n; ++i) {
        FreeReg();
    }
}

void FuncInfo::EnterScope(bool breakable) {
    m_scopeLv++;
    if (breakable) {
        m_breaks.push_back(new std::vector<int>);
    } else {
        m_breaks.push_back(nullptr);
    }
}

int FuncInfo::AddLocVar(const std::string& name) {
    auto newVar = new LocVarInfo {
        .name = name,
        .prev = m_locNames[name],
        .scopeLv = m_scopeLv,
        .slot = AllocReg()
    };

    m_locVars.push_back(newVar);
    m_locNames[name] = newVar;

    return newVar->slot;
}

int FuncInfo::SlotOfLocVar(const std::string& name) {
    if(m_locNames.find(name) != m_locNames.end()) {
        return m_locNames[name]->slot;
    } else {
        return -1;
    }
}

void FuncInfo::ExitScope() {
    auto pendingBreakJmps = m_breaks.back();
    m_breaks.pop_back();

    if(pendingBreakJmps != nullptr) {
        int a = GetJmpArgA();
        for (int pc : *pendingBreakJmps) {
            int sBx = Pc() - pc;
            int i = (sBx + MAX_ARG_sBx) << 14 | a << 6 | OP_JMP;
            m_insts[pc] = i;
        }
    }
    
    m_scopeLv--;
    for (const auto& pair : m_locNames) {
        if(pair.second->scopeLv > m_scopeLv) {  // exit scope
            RemoveLocVar(pair.second);
        }
    }
}

void FuncInfo::RemoveLocVar(LocVarInfo *locVar) {
    // unbind local variable, recycle register
    FreeReg();
    if(locVar->prev == nullptr) {
        m_locNames.erase(locVar->name);
    } else if(locVar->prev->scopeLv == locVar->scopeLv) {
        RemoveLocVar(locVar->prev);
    } else {
        m_locNames[locVar->name] = locVar->prev;
    }
}

void FuncInfo::AddBreakJmp(int pc) {
    for (int i = m_scopeLv; i >= 0; --i) {
        if (m_breaks[i] != nullptr) {   // 循环块
            m_breaks[i]->push_back(pc);
            return;
        }
    }
    assert(false && "<break> at line ? is not inside a loop!");
}
