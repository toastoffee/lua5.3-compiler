/**
  ******************************************************************************
  * @file           : lua_number.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/5
  ******************************************************************************
  */



#include "lua_number.hpp"

i64 LuaNumber::ParseInteger(const std::string& s) {
    try {
        i64 n = std::stoi(s);
        return n;
    } catch (std::invalid_argument &e) {
        assert(false && e.what());
    }
}

f64 LuaNumber::ParseFloat(const std::string& s) {
    try {
        f64 d = std::stod(s);
        return d;
    } catch (std::invalid_argument &e) {
        assert(false && e.what());
    }
}
