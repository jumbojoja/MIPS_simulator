#include "Float_Calculate.h"

// 获取浮点数的符号位、指数部分和尾数部分
void extractComponents(uint32_t bits, int& sign, int& exponent, uint32_t& fraction) {
    sign = (bits >> 31) & 0x1;
    exponent = ((bits >> 23) & 0xFF); // 调整指数的偏移
    fraction = (bits & 0x7FFFFF) | 0x800000; // 加上省略的 1
}

// 将符号位、指数部分和尾数部分组装成 IEEE 754 格式
uint32_t combineComponents(int sign, int exponent, uint32_t fraction) {
    return (sign << 31) | (exponent << 23) | (fraction & 0x7FFFFF);
}

// 浮点数加法
uint32_t floatAddition(uint32_t aBits, uint32_t bBits) {
    int aSign, bSign;
    int aExponent, bExponent;
    uint32_t aFraction, bFraction;
    extractComponents(aBits, aSign, aExponent, aFraction);
    extractComponents(bBits, bSign, bExponent, bFraction);

    // 对指数进行调整，使它们相等
    int exponentDiff = aExponent - bExponent;
    if (exponentDiff > 0) {
        bFraction >>= exponentDiff;
        bExponent += exponentDiff;
    }
    else {
        aFraction >>= -exponentDiff;
        aExponent += -exponentDiff;
    }


    // 根据符号位执行加法或减法
    uint32_t resultFraction;
    if (aSign == bSign) {
        resultFraction = aFraction + bFraction;
    }
    else {
        resultFraction = (aFraction > bFraction) ? (aFraction - bFraction) : (bFraction - aFraction);
    }

    // 处理尾数大于等于 0x8fffff 的情况
    while (resultFraction >= 0xffffff) {
        resultFraction >>= 1;
        ++aExponent;
    }

    // 处理尾数小于等于 0x800000 的情况
    while (resultFraction <= 0x800000) {
        resultFraction <<= 1;
        --aExponent;
    }

    // 去掉多余的1
    resultFraction &= 0x7FFFFF;

    // 组装结果
    int resultSign = aSign;
    int resultExponent = aExponent;
    return combineComponents(resultSign, resultExponent, resultFraction);
}



// 浮点数减法
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits) {
    int bSign, bExponent;
    uint32_t bFraction;

    extractComponents(bBits, bSign, bExponent, bFraction);
    bSign ^= 0x1;

    return floatAddition(aBits, combineComponents(bSign, bExponent, bFraction));
}
// 模拟整数加法，使用无符号整数
unsigned int simulateIntegerAddition(unsigned int a, unsigned int b) {
    // 无符号整数加法，溢出时结果会是模数的补码
    unsigned int sum = a + b;

    return sum;
}

// 模拟整数减法，使用无符号整数
unsigned int simulateIntegerSubtraction(unsigned int a, unsigned int b) {
    // 注意：这里假设 a >= b，否则需要进行交换

    // 无符号整数减法，溢出时结果会是模数的补码
    unsigned int difference = a - b;

    return difference;
}
