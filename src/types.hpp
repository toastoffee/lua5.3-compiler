/**
  ******************************************************************************
  * @file           : types.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/8/27
  ******************************************************************************
  */



#ifndef LUA5_3_COMPILER_TYPES_HPP
#define LUA5_3_COMPILER_TYPES_HPP

#include <cstdint>

typedef uint8_t byte;

typedef char c8;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;


template<typename DecType, typename ValType>
bool isInstanceOf(ValType* instance) {
    return dynamic_cast<DecType*>(instance) != nullptr;
}



#endif //LUA5_3_COMPILER_TYPES_HPP
