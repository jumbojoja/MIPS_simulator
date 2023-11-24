#include "MIPS_Simulator.h"


// ���Ĵ����ַ���ӳ�䵽��ŵĺ���
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
        return -1; // ��Ч�ļĴ���
    }
}

// ���������ַ���ת��Ϊʮ�������ֵĺ���
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

// add - R ����ָ��
void MIPS_Simulator::add(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value + rt_value);
    std::cout << "add " << rd << ", " << rs << ", " << rt << std::endl;
}

// and - R ����ָ��
void MIPS_Simulator::andOp(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value & rt_value);
    std::cout << "and " << rd << ", " << rs << ", " << rt << std::endl;
}

// or - R ����ָ��
void MIPS_Simulator::orOp(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value | rt_value);
    std::cout << "or " << rd << ", " << rs << ", " << rt << std::endl;
}

// sub - R ����ָ��
void MIPS_Simulator::sub(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), rs_value - rt_value);
    std::cout << "sub " << rd << ", " << rs << ", " << rt << std::endl;
}

// nor - R ����ָ��
void MIPS_Simulator::nor(const std::string& rs, const std::string& rt, const std::string& rd) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));
    setRegister(regToInt(rd), ~(rs_value | rt_value));
    std::cout << "nor " << rd << ", " << rs << ", " << rt << std::endl;
}

// sll - R ����ָ��
void MIPS_Simulator::sll(const std::string& rt, const std::string& rd, const std::string& sa) {
    uint32_t rt_value = getRegister(regToInt(rt));
    uint32_t sa_value = binaryToDecimal(sa);
    setRegister(regToInt(rd), rt_value << sa_value);
    std::cout << "sll " << rd << ", " << rt << ", " << sa << std::endl;
}

// srl - R ����ָ��
void MIPS_Simulator::srl(const std::string& rt, const std::string& rd, const std::string& sa) {
    uint32_t rt_value = getRegister(regToInt(rt));
    uint32_t sa_value = binaryToDecimal(sa);
    setRegister(regToInt(rd), rt_value >> sa_value);
    std::cout << "srl " << rd << ", " << rt << ", " << sa << std::endl;
}

// addi - I ����ָ��
void MIPS_Simulator::addi(const std::string& rs, const std::string& rt, const std::string& imm) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t imm_value = binaryToDecimal(imm);
    setRegister(regToInt(rt), rs_value + imm_value);
    std::cout << "addi " << rt << ", " << rs << ", " << imm << std::endl;
}

// andi - I ����ָ��
void MIPS_Simulator::andi(const std::string& rs, const std::string& rt, const std::string& imm) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t imm_value = binaryToDecimal(imm);
    setRegister(regToInt(rt), rs_value & imm_value);
    std::cout << "andi " << rt << ", " << rs << ", " << imm << std::endl;
}

// ori - I ����ָ��
void MIPS_Simulator::ori(const std::string& rs, const std::string& rt, const std::string& imm) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t imm_value = binaryToDecimal(imm);
    setRegister(regToInt(rt), rs_value | imm_value);
    std::cout << "ori " << rt << ", " << rs << ", " << imm << std::endl;
}

// bne - I ����ָ��
void MIPS_Simulator::bne(const std::string& rs, const std::string& rt, const std::string& offset) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));

    // ��6λ��ƫ��������Ϊ�з�������
    short offset_value = static_cast<short>(std::stoi(offset, nullptr, 2));

    if (rs_value != rt_value) {
        pc += (offset_value << 2) - 4; // ��֧�� PC ������ offset_value
    }
    std::cout << "bne " << rs << ", " << rt << ", " << offset_value << std::endl;
}

// beq - I ����ָ��
void MIPS_Simulator::beq(const std::string& rs, const std::string& rt, const std::string& offset) {
    uint32_t rs_value = getRegister(regToInt(rs));
    uint32_t rt_value = getRegister(regToInt(rt));

    // ��16λ��ƫ��������Ϊ�з�������
    short offset_value = static_cast<short>(std::stoi(offset, nullptr, 2));

    if (rs_value == rt_value) {
        pc += (offset_value <<2) - 4; // ��֧�� PC ������ offset_value
    }
    std::cout << "beq " << rs << ", " << rt << ", " << offset << std::endl;
}


// jal - J ����ָ��
void MIPS_Simulator::jal(const std::string& target) {
    functionPrologue(); // ��תǰ���ú���ǰ��
    setRegister(31, pc + 4); // ���淵�ص�ַ�� $ra

    // ��26λ��Ŀ���ַ����Ϊ�з�������
    uint32_t target_value = std::stoi(target, nullptr, 2);

    // ������26λ��ַ������2λ�õ�32λ��ַ
    pc = (target_value << 2) - 4; // ��ת�� PC ������ target_value
    std::cout << "jal " << target << std::endl;
}

// jr - R ����ָ��
void MIPS_Simulator::jr(const std::string& rs) {
    pc = getRegister(regToInt(rs)) - 4; // ��ת�� PC ������ 4
    functionEpilogue(); // ��תǰ���ú�������
    std::cout << "jr " << rs << std::endl;
}

