#pragma once

#ifndef COMPARCHPROJ1_HPP
#define COMPARCHPROJ1_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <map>

using namespace std;

struct return_by_dis
{
	string returnLine = "";
	bool errorFound = false;
	bool branch_address = false; // true with branch functions
	int addressOffset = 0;
};

struct functReturn
{
	string returnLine = "";
	bool errorFound = false;
	bool shamtNeeded = false;
};

struct opcodeReturn
{
	string returnLine = "";
	bool errorFound = false;
	bool offset = false;
	bool branch_address = false;
};

struct printInstruct
{
	string instruction = "";
	int addressOffset = 0;
	bool branch_address = false; // true with branch functions
};

struct addressReturn
{
	string hexStr = "";
	bool errorFound = false;
};

// Labels for i type and r type instructions
enum instructFormat
{
	i_type,
	r_type
};

// Algorithm to disasemble the current line while also checking for errors
return_by_dis disassembleLine(string line, int lineNum);
return_by_dis to_binary(string line);
bool validity(char curr);
string hexToBinary(char hexChar);

// Converts the binary into instructions while checking that the instruction is valid
return_by_dis get_instruction(string binaryLine, int lineNum);

// Takes in opcode and returns instruction type
instructFormat instructType(string opcode);

return_by_dis r_type_decoder(string binaryLine);
return_by_dis i_type_decoder(string binaryLine, string opcode, int lineNum);

// Converts 2 hex digits into an I-instruction opcode, also checks for invalid opcode
opcodeReturn hexOpcode(char hex1, char hex2);

// Finds the funct from the binary value, finds if shamt or a 3rd register is assessed, spits out an error if not a valid funct
functReturn findfunct(string funct);

// Finds the register from the binary value, will spit out an error if not a valid register
return_by_dis get_register(string reg);

// Converts immediateStr to corresponding int value
int get_imm(string immediateStr);

// Converts shamt string to corresponding int value
int shamtVal(string shamt);

// Converts regNum to register string
string regConvert(int regNum);

// Converts binary string to hex char
char binaryToHex(string binaryStr);

// Creates the address string
addressReturn addressStrForm(int immNum, int lineNum);

#endif
