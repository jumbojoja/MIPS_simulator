#pragma once
#ifndef FLOAT_H
#define FLOAT_H

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;
// 获取浮点数的符号位、指数部分和尾数部分
void extractComponents(uint32_t bits, int &sign, int &exponent,
                       uint32_t &fraction);

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

string To_anyscale_string(
    int x, int scale, int num_of_str,
    int character =
        0); // 十进制转二进制(数，进制，位数，是否符号扩展(默认不扩展))

long int To_decimal_int(
    string source, int base,
    int symbol =
        1); // 将任意进制的字符串转换为10进制的整数，base为该字符串的进制,symbol
            // = 1为有符号数
// 将单精度浮点数，转换为二进制
string Float_To_Binary_ByMemory(float num);
// 将二进制的数据，转换为float浮点数
float Binary_To_float(string str);
#endif
