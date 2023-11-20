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
    std::vector<unsigned int> registers;   // 存储寄存器的容器
    std::vector<unsigned int> memory;      // 存储内存的容器
    int pc;                                // 程序计数器，指向当前指令的地址
    int dataStart;                         // 数据段的起始地址
    int stackTop;                          // 栈顶地址
    int totalInstructionCount;             // 指令总数

protected:
    // 解析并执行指令
    void parseAndExecute(const std::string& code);
    void executed();  // 执行完指令后的操作

    // 栈操作指令
    void pushStack(int value);
    int popStack();
    void functionPrologue();  // 函数进入前的操作
    void functionEpilogue();  // 函数退出前的操作

    // 修改寄存器的值
    void setRegister(int reg, int value);

    // 具体指令的实现
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

    // 进制转换
    double _binary_decimal(const std::string& operand);

public:
    MIPS_Simulator();  // 构造函数

    // 运行下一条指令
    void executeNextInstruction();

    // 检查指令是否全部执行完毕
    bool isExecutionFinished() const;

    // 加载指令
    bool loadInstructions(int instruction);
    bool loadInstructions(const std::string& filePath);

    // 获取寄存器的值
    int getRegister(int reg) const;

    // 获取所有寄存器的值
    std::vector<unsigned int> getRegisters();

    // 获取所有内存的值
    std::vector<unsigned int> getMemories();

    // 打印寄存器的值
    void printRegisters();

    // 打印内存的值
    void printMemory();
};

#endif
