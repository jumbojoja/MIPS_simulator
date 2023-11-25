#include "disassembler.h"
#include <algorithm>
#include <fstream>

using namespace std;

void read_and_write() {
  string line;
  string fileName = "";
  string fileObj;
  string fileAsm;
  return_by_dis disassembleRet; // The disassembleLine function return struct (a
                                // bool and string)
  int lineNum = 0;              // Keeps track of the line number
  int addressLineNum; // This holds the address line numbers calculated when an
                      // address is encountered at a given position
  map<int, std::string> addMap; // Map holds pairs of addresses and lineNumbers

  // cout << "Please input the name of the .obj file: " << endl;
  // cin >> fileName;

  int found = fileName.find(".");
  if (found != string::npos) {
    fileName = fileName.substr(0, found);
  }

  fileObj.append(fileName);
  fileObj.append(".obj");
  fileAsm.append(fileName);
  fileAsm.append(".asm");

  ifstream objStream;
  objStream.open("..\\IO\\machine.obj", ios::in);

  if (!objStream.is_open()) {
    cout << "Failed to open .obj file!" << endl;
    return;
  }

  while (getline(objStream, line)) {
    // 更新后可以直接输入二进制文件
    // string hex;
    // stringstream ss;
    // ss << std::hex << stoi(line, nullptr, 2);
    // ss >> hex;
    // transform(hex.begin(), hex.end(), hex.begin(), ::tolower);
    // while (hex.length() < 8)
    // {
    //     hex = "0" + hex;
    // }
    // line = hex;
    // cout << "这里看看：" << line << endl;

    disassembleRet = disassembleLine(line, lineNum);
    if (disassembleRet.errorFound == true) // 方便找到报错位置
    {
      // cout << "这里就有问题了" << endl;
      cout << "Cannot disassemble " << line << " at line " << lineNum << endl;
    } else {
      // 需要跳转
      if (disassembleRet.branch_address) {
        addressLineNum = disassembleRet.addressOffset + lineNum + 1;
        if (disassembleRet.returnLine.substr(0, 3) == "jal" ||
            disassembleRet.returnLine.substr(0, 2) == "j ") {
          addressLineNum = disassembleRet.addressOffset;
        }
        if (addMap.find(addressLineNum) == addMap.end()) {
          addressReturn temp =
              addressStrForm(disassembleRet.addressOffset, lineNum);
          string label = temp.hexStr;
          addMap[addressLineNum] = label; // 添加标签
        }
      }
    }
    lineNum++;
  }

  // 开始写
  if (disassembleRet.errorFound == false) {
    ofstream asmStream;
    asmStream.open("..\\IO\\machine.asm", ios::out);
    if (!asmStream.is_open()) {
      cout << "Failed to open .asm file!" << endl;
      return;
    }

    objStream.clear();
    objStream.seekg(0, ios_base::beg);
    // objStream重新读取
    lineNum = 0;
    while (getline(objStream, line)) {

      disassembleRet = disassembleLine(line, lineNum);
      for (const auto &pair : addMap) // 转汇编输出时看有没有标签
      {
        if (pair.first == lineNum) {
          asmStream << pair.second << ":\n";
        }
      }
      lineNum++;
      asmStream << "\t" << disassembleRet.returnLine << "\n";
    }
    asmStream.close();
  }
  // Closes out files and finishes program
  objStream.close();
}

return_by_dis disassembleLine(string line, int lineNum) {
  return_by_dis disassembleRet;

  // Checks the length and returns an error if its not 8
  if (line.length() != 8) {
    cout << "Invalid Length" << endl;
    disassembleRet.errorFound = true;
    return disassembleRet;
  }

  disassembleRet = to_binary(line);

  // Now if the error is found it is due to an invalid character
  if (disassembleRet.errorFound == true) {
    cout << "Invalid Char Present" << endl;
    return disassembleRet;
  }

  disassembleRet = get_instruction(disassembleRet.returnLine, lineNum);

  return disassembleRet;
}

return_by_dis to_binary(string line) {
  return_by_dis disassembleRet;
  string binary = "";
  string binaryChar;
  char currChar;

  // For loop goes through each character
  for (int i = 0; i < 8; i++) {
    currChar = line.at(i);
    disassembleRet.errorFound = !validity(currChar); // 先检查一下是不是十六进制
    if (!disassembleRet.errorFound) {
      binaryChar = hexToBinary(currChar);
      binary.append(binaryChar);
    } else {
      return disassembleRet;
    }
  }
  disassembleRet.returnLine = binary;
  return disassembleRet;
}

bool validity(char curr) {
  if (curr == '0' || curr == '1' || curr == '2' || curr == '3' || curr == '4' ||
      curr == '5' || curr == '6' || curr == '7' || curr == '8' || curr == '9' ||
      curr == 'a' || curr == 'b' || curr == 'c' || curr == 'd' || curr == 'e' ||
      curr == 'f') {
    return true;
  }
  return false;
}

string hexToBinary(char hexChar) {
  string binaryStr;
  switch (hexChar) {
  case '0':
    binaryStr = "0000";
    break;
  case '1':
    binaryStr = "0001";
    break;
  case '2':
    binaryStr = "0010";
    break;
  case '3':
    binaryStr = "0011";
    break;
  case '4':
    binaryStr = "0100";
    break;
  case '5':
    binaryStr = "0101";
    break;
  case '6':
    binaryStr = "0110";
    break;
  case '7':
    binaryStr = "0111";
    break;
  case '8':
    binaryStr = "1000";
    break;
  case '9':
    binaryStr = "1001";
    break;
  case 'a':
    binaryStr = "1010";
    break;
  case 'b':
    binaryStr = "1011";
    break;
  case 'c':
    binaryStr = "1100";
    break;
  case 'd':
    binaryStr = "1101";
    break;
  case 'e':
    binaryStr = "1110";
    break;
  case 'f':
    binaryStr = "1111";
    break;
  default:
    binaryStr = "";
    break;
  }
  return binaryStr;
}

return_by_dis get_instruction(string binaryLine, int lineNum) {
  return_by_dis disassembleRet;
  string opcode;
  instructFormat format;
  opcode = binaryLine.substr(0, 6);
  opcode.insert(0, "00");

  format = instructType(opcode);

  if (format == r_type) {
    disassembleRet = r_type_decoder(binaryLine);
  } else {
    disassembleRet = i_type_decoder(binaryLine, opcode, lineNum);
  }
  return disassembleRet;
}

instructFormat instructType(string opcode) {
  if (opcode == "00000000") {
    return r_type;
  }
  return i_type;
}

return_by_dis r_type_decoder(string binaryLine) {
  // cout << binaryLine << endl;
  // if (binaryLine == "00000001001010100100100000100100")
  // {
  //     cout << binaryLine << endl;
  //     cout << "jal上一行来啦" << endl;
  // }

  string rs = binaryLine.substr(6, 5);
  string rt = binaryLine.substr(11, 5);
  string rd = binaryLine.substr(16, 5);
  string shamt = binaryLine.substr(21, 5);
  string funct = binaryLine.substr(26, 6);

  return_by_dis disassembleRet;
  functReturn functRet;
  return_by_dis rdRet;
  return_by_dis rtRet;
  return_by_dis rsRet;
  int shamtRet;
  disassembleRet.returnLine = "";

  functRet = findfunct(funct);
  if (functRet.errorFound) {
    disassembleRet.returnLine = functRet.returnLine;
    disassembleRet.errorFound = functRet.errorFound;
    return disassembleRet;
  }

  disassembleRet.returnLine.append(functRet.returnLine);

  if (functRet.shamtNeeded) {
    rdRet = get_register(rd);
    rtRet = get_register(rt);
    shamtRet = get_shamt(shamt);
    disassembleRet.returnLine.append(" " + rdRet.returnLine);
    disassembleRet.returnLine.append(", " + rtRet.returnLine);
    disassembleRet.returnLine.append(", " + to_string(shamtRet));
  } else {
    // cout << binaryLine << endl;
    rdRet = get_register(rd);
    rsRet = get_register(rs);
    rtRet = get_register(rt);
    if (funct == "001000") {
      disassembleRet.returnLine.append("  " + rsRet.returnLine);
    } else {
      disassembleRet.returnLine.append("  " + rdRet.returnLine);
      disassembleRet.returnLine.append(", " + rsRet.returnLine);
      disassembleRet.returnLine.append(", " + rtRet.returnLine);
    }
  }

  return disassembleRet;
}

return_by_dis i_type_decoder(string binaryLine, string opcode, int lineNum) {
  // cout << binaryLine << endl;
  string opcodeStr1;
  string opcodeStr2;
  char hex1;
  char hex2;
  return_by_dis rsRet;
  return_by_dis rtRet;
  string rs = binaryLine.substr(6, 5);
  string rt = binaryLine.substr(11, 5);
  string immediateStr = binaryLine.substr(16, 16);
  int immNum;
  return_by_dis disassembleRet;
  addressReturn addRet;
  opcodeReturn opcodeRet;
  string addressStr;
  disassembleRet.returnLine = "";

  opcodeStr1 = opcode.substr(0, 4);
  opcodeStr2 = opcode.substr(4, 4);
  hex1 = binary_to_hex(opcodeStr1);
  hex2 = binary_to_hex(opcodeStr2);
  // cout << "在这里：" << hex1 << " " << hex2 << endl;

  opcodeRet = hexOpcode(hex1, hex2);

  if (opcodeRet.errorFound) {
    disassembleRet.returnLine = opcodeRet.returnLine;
    disassembleRet.errorFound = opcodeRet.errorFound;
    return disassembleRet;
  }

  disassembleRet.returnLine.append(opcodeRet.returnLine);

  // Finds the rs and rt strings, adds rt
  rsRet = get_register(rs);
  rtRet = get_register(rt);

  // Finds the immediate value
  immNum = get_imm(immediateStr);
  if (opcode == "00000011" || opcode == "00000010") {
    /* cout << binaryLine; */
    immediateStr = binaryLine.substr(6);
    /* cout << "来看看" << immediateStr; */
    immNum = stoi(immediateStr, nullptr, 2);
    /* cout << " " << immNum << endl; */
  }

  // Translates branch_address variable, also changes the address Offset to
  // non-zero if the address must be printed
  if (opcodeRet.branch_address) {
    addRet = addressStrForm(immNum, lineNum);
    if (addRet.errorFound) {
      disassembleRet.errorFound = addRet.errorFound;
      return disassembleRet;
    }
    if (opcode == "00000011") {
      // cout << "这儿";
      disassembleRet.returnLine.append("  " + addRet.hexStr);
      disassembleRet.branch_address = opcodeRet.branch_address;
      disassembleRet.addressOffset = immNum;
    } else {
      disassembleRet.returnLine.append(" " + rsRet.returnLine);
      disassembleRet.returnLine.append(", " + rtRet.returnLine);
      disassembleRet.branch_address = opcodeRet.branch_address;
      disassembleRet.returnLine.append(", " + addRet.hexStr);
      disassembleRet.addressOffset = immNum;
    }
  } else if (opcodeRet.offset == true) {
    disassembleRet.returnLine.append(" " + rtRet.returnLine);
    disassembleRet.returnLine.append(", " + to_string(immNum));
    disassembleRet.returnLine.append("(" + rsRet.returnLine + ")");
  } else {
    disassembleRet.returnLine.append(" " + rtRet.returnLine);
    disassembleRet.returnLine.append(", " + rsRet.returnLine);
    disassembleRet.returnLine.append(", " + to_string(immNum));
  }

  return disassembleRet;
}

addressReturn addressStrForm(int immNum, int lineNum) {
  int offset;
  stringstream stream;
  addressReturn addRet;
  addRet.hexStr = "Addr_";
  // Finds the line offset than multiplies it by 4
  offset = immNum + 1 + lineNum;
  offset = offset * 4;
  // Makes sure the offset is in range
  if (offset > -32768 && offset < 32767) {
    // Converts the offset to 4 digit hex
    stream << setfill('0') << setw(4) << hex << offset;
    addRet.hexStr.append(stream.str());
  } else {
    addRet.errorFound = true;
  }
  return addRet;
}

opcodeReturn hexOpcode(char hex1, char hex2) {
  opcodeReturn opcodeRet;
  if (hex1 == '0') {
    if (hex2 == '4') // 04
    {
      opcodeRet.returnLine = "beq   ";
      opcodeRet.branch_address = true;
    } else if (hex2 == '3') // 03
    {
      opcodeRet.returnLine = "jal   ";
      // 它的立即执行数获取有问题
      opcodeRet.branch_address = true;
    } else if (hex2 == '5') // 05
    {
      opcodeRet.returnLine = "bne   ";
      opcodeRet.branch_address = true;
    } else if (hex2 == '8') // 08
    {
      opcodeRet.returnLine = "addi  ";
    } else if (hex2 == '9') // 09
    {
      opcodeRet.returnLine = "addiu ";
    } else if (hex2 == 'a') // 0a
    {
      opcodeRet.returnLine = "slti  ";
    } else if (hex2 == 'b') // 0b
    {
      opcodeRet.returnLine = "sltiu ";
    } else if (hex2 == 'c') // 0c
    {
      opcodeRet.returnLine = "andi  ";
    } else if (hex2 == 'd') // 0d
    {
      opcodeRet.returnLine = "ori   ";
    } else if (hex2 == 'f') // 0f
    {
      opcodeRet.returnLine = "lui   ";
    } else {
      // cout << "报错原因看这里：" << hex1 << " " << hex2 << endl;
      opcodeRet.returnLine = "Opcode not found";
      opcodeRet.errorFound = true;
    }
  } else if (hex1 == '2') {
    if (hex2 == '3') // 23
    {
      opcodeRet.returnLine = "lw    ";
    } else if (hex2 == '4') // 24
    {
      opcodeRet.returnLine = "lbu   ";
    } else if (hex2 == '5') // 25
    {
      opcodeRet.returnLine = "lhu   ";
    } else if (hex2 == '8') // 26
    {
      opcodeRet.returnLine = "sb    ";
    } else if (hex2 == '9') // 27
    {
      opcodeRet.returnLine = "sh    ";
    } else if (hex2 == 'b') // 28
    {
      opcodeRet.returnLine = "sw    ";
    } else {
      opcodeRet.returnLine = "Opcode not found";
      opcodeRet.errorFound = true;
    }
    opcodeRet.offset = true;
  } else if (hex1 == '3') {
    if (hex2 == '0') // 30
    {
      opcodeRet.returnLine = "ll    ";
    } else if (hex2 == '8') // 38
    {
      opcodeRet.returnLine = "sc    ";
    } else {
      opcodeRet.returnLine = "Opcode not found";
      opcodeRet.errorFound = true;
    }
    opcodeRet.offset = true;
  } else {
    opcodeRet.returnLine = "Opcode not found";
    opcodeRet.errorFound = true;
  }
  return opcodeRet;
}

return_by_dis get_register(string reg) {
  return_by_dis regReturn;
  int regNum = 0;

  for (int i = 0; i < 5; i++) {
    if (reg[i] == '1') {
      regNum = regNum + (int)(pow(2, (4 - i)));
    }
  }

  if (regNum < 0 || regNum > 31) {
    regReturn.errorFound = true;
    return regReturn;
  }

  regReturn.returnLine = regConvert(regNum);
  return regReturn;
}

string regConvert(int regNum) {
  switch (regNum) {
  // 0 and at 0-1
  case 0:
    return "$zero";
  case 1:
    return "$at";
  // value registers 2-3
  case 2:
    return "$v" + to_string(regNum - 2);
  case 3:
    return "$v" + to_string(regNum - 2);
  // argument registers 4-7
  case 4:
    return "$a" + to_string(regNum - 4);
  case 5:
    return "$a" + to_string(regNum - 4);
  case 6:
    return "$a" + to_string(regNum - 4);
  case 7:
    return "$a" + to_string(regNum - 4);
  // temporary registers 8-15
  case 8:
    return "$t" + to_string(regNum - 8);
  case 9:
    return "$t" + to_string(regNum - 8);
  case 10:
    return "$t" + to_string(regNum - 8);
  case 11:
    return "$t" + to_string(regNum - 8);
  case 12:
    return "$t" + to_string(regNum - 8);
  case 13:
    return "$t" + to_string(regNum - 8);
  case 14:
    return "$t" + to_string(regNum - 8);
  case 15:
    return "$t" + to_string(regNum - 8);
  // saved temporary registers 16-23
  case 16:
    return "$s" + to_string(regNum - 16);
  case 17:
    return "$s" + to_string(regNum - 16);
  case 18:
    return "$s" + to_string(regNum - 16);
  case 19:
    return "$s" + to_string(regNum - 16);
  case 20:
    return "$s" + to_string(regNum - 16);
  case 21:
    return "$s" + to_string(regNum - 16);
  case 22:
    return "$s" + to_string(regNum - 16);
  case 23:
    return "$s" + to_string(regNum - 16);
  // more temporary registers 24-25
  case 24:
    return "$t" + std::to_string(regNum - 16);
  case 25:
    return "$t" + std::to_string(regNum - 16);
  // Reserved for OS kernel
  case 26:
    return "$k" + std::to_string(regNum - 26);
  case 27:
    return "$k" + std::to_string(regNum - 26);
  // $gp
  case 28:
    return "$gp";
  // $sp
  case 29:
    return "$sp";
  // $fp
  case 30:
    return "$fp";
  // $ra
  case 31:
    return "$ra";
  default:
    return "";
  }
}

int get_imm(string immediateStr) {
  int immNum = 0;
  if (immediateStr[0] == '1') // 负数
  {
    for (int i = 1; i < 16; i++) {
      if (immediateStr[i] == '1') {
        immNum = immNum + (int)(pow(2, (15 - i)));
      }
    }
    immNum = 32768 - immNum;
    immNum = immNum * -1;
  } else {
    // cout << "来看看" << immediateStr;
    immNum = stoi(immediateStr, nullptr, 2);
    // cout << " " << immNum << endl;
  }
  return immNum;
}

int get_shamt(string shamt) {
  int shamtNum = 0;
  if (shamt[0] == '1') {
    for (int i = 1; i < 5; i++) {
      if (shamt[i] == '1') {
        shamtNum = shamtNum + (int)(pow(2, (4 - i)));
      }
    }
    shamtNum = 16 - shamtNum;
    shamtNum = shamtNum * -1;
  } else {
    shamtNum = stoi(shamt, nullptr, 2);
  }
  return shamtNum;
}

functReturn findfunct(string funct) {
  functReturn functRet;
  string digit1;
  string digit2;
  char hex1;
  char hex2;
  funct.insert(0, "00");
  digit1 = funct.substr(0, 4);
  digit2 = funct.substr(4, 7);
  hex1 = binary_to_hex(digit1);
  hex2 = binary_to_hex(digit2);
  if (hex1 == '0') {
    if (hex2 == '0') // 0 / 00
    {
      funct = "sll   ";
    } else if (hex2 == '2') // 0 / 02
    {
      funct = "srl   ";
    } else if (hex2 == '8') // 0 / 08
    {
      funct = "jr   ";
    } else {
      cout << "报错原因看这里：" << hex1 << " " << hex2 << endl;
      funct = "Function not found";
      functRet.errorFound = true;
    }
    functRet.shamtNeeded = true;
    if (hex2 == '8')
      functRet.shamtNeeded = false;
  } else if (hex1 == '2') {
    if (hex2 == '0') // 0 / 20
    {
      funct = "add   ";
    } else if (hex2 == '1') // 0 / 21
    {
      funct = "addu  ";
    } else if (hex2 == '2') // 0 / 22
    {
      funct = "sub   ";
    } else if (hex2 == '3') // 0 / 23
    {
      funct = "subu  ";
    } else if (hex2 == '4') // 0 / 24
    {
      funct = "and   ";
    } else if (hex2 == '5') // 0 / 25
    {
      funct = "or    ";
    } else if (hex2 == '7') // 0 / 27
    {
      funct = "nor   ";
    } else if (hex2 == 'a') // 0 / 2a
    {
      funct = "slt   ";
    } else if (hex2 == 'b') // 0 / 2b
    {
      funct = "sltu  ";
    } else {
      cout << "报错原因看这里：" << funct << endl;
      funct = "Function not found";
      functRet.errorFound = true;
    }
    functRet.shamtNeeded = false;
  } else {
    cout << "报错原因看这里：" << funct << endl;
    funct = "Function not found";
    functRet.errorFound = true;
  }
  functRet.returnLine = funct;
  return functRet;
}

char binary_to_hex(string binaryStr) {
  char hexChar;
  if (binaryStr == "0000") {
    hexChar = '0';
  } else if (binaryStr == "0001") {
    hexChar = '1';
  } else if (binaryStr == "0010") {
    hexChar = '2';
  } else if (binaryStr == "0011") {
    hexChar = '3';
  } else if (binaryStr == "0100") {
    hexChar = '4';
  } else if (binaryStr == "0101") {
    hexChar = '5';
  } else if (binaryStr == "0110") {
    hexChar = '6';
  } else if (binaryStr == "0111") {
    hexChar = '7';
  } else if (binaryStr == "1000") {
    hexChar = '8';
  } else if (binaryStr == "1001") {
    hexChar = '9';
  } else if (binaryStr == "1010") {
    hexChar = 'a';
  } else if (binaryStr == "1011") {
    hexChar = 'b';
  } else if (binaryStr == "1100") {
    hexChar = 'c';
  } else if (binaryStr == "1101") {
    hexChar = 'd';
  } else if (binaryStr == "1110") {
    hexChar = 'e';
  } else if (binaryStr == "1111") {
    hexChar = 'f';
  } else {
    hexChar = '\0';
  }
  return hexChar;
}
