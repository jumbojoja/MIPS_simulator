#include <iostream>
#include <cstdint>

// ��������ת��Ϊ IEEE 754 ��ʽ
uint32_t floatToIEEE754(float num) {
    uint32_t bits;
    std::memcpy(&bits, &num, sizeof(bits));
    return bits;
}

// �� IEEE 754 ��ʽת��Ϊ������
float IEEE754ToFloat(uint32_t bits) {
    float num;
    std::memcpy(&num, &bits, sizeof(num));
    return num;
}

// �������ӷ�
uint32_t floatAddition(uint32_t aBits, uint32_t bBits) {
    float a = IEEE754ToFloat(aBits);
    float b = IEEE754ToFloat(bBits);

    // ��ָ�����е�����ʹ�������
    int exponentDiff = std::frexpf(a) - std::frexpf(b);
    if (exponentDiff > 0) {
        b = std::scalbnf(b, exponentDiff);
    }
    else {
        a = std::scalbnf(a, -exponentDiff);
    }

    // ִ�мӷ�
    return floatToIEEE754(a + b);
}

// ����������
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits) {
    float b = IEEE754ToFloat(bBits);

    // �� b �ķ���λȡ��
    bBits ^= 0x80000000;

    // ִ�мӷ�
    return floatAddition(aBits, bBits);
}

int main() {
    float a = 3.14;
    float b = 1.5;

    // ��������ת��Ϊ IEEE 754 ��ʽ
    uint32_t aBits = floatToIEEE754(a);
    uint32_t bBits = floatToIEEE754(b);

    // ִ�и������ӷ��ͼ���
    uint32_t sumBits = floatAddition(aBits, bBits);
    uint32_t differenceBits = floatSubtraction(aBits, bBits);

    // �����ת���ظ�����
    float sum = IEEE754ToFloat(sumBits);
    float difference = IEEE754ToFloat(differenceBits);

    // ��ӡ���
    std::cout << "a + b: " << sum << std::endl;
    std::cout << "a - b: " << difference << std::endl;

    return 0;
}
