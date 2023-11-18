#include "MIPS_Simulator.h"

MIPS_Simulator::MIPS_Simulator() : registers(REGISTER_SIZE, 0), memory(MEMORY_SIZE, 0), pc(0) {
    // 初始化 dataStart 和 stackTop
    dataStart = MEMORY_SIZE / 2;  // 假设数据段从内存的一半开始
    stackTop = MEMORY_SIZE - 1;   // 栈从内存的末尾开始

    // 设置栈指针和帧指针
    registers[29] = stackTop * 4;  // $sp 栈指针
    registers[30] = stackTop * 4;  // $fp 帧指针
}

bool MIPS_Simulator::loadInstructions(int instruction) {
    // 直接将整数值存入内存
    memory[totalInstructionCount] = instruction;
    totalInstructionCount++;  // 更新指令计数
    return true;
}

bool MIPS_Simulator::loadInstructions(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 假设每一行包含一条二进制指令
        memory[totalInstructionCount] = std::bitset<32>(line).to_ulong();
        totalInstructionCount++;  // 更新指令计数
    }

    file.close();
    return true;
}

void MIPS_Simulator::executeNextInstruction() {
    // 检查 pc 是否越界
    if (pc >= 0 && pc < MEMORY_SIZE) {
        // 从内存获取二进制指令
        std::string instructionCode = std::bitset<32>(memory[pc / 4]).to_string();

        // 调用 parseAndExecute 执行指令
        parseAndExecute(instructionCode);

        // 执行完指令后更新 pc
        executed();
    }
}

void MIPS_Simulator::executed() {
    pc += 4;
}

bool MIPS_Simulator::isExecutionFinished() const {
    return pc/4 >= totalInstructionCount;
}

double MIPS_Simulator::_binary_decimal(const std::string& operand) {
    int length = operand.size();
    double result = 0;
    int power = 1;
    for (int i = length - 1; i >= 0; --i) {
        if (operand[i] == '1') {
            result += power;
        }
        power *= 2;
    }
    return result;
}

void MIPS_Simulator::parseAndExecute(const std::string& code) {
    std::string opcode(code, 0, 6); // 取得操作码
    std::string func(code, 26, 6);  // 取得函数码
    std::string op1(code, 0, 6);
    std::string op2(code, 6, 5);

    if (opcode == "000000" && func == "100000") {
        // 跳转 add 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        add(rs, rt, rd);
    }
    else if (opcode == "000000" && func == "100100") {
        // 跳转 and 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        andOp(rs, rt, rd);
    }
    else if (opcode == "000000" && func == "100010") {
        // 跳转 sub 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        sub(rs, rt, rd);
    }
    else if (opcode == "000000" && func == "100101") {
        // 跳转 or 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        orOp(rs, rt, rd);
    }
    else if (opcode == "000000" && func == "100111") {
        // 跳转 nor 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        nor(rs, rt, rd);
    }
    else if (opcode == "001000") {
        // 跳转 addi 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string immediate(code, 16, 16);
        addi(rs, rt, immediate);
    }
    else if (opcode == "001100") {
        // 跳转 andi 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string immediate(code, 16, 16);
        andi(rs, rt, immediate);
    }
    else if (opcode == "001101") {
        // 跳转 ori 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string immediate(code, 16, 16);
        ori(rs, rt, immediate);
    }
    else if (opcode == "000100") {
        // 跳转 beq 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string offset(code, 16, 16);
        beq(rs, rt, offset);
    }
    else if (opcode == "000101") {
        // 跳转 bne 的代码实现
        std::string rs(code, 6, 5);
        std::string rt(code, 11, 5);
        std::string offset(code, 16, 16);
        bne(rs, rt, offset);
    }
    else if (op1 == "000000" && op2 == "00000" && func == "000000") {
        // 跳转 sll 的代码实现
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        std::string sa(code, 21, 5);
        sll(rt, rd, sa);
    }
    else if (op1 == "000000" && op2 == "00000" && func == "000010") {
        // 跳转 srl 的代码实现
        std::string rt(code, 11, 5);
        std::string rd(code, 16, 5);
        std::string sa(code, 21, 5);
        srl(rt, rd, sa);
    }
    else if (opcode == "000011") {
        // 跳转 jal 的代码实现
        std::string target(code, 6, 26);
        jal(target);
    }
    else if (opcode == "000000" && func == "001000") {
        // 跳转 jr 的代码实现
        std::string rs(code, 6, 5);
        jr(rs);
    }
    else {
        std::cout << "unknown command" << std::endl;
    }
}

int MIPS_Simulator::getRegister(int reg) const {
    return registers[reg];
}

void MIPS_Simulator::setRegister(int reg, int value) {
    registers[reg] = value;
}

void MIPS_Simulator::printRegisters() {
    std::cout << "Registers:" << std::endl;
    for (int i = 0; i < registers.size(); ++i) {
        std::cout << "$" << i << ": " << registers[i] << "\t";
        if ((i + 1) % 8 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void MIPS_Simulator::printMemory() {
    std::cout << "Memory:" << std::endl;

    for (int i = 0; i < MEMORY_SIZE; ++i) {
        std::cout << "[" << i << "]: " << memory[i] << "\t";
        if ((i + 1) % 8 == 0) {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}

std::vector<unsigned int> MIPS_Simulator::getRegisters() {
    return registers;
}

std::vector<unsigned int> MIPS_Simulator::getMemories() {
    return memory;
}

void MIPS_Simulator::pushStack(int value) {
    // 将值压入堆栈
    memory[registers[29] / 4] = value;
    // 更新堆栈指针
    registers[29] -= 4;
}

int MIPS_Simulator::popStack() {
    // 更新堆栈指针
    registers[29] += 4;
    // 弹出堆栈的值
    int value = memory[registers[29] / 4];
    return value;
}

void MIPS_Simulator::functionPrologue() {
    // 保存 $ra 寄存器到栈上
    pushStack(registers[31]);

    // 保存 $fp 寄存器到栈上
    pushStack(registers[30]);

    // 保存 $s0-$s7 寄存器到栈上
    for (int i = 16; i <= 23; ++i) {
        pushStack(registers[i]);
    }

    // 设置帧指针
    registers[30] = registers[29];
}

void MIPS_Simulator::functionEpilogue() {
    // 恢复 $s7-$s0 寄存器
    for (int i = 23; i >= 16; --i) {
        registers[i] = popStack();
    }

    // 恢复 $fp 寄存器
    registers[30] = popStack();

    // 恢复 $ra 寄存器
    registers[31] = popStack();
}

int main() {
    MIPS_Simulator simulator;

    // Sample instructions
    // Load instructions into memory
    simulator.loadInstructions(0x20100001);  // addi $s0, $zero, 1
    simulator.loadInstructions(0x20110002);  // addi $s1, $zero, 2
    simulator.loadInstructions(0x20120003);  // addi $s2, $zero, 3
    simulator.loadInstructions(0x20130004);  // addi $s3, $zero, 4
    simulator.loadInstructions(0x20380040);  // addi $t8, $zero, 64
    simulator.loadInstructions(0x00004020);  // add $t0, $zero, $zero
    simulator.loadInstructions(0x00004820);  // add $t1, $zero, $zero
    simulator.loadInstructions(0x312900FF);  // andi $t1, $t1, 255
    simulator.loadInstructions(0x01295020);  // add $t1, $t1, $t2
    simulator.loadInstructions(0x012A4825);  // or $t1, $t1, $t2
    simulator.loadInstructions(0x35290080);  // ori $t1, $t1, 256
    simulator.loadInstructions(0x012A4822);  // sub $t1, $t1, $t2
    simulator.loadInstructions(0x02509022);  // sub $s2, $s2, $s0
    simulator.loadInstructions(0x012A4827);  // nor $t1, $t1, $t2
    simulator.loadInstructions(0x012A4824);  // and $t1, $t1, $t2
    simulator.loadInstructions(0x0C000016);  // jal func
    simulator.loadInstructions(0x000A5880);  // sll $t3, $t2, 2
    simulator.loadInstructions(0x000A5842);  // srl $t3, $t2, 1
    simulator.loadInstructions(0x1512FFF5);  // bne $t0, $s2, loop
    simulator.loadInstructions(0x02609824);  // and $s3, $s3, $zero
    simulator.loadInstructions(0x11130006);  // beq $t0, $s3, end
    simulator.loadInstructions(0x21080001);  // addi $t0, $t0, 1
    simulator.loadInstructions(0x200C0001);  // addi $t4, $zero, 1
    simulator.loadInstructions(0x018D6022);  // sub $t4, $t4, $t5
    simulator.loadInstructions(0x000C6280);  // sll $t4, $t4, 10
    simulator.loadInstructions(0x03E00008);  // jr $ra
    simulator.loadInstructions(0x200D0001);  // addi $t5, $zero, 1
    simulator.loadInstructions(0x000D6A80);  // sll $t5, $t5, 10
    simulator.printMemory();
    // Execute instructions using while loop
    while (!simulator.isExecutionFinished()) {
        simulator.executeNextInstruction();
        simulator.printRegisters();
        //simulator.printMemory();
    }

    return 0;
}
