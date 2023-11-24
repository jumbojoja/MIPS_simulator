#include "MIPS_Simulator.h"


// 将寄存器字符串映射到编号的函数
uint32_t regToInt(const std::string& reg) {
    static const std::unordered_map<std::string, uint32_t> registerMap = {
        {"00000", 0}, {"00001", 1}, {"00010", 2}, {"00011", 3},
        {"00100", 4}, {"00101", 5}, {"00110", 6}, {"00111", 7},
        {"01000", 8}, {"01001", 9}, {"01010", 10}, {"01011", 11},
        {"01100", 12}, {"01101", 13}, {"01110", 14}, {"01111", 15},
        {"10000", 16}, {"10001", 17}, {"10010", 18}, {"10011", 19},
        {"10100", 20}, {"10101", 21}, {"10110", 22}, {"10111", 23},
        {"11000", 24}, {"11001", 25}, {"11010", 26}, {"11011", 27},
        {"11100", 28}, {"11101", 29}, {"11110", 30}, {"11111", 31}
    };

    auto it = registerMap.find(reg);
    if (it != registerMap.end()) {
        return it->second;
    }
    else {
        std::cerr << "Error: Unknown register " << reg << std::endl;
        return -1; // 无效的寄存器
    }
}

// 将二进制字符串转换为十进制数字的函数
double binaryToDecimal(const std::string& binary) {
    int length = binary.size();
    double result = 0;
    int power = 1;

    for (int i = length - 1; i >= 0; --i) {
        if (binary[i] == '1') {
            result += power;
        }
        power *= 2;
    }

    return result;
}

// add - R 类型指令
void MIPS_Simulator::add(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value + rt_value);
    std::cout << "add " << rd << ", " << rs << ", " << rt << std::endl;
}

// and - R 类型指令
void MIPS_Simulator::andOp(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value & rt_value);
    std::cout << "and " << rd << ", " << rs << ", " << rt << std::endl;
}

// or - R 类型指令
void MIPS_Simulator::orOp(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value | rt_value);
    std::cout << "or " << rd << ", " << rs << ", " << rt << std::endl;
}

// sub - R 类型指令
void MIPS_Simulator::sub(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value - rt_value);
    std::cout << "sub " << rd << ", " << rs << ", " << rt << std::endl;
}

// nor - R 类型指令
void MIPS_Simulator::nor(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), ~(rs_value | rt_value));
    std::cout << "nor " << rd << ", " << rs << ", " << rt << std::endl;
}

// sll - R 类型指令
void MIPS_Simulator::sll(const std::string& rt, const std::string& rd, const std::string& sa) {
    uint32_t rt_value = getRegister(regToInt(rt));
    uint32_t sa_value = binaryToDecimal(sa);
    setRegister(regToInt(rd), rt_value << sa_value);
    std::cout << "sll " << rd << ", " << rt << ", " << sa << std::endl;
}

// srl - R 类型指令
void MIPS_Simulator::srl(const std::string& rt, const std::string& rd, const std::string& sa) {
    uint32_t rt_value = getRegister(regToInt(rt));
    uint32_t sa_value = binaryToDecimal(sa);
    setRegister(regToInt(rd), rt_value >> sa_value);
    std::cout << "srl " << rd << ", " << rt << ", " << sa << std::endl;
}

// addi - I 类型指令
void MIPS_Simulator::addi(const std::string& rs, const std::string& rt, const std::string& imm) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t imm_value = binaryToDecimal(imm);
    setRegister(regToInt(rt), rs_value + imm_value);
    std::cout << "addi " << rt << ", " << rs << ", " << imm << std::endl;
}

// andi - I 类型指令
void MIPS_Simulator::andi(const std::string& rs, const std::string& rt, const std::string& imm) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t imm_value = binaryToDecimal(imm);
    setRegister(regToInt(rt), rs_value & imm_value);
    std::cout << "andi " << rt << ", " << rs << ", " << imm << std::endl;
}

// ori - I 类型指令
void MIPS_Simulator::ori(const std::string& rs, const std::string& rt, const std::string& imm) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t imm_value = binaryToDecimal(imm);
    setRegister(regToInt(rt), rs_value | imm_value);
    std::cout << "ori " << rt << ", " << rs << ", " << imm << std::endl;
}

// bne - I 类型指令
void MIPS_Simulator::bne(const std::string& rs, const std::string& rt, const std::string& offset) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));

    // 将6位的偏移量解释为有符号整数
    short offset_value = static_cast<short>(std::stoi(offset, nullptr, 2));

    if (rs_value != rt_value) {
        pc += (offset_value << 2) - 4; // 分支后 PC 将增加 offset_value
    }
    std::cout << "bne " << rs << ", " << rt << ", " << offset_value << std::endl;
}

// beq - I 类型指令
void MIPS_Simulator::beq(const std::string& rs, const std::string& rt, const std::string& offset) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));

    // 将16位的偏移量解释为有符号整数
    short offset_value = static_cast<short>(std::stoi(offset, nullptr, 2));

    if (rs_value == rt_value) {
        pc += (offset_value <<2) - 4; // 分支后 PC 将增加 offset_value
    }
    std::cout << "beq " << rs << ", " << rt << ", " << offset << std::endl;
}


// jal - J 类型指令
void MIPS_Simulator::jal(const std::string& target) {
    functionPrologue(); // 跳转前调用函数前奏
    setRegister(31, pc + 4); // 保存返回地址到 $ra

    // 将26位的目标地址解释为有符号整数
    uint32_t target_value = std::stoi(target, nullptr, 2);

    // 由于是26位地址，左移2位得到32位地址
    pc = (target_value << 2) - 4; // 跳转后 PC 将增加 target_value
    std::cout << "jal " << target << std::endl;
}

// jr - R 类型指令
void MIPS_Simulator::jr(const std::string& rs) {
    pc = getRegister(regToInt(rs)) - 4; // 跳转后 PC 将增加 4
    functionEpilogue(); // 跳转前调用函数结束
    std::cout << "jr " << rs << std::endl;
}

