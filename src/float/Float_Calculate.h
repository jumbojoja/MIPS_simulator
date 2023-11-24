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
// ��ȡ�������ķ���λ��ָ�����ֺ�β������
void extractComponents(uint32_t bits, int &sign, int &exponent,
                       uint32_t &fraction);

// ������λ��ָ�����ֺ�β��������װ�� IEEE 754 ��ʽ
uint32_t combineComponents(int sign, int exponent, uint32_t fraction);

// �������ӷ�
uint32_t floatAddition(uint32_t aBits, uint32_t bBits);

// ����������
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits);
// ģ�������ӷ���ʹ���޷�������
unsigned int simulateIntegerAddition(unsigned int a, unsigned int b);

// ģ������������ʹ���޷�������
unsigned int simulateIntegerSubtraction(unsigned int a, unsigned int b);

string To_anyscale_string(
    int x, int scale, int num_of_str,
    int character =
        0); // ʮ����ת������(�������ƣ�λ�����Ƿ������չ(Ĭ�ϲ���չ))

long int To_decimal_int(
    string source, int base,
    int symbol =
        1); // ��������Ƶ��ַ���ת��Ϊ10���Ƶ�������baseΪ���ַ����Ľ���,symbol
            // = 1Ϊ�з�����
// �������ȸ�������ת��Ϊ������
string Float_To_Binary_ByMemory(float num);
// �������Ƶ����ݣ�ת��Ϊfloat������
float Binary_To_float(string str);
#endif
