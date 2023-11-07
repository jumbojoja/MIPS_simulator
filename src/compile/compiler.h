#ifndef COMPILER_H
#define COMPILER_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

enum type { R, I, j };

extern int index;
extern int jump_type;
extern std::vector<std::string> lable;
extern std::vector<int> index_of_lable;
extern std::string mem[1000];
extern std::string machine_code[1000];
extern std::string Hex[1000];

std::vector<std::string> stringSplit(const std::string &str, char delim);
std::string
To_string(int x, int scale, int num_of_str,
          int character =
              0); // 十进制转二进制(数，进制，位数，是否符号扩展(默认不扩展))
void Assembly_to_Machine(std::string *mem);
void get_code();
void write_code();

#endif
