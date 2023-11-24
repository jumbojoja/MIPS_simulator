#include "compiler.h"
#include <fstream>

using namespace std;

int index = 0;
int jump_type = 0;
std::vector<std::string> lable;
std::vector<int> index_of_lable;
std::string mem[1000];
std::string machine_code[1000];
std::string Hex[1000];

vector<std::string> stringSplit(const std::string &str, char delim) {
  std::size_t previous = 0;
  std::size_t current = str.find(delim);
  std::vector<std::string> elems;
  while (current != std::string::npos) {
    if (current > previous) {
      elems.push_back(str.substr(previous, current - previous));
    }
    previous = current + 1;
    current = str.find(delim, previous);
  }
  if (previous != str.size()) {
    elems.push_back(str.substr(previous));
  }
  return elems;
}

string To_string(
    int x, int scale, int num_of_str,
    int character) // 十进制转二进制(数，进制，位数，是否符号扩展(默认不扩展))
{
  int flag = 0, num = 0;
  string s = "";
  string tmp;
  if (x < 0) {
    x = -x;
    flag = 1;
  }
  while (x) {
    if (x % scale < 10)
      tmp = to_string(x % scale);
    else {
      tmp = 'a' + (x % scale - 10);
    }
    if (x % scale == 0)
      s = '0' + s;
    else
      s = tmp + s;
    x /= scale;
  }

  while (s.size() < num_of_str) {
    if (character == 0)
      s = '0' + s;
    else if (flag) // 负数符号扩展,因为下一步会取反，因此我们先补0
      s = '0' + s;
    else
      s = '0' + s;
  }
  if (flag) {
    for (auto &each : s) {
      each = each == '1' ? 'm' : each;
    }
    for (auto &each : s) {
      each = each == '0' ? '1' : each;
    }
    for (auto &each : s) {
      each = each == 'm' ? '0' : each;
    }
    num = 1 + stoi(s, nullptr, 2);
    s = To_string(num, scale, num_of_str);
  }
  return s;
}

void Assembly_to_Machine(string *mem) {
  int i = 0;
  while (mem[i] != "") {
    jump_type = 0;
    string Machine_code;
    string Immediate;
    string opcode; // 指令
    string funct;  // func
    string type_of_rd = "";
    string type_of_rs = "";
    string type_of_rt = "";
    string str_rd = "", str_rt = "", str_rs = "";

    int rd = 0, rt = 0, rs = 0;

    string cur = mem[i];

    // string part[0] = cur.substr(0,)
    vector<string> part = stringSplit(cur, ' ');

    /* cout << "当前指令有" << part.size() << "部分."; */
    // for (int i = 0; i < part.size(); ++i)
    // {
    //     cout << part[i] << endl;
    // }

    if (part.size() == 5) // 有标签
    {
      // lable.push_back(part[0]);
      // index_of_lable.push_back(index);
      /* cout << "标签索引：" << index << " "; */
      part.erase(part.begin());
    } else if (part.size() == 2) // 跳转型指令
    {
      // lable.push_back("");
      // index_of_lable.push_back(0);
      part.push_back("");
      part.push_back("");
      jump_type = 1;
      // str_rd = part[1].substr(1);
      // cout << "jump型";
    } else {
      // lable.push_back("");
      // index_of_lable.push_back(0);
    }

    if (part[0] == "addi" || part[0] == "addiu" || part[0] == "andi" ||
        part[0] == "ori" || part[0] == "xori" || part[0] == "lui" ||
        part[0] == "lw" || part[0] == "sw" || part[0] == "beq" ||
        part[0] == "bne" || part[0] == "slti" || part[0] == "sltiu") // I型指令
    {
      Immediate = To_string(rt, 2, 16);
      /* cout << "i型指令"; */
    } else if (part[0] == "j" || part[0] == "jal") // J型
    {
      /* cout << "j/jal指令"; */
    } else if (part[0] == "jr" && part[1] == "$ra") // Jr特殊
    {
      /* cout << "666这条指令需要获取ra的值,暂时无法处理"; */
    } else // R型，确定三个寄存器，注意jr特殊
    {
      opcode = "000000";
      /* cout << "R型指令"; */
    }

    if (1) // 把寄存器分出来
    {
      // 得到三个寄存器
      // cout << part[1];
      for (int i = 0; i < part[1].length(); i++) {
        if (isdigit(part[1][i])) {
          str_rd += part[1][i];
        }
      }
      for (int i = 0; i < part[2].length(); i++) {
        if (isdigit(part[2][i])) {
          str_rs += part[2][i];
        }
      }
      for (int i = 0; i < part[3].length(); i++) {
        if (isdigit(part[3][i])) {
          str_rt += part[3][i];
        }
      }
      // cout << "rd为:" << part[1][1] + str_rd << ".";
      // cout << "rs为:" << part[2][1] + str_rs << ".";
      // cout << "rt为:" << part[3][1] + str_rt << ".";
      if (str_rd != "")
        rd = stoi(str_rd);
      if (str_rs != "")
        rs = stoi(str_rs);
      if (str_rt != "")
        rt = stoi(str_rt);

      // 获得寄存器的真实序号（0~31）
      for (int i = 1; i < part[1].length(); i++) {
        if (!isdigit(part[1][i]) && part[1][i] != ',') {
          type_of_rd += part[1][i];
        }
      }
      if (jump_type == 1) {
        type_of_rd = part[1][0] + type_of_rd;
      }
      for (int i = 1; i < part[2].length(); i++) {
        if (!isdigit(part[2][i]) && part[2][i] != ',') {
          type_of_rs += part[2][i];
        }
      }
      for (int i = 1; i < part[3].length(); i++) {
        if (!isdigit(part[3][i]) && part[3][i] != ',') {
          type_of_rt += part[3][i];
        }
      }
      // rd
      if (type_of_rd == "zero") {
        rd = 0;
      } else if (type_of_rd == "at") {
        rd = 1;
      } else if (type_of_rd == "v") {
        rd += 2;
      } else if (type_of_rd == "a") {
        rd += 4;
      } else if (type_of_rd == "t" && rd <= 7) {
        rd += 8;
      } else if (type_of_rd == "s") {
        rd += 16;
      } else if (type_of_rd == "t" && rd > 7) {
        rd += 16;
      } else if (type_of_rd == "gp") {
        rd = 28;
      } else if (type_of_rd == "sp") {
        rd = 29;
      } else if (type_of_rd == "fp") {
        rd = 30;
      } else if (type_of_rd == "ra") {
        rd = 31;
      }

      // rs
      if (type_of_rs == "zero") {
        rs = 0;
      } else if (type_of_rs == "at") {
        rs = 1;
      } else if (type_of_rs == "v") {
        rs += 2;
      } else if (type_of_rs == "a") {
        rs += 4;
      } else if (type_of_rs == "t" && rs <= 7) {
        rs += 8;
      } else if (type_of_rs == "s") {
        rs += 16;
      } else if (type_of_rs == "t" && rs > 7) {
        rs += 16;
      } else if (type_of_rs == "gp") {
        rs = 28;
      } else if (type_of_rs == "sp") {
        rs = 29;
      } else if (type_of_rs == "fp") {
        rs = 30;
      } else if (type_of_rs == "ra") {
        rs = 31;
      }

      // rt
      if (type_of_rt == "zero") {
        rt = 0;
      } else if (type_of_rt == "at") {
        rt = 1;
      } else if (type_of_rt == "v") {
        rt += 2;
      } else if (type_of_rt == "a") {
        rt += 4;
      } else if (type_of_rt == "t" && rt <= 7) {
        rt += 8;
      } else if (type_of_rt == "s") {
        rt += 16;
      } else if (type_of_rt == "t" && rt > 7) {
        rt += 16;
      } else if (type_of_rt == "gp") {
        rt = 28;
      } else if (type_of_rt == "sp") {
        rt = 29;
      } else if (type_of_rt == "fp") {
        rt = 30;
      } else if (type_of_rt == "ra") {
        rt = 31;
      }
    }

    // cout << "type_of_rd:" << type_of_rd << ",";
    // cout << "type_of_rs:" << type_of_rs << ",";
    // cout << "type_of_rt:" << type_of_rt << ",";

    /* cout << "寄存器索引分别为" << rd << "," << rs << "," << rt; */
    // 处理指令
    // R型

    if (1) // 根据指令确定opcode和funct
    {
      if (part[0] == "add") {
        funct = "100000";
        // cout << "起飞" << endl;
        // cout << rd;
      } else if (part[0] == "addu") {
        funct = "100001";
      } else if (part[0] == "sub") {
        funct = "100010";
        // cout << "降落" << endl;
      } else if (part[0] == "subu") {
        funct = "100011";
      } else if (part[0] == "and") {
        funct = "100100";
      } else if (part[0] == "or") {
        funct = "100101";
      } else if (part[0] == "xor") {
        funct = "100110";
      } else if (part[0] == "nor") {
        funct = "100111";
      } else if (part[0] == "slt") {
        funct = "101010";
      } else if (part[0] == "sltu") {
        funct = "101011";
      } else if (part[0] == "sll") {
        funct = "000000";
      } else if (part[0] == "srl") {
        funct = "000010";
      } else if (part[0] == "sra") {
        funct = "000011";
      } else if (part[0] == "sllv") {
        funct = "000100";
      } else if (part[0] == "srlv") {
        funct = "000110";
      } else if (part[0] == "srav") {
        funct = "000111";
      } else if (part[0] == "jr") {
        funct = "001000";
      }
      // I型
      else if (part[0] == "addi") {
        opcode = "001000";
      } else if (part[0] == "addiu") {
        opcode = "001001";
      } else if (part[0] == "andi") {
        opcode = "001100";
      } else if (part[0] == "ori") {
        opcode = "001101";
      } else if (part[0] == "xori") {
        opcode = "001110";
      } else if (part[0] == "lui") {
        opcode = "001111";
      } else if (part[0] == "lw") {
        opcode = "100011";
      } else if (part[0] == "sw") {
        opcode = "101011";
      } else if (part[0] == "beq") {
        opcode = "000100";
      } else if (part[0] == "bne") {
        opcode = "000101";
      } else if (part[0] == "slti") {
        opcode = "001010";
      } else if (part[0] == "sltiu") {
        opcode = "001011";
      }
      // j型
      else if (part[0] == "j") {
        opcode = "000010";
      } else if (part[0] == "jal") {
        opcode = "000011";
      }
    }

    if (part[0] == "addiu" || part[0] == "andi" || part[0] == "ori" ||
        part[0] == "xori" || part[0] == "lui" || part[0] == "lw" ||
        part[0] == "sw" || part[0] == "beq" || part[0] == "bne" ||
        part[0] == "slti" || part[0] == "sltiu") // J型指令
    {
      if (!isdigit(part[3][0])) // 标签跳转型
      {
        for (int i = 0; i < lable.size(); i++) {
          // cout << lable[i];
          if (lable[i] == part[3] + ":") {
            rt = i;
            // cout << "进来了";
            break;
          }
        }
        // cout << "rt为:" << rt;
        rt = rt - (i + 1);
        /* cout << "跳转距离：" << rt << " "; */
      }
      if (part[0] == "lw" || part[0] == "sw" || part[0] == "beq" ||
          part[0] == "bne")
        Machine_code = opcode + To_string(rd, 2, 5) + To_string(rs, 2, 5) +
                       To_string(rt, 2, 16, 1);
      else
        Machine_code = opcode + To_string(rs, 2, 5) + To_string(rd, 2, 5) +
                       To_string(rt, 2, 16);
    } else if (part[0] == "addi") {
      Machine_code = opcode + To_string(rs, 2, 5) + To_string(rd, 2, 5) +
                     To_string(rt, 2, 16, 1);      // addi按符号位扩展
    } else if (part[0] == "j" || part[0] == "jal") // J型
    {
      for (int i = 0; i < lable.size(); i++) {
        if (lable[i] == type_of_rd + ":") {
          rd = i;
          break;
        }
      }
      /* cout << "跳转目标索引为：" << rd; */
      Machine_code = opcode + To_string(rd, 2, 26);
    } else if (part[0] == "srl" || part[0] == "sll" || part[0] == "sra") {
      Machine_code = opcode + "00000" + To_string(rs, 2, 5) +
                     To_string(rd, 2, 5) + To_string(rt, 2, 5) + funct;
    } else if (part[0] == "jr") // Jr特殊
    {
      Machine_code = "000000" + To_string(rs, 2, 5) + "000000000000000001000";
    } else // 普通无shamt的R型，确定三个寄存器，注意jr特殊
    {
      Machine_code = opcode + To_string(rs, 2, 5) + To_string(rt, 2, 5) +
                     To_string(rd, 2, 5) + "00000" + funct;
    }

    // cout << "机器码有" << Machine_code.size() << "位:" << Machine_code <<
    // endl;

    string hex;
    stringstream ss;
    ss << std::hex << stoi(Machine_code, nullptr, 2);
    ss >> hex;
    // transform(hex.begin(), hex.end(), hex.begin(), ::toupper);

    /* cout << "\t" */
    /*      << "16进制机器码:" << setw(8) << setfill('0') << hex << endl; */
    /**/
    /* cout << "机器码有" << Machine_code.size() << "位:" << Machine_code <<
     * endl; */
    while (hex.length() < 8) {
      hex = "0" + hex;
    }
    Hex[i] = "0x" + hex;
    machine_code[i] = Machine_code;
    i++;
    index += 1;
  }
}

void get_code() {
  int i = 0, j = 0;
  const int LINE_LENGTH = 100;
  char l[LINE_LENGTH];
  ifstream infile;
  infile.open("..\\IO\\in.asm", ios::in);
  while (infile.getline(l, LINE_LENGTH)) {
    mem[i] = l;
    i++;
  }
  infile.close();
  while (mem[j] != "") {
    vector<string> part = stringSplit(mem[j], ' ');
    if (part.size() == 5) // 有标签
    {
      lable.push_back(part[0]);
      index_of_lable.push_back(index);
      // cout << "标签索引：" << index;
      part.erase(part.begin());
    } else if (part.size() == 2) // 跳转型指令
    {
      lable.push_back("");
      index_of_lable.push_back(0);
      part.push_back("");
      part.push_back("");
      jump_type = 1;
      // str_rd = part[1].substr(1);
      // cout << "jump型";
    } else {
      lable.push_back("");
      index_of_lable.push_back(0);
    }
    j++;
  }
  Assembly_to_Machine(mem);
}

void write_code() {
  int i = 0;
  ofstream outfile;
  outfile.open("..\\IO\\out.bin", ios::out);
  while (machine_code[i] != "") {
    outfile << machine_code[i] << std::endl;
    i++;
  }
  outfile.close();
}
