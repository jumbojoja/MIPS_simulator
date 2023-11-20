#pragma once
#ifndef MIPS_SIMULATOR_H
#define MIPS_SIMULATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <fstream>
#include <unordered_map>

#define REGISTER_SIZE 32
#define MEMORY_SIZE 1024

class MIPS_Simulator {
private:
    std::vector<unsigned int> registers;   // �洢�Ĵ���������
    std::vector<unsigned int> memory;      // �洢�ڴ������
    int pc;                                // �����������ָ��ǰָ��ĵ�ַ
    int dataStart;                         // ���ݶε���ʼ��ַ
    int stackTop;                          // ջ����ַ
    int totalInstructionCount;             // ָ������

protected:
    // ������ִ��ָ��
    void parseAndExecute(const std::string& code);
    void executed();  // ִ����ָ���Ĳ���

    // ջ����ָ��
    void pushStack(int value);
    int popStack();
    void functionPrologue();  // ��������ǰ�Ĳ���
    void functionEpilogue();  // �����˳�ǰ�Ĳ���

    // �޸ļĴ�����ֵ
    void setRegister(int reg, int value);

    // ����ָ���ʵ��
    void add(const std::string& rs, const std::string& rt, const std::string& rd);
    void andOp(const std::string& rs, const std::string& rt, const std::string& rd);
    void sub(const std::string& rs, const std::string& rt, const std::string& rd);
    void orOp(const std::string& rs, const std::string& rt, const std::string& rd);
    void nor(const std::string& rs, const std::string& rt, const std::string& rd);
    void addi(const std::string& rs, const std::string& rt, const std::string& imm);
    void andi(const std::string& rs, const std::string& rt, const std::string& imm);
    void ori(const std::string& rs, const std::string& rt, const std::string& imm);
    void beq(const std::string& rs, const std::string& rt, const std::string& offset);
    void bne(const std::string& rs, const std::string& rt, const std::string& offset);
    void sll(const std::string& rt, const std::string& rd, const std::string& sa);
    void srl(const std::string& rt, const std::string& rd, const std::string& sa);
    void jal(const std::string& target);
    void jr(const std::string& rs);

    // ����ת��
    double _binary_decimal(const std::string& operand);

public:
    MIPS_Simulator();  // ���캯��

    // ������һ��ָ��
    void executeNextInstruction();

    // ���ָ���Ƿ�ȫ��ִ�����
    bool isExecutionFinished() const;

    // ����ָ��
    bool loadInstructions(int instruction);
    bool loadInstructions(const std::string& filePath);

    // ��ȡ�Ĵ�����ֵ
    int getRegister(int reg) const;

    // ��ȡ���мĴ�����ֵ
    std::vector<unsigned int> getRegisters();

    // ��ȡ�����ڴ��ֵ
    std::vector<unsigned int> getMemories();

    // ��ӡ�Ĵ�����ֵ
    void printRegisters();

    // ��ӡ�ڴ��ֵ
    void printMemory();
};

#endif
