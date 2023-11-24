#pragma once
#include <iostream>
#include <cstdint>
#ifndef FLOAT_H
#define FLOAT_H
// 获取浮点数的符号位、指数部分和尾数部分
void extractComponents(uint32_t bits, int& sign, int& exponent, uint32_t& fraction);

// 将符号位、指数部分和尾数部分组装成 IEEE 754 格式
uint32_t combineComponents(int sign, int exponent, uint32_t fraction);

// 浮点数加法
uint32_t floatAddition(uint32_t aBits, uint32_t bBits);


// 浮点数减法
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits);
// 模拟整数加法，使用无符号整数
unsigned int simulateIntegerAddition(unsigned int a, unsigned int b);

// 模拟整数减法，使用无符号整数
unsigned int simulateIntegerSubtraction(unsigned int a, unsigned int b);
#endif

