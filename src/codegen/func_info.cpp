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
