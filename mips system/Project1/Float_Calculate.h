#pragma once
#include <iostream>
#include <cstdint>
#ifndef FLOAT_H
#define FLOAT_H
// ��ȡ�������ķ���λ��ָ�����ֺ�β������
void extractComponents(uint32_t bits, int& sign, int& exponent, uint32_t& fraction);

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
#endif

