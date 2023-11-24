#include "Float_Calculate.h"

// ��ȡ�������ķ���λ��ָ�����ֺ�β������
void extractComponents(uint32_t bits, int& sign, int& exponent, uint32_t& fraction) {
    sign = (bits >> 31) & 0x1;
    exponent = ((bits >> 23) & 0xFF); // ����ָ����ƫ��
    fraction = (bits & 0x7FFFFF) | 0x800000; // ����ʡ�Ե� 1
}

// ������λ��ָ�����ֺ�β��������װ�� IEEE 754 ��ʽ
uint32_t combineComponents(int sign, int exponent, uint32_t fraction) {
    return (sign << 31) | (exponent << 23) | (fraction & 0x7FFFFF);
}

// �������ӷ�
uint32_t floatAddition(uint32_t aBits, uint32_t bBits) {
    int aSign, bSign;
    int aExponent, bExponent;
    uint32_t aFraction, bFraction;
    extractComponents(aBits, aSign, aExponent, aFraction);
    extractComponents(bBits, bSign, bExponent, bFraction);

    // ��ָ�����е�����ʹ�������
    int exponentDiff = aExponent - bExponent;
    if (exponentDiff > 0) {
        bFraction >>= exponentDiff;
        bExponent += exponentDiff;
    }
    else {
        aFraction >>= -exponentDiff;
        aExponent += -exponentDiff;
    }


    // ���ݷ���λִ�мӷ������
    uint32_t resultFraction;
    if (aSign == bSign) {
        resultFraction = aFraction + bFraction;
    }
    else {
        resultFraction = (aFraction > bFraction) ? (aFraction - bFraction) : (bFraction - aFraction);
    }

    // ����β�����ڵ��� 0x8fffff �����
    while (resultFraction >= 0xffffff) {
        resultFraction >>= 1;
        ++aExponent;
    }

    // ����β��С�ڵ��� 0x800000 �����
    while (resultFraction <= 0x800000) {
        resultFraction <<= 1;
        --aExponent;
    }

    // ȥ�������1
    resultFraction &= 0x7FFFFF;

    // ��װ���
    int resultSign = aSign;
    int resultExponent = aExponent;
    return combineComponents(resultSign, resultExponent, resultFraction);
}



// ����������
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits) {
    int bSign, bExponent;
    uint32_t bFraction;

    extractComponents(bBits, bSign, bExponent, bFraction);
    bSign ^= 0x1;

    return floatAddition(aBits, combineComponents(bSign, bExponent, bFraction));
}
// ģ�������ӷ���ʹ���޷�������
unsigned int simulateIntegerAddition(unsigned int a, unsigned int b) {
    // �޷��������ӷ������ʱ�������ģ���Ĳ���
    unsigned int sum = a + b;

    return sum;
}

// ģ������������ʹ���޷�������
unsigned int simulateIntegerSubtraction(unsigned int a, unsigned int b) {
    // ע�⣺������� a >= b��������Ҫ���н���

    // �޷����������������ʱ�������ģ���Ĳ���
    unsigned int difference = a - b;

    return difference;
}
