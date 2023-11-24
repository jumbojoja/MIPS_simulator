#include <iostream>
#include <cstdint>

// 将浮点数转换为 IEEE 754 格式
uint32_t floatToIEEE754(float num) {
    uint32_t bits;
    std::memcpy(&bits, &num, sizeof(bits));
    return bits;
}

// 将 IEEE 754 格式转换为浮点数
float IEEE754ToFloat(uint32_t bits) {
    float num;
    std::memcpy(&num, &bits, sizeof(num));
    return num;
}

// 浮点数加法
uint32_t floatAddition(uint32_t aBits, uint32_t bBits) {
    float a = IEEE754ToFloat(aBits);
    float b = IEEE754ToFloat(bBits);

    // 对指数进行调整，使它们相等
    int exponentDiff = std::frexpf(a) - std::frexpf(b);
    if (exponentDiff > 0) {
        b = std::scalbnf(b, exponentDiff);
    }
    else {
        a = std::scalbnf(a, -exponentDiff);
    }

    // 执行加法
    return floatToIEEE754(a + b);
}

// 浮点数减法
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits) {
    float b = IEEE754ToFloat(bBits);

    // 将 b 的符号位取反
    bBits ^= 0x80000000;

    // 执行加法
    return floatAddition(aBits, bBits);
}

int main() {
    float a = 3.14;
    float b = 1.5;

    // 将浮点数转换为 IEEE 754 格式
    uint32_t aBits = floatToIEEE754(a);
    uint32_t bBits = floatToIEEE754(b);

    // 执行浮点数加法和减法
    uint32_t sumBits = floatAddition(aBits, bBits);
    uint32_t differenceBits = floatSubtraction(aBits, bBits);

    // 将结果转换回浮点数
    float sum = IEEE754ToFloat(sumBits);
    float difference = IEEE754ToFloat(differenceBits);

    // 打印结果
    std::cout << "a + b: " << sum << std::endl;
    std::cout << "a - b: " << difference << std::endl;

    return 0;
}
