//
//  scanner.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef scanner_hpp
#define scanner_hpp

#include <string>
#include "exception.hpp"
using std::string;

enum OP
{
	ADD, SUB, MUL, MULU, DIV, DIVU, XOR, NEG, REM, REMU, LI, SEQ, SGE, SGT, SLE, SLT, SNE,
	JMP, JMPL, BEQ, BNE, BGE, BGT, BLE, BLT, BEQZ, BNEZ, BGEZ, BGTZ, BLEZ, BLTZ, LA,
	LB, LH, LW, SB, SH, SW, MOVE, MFHI, MFLO, NOP, SYS
};

class scanner
{
	string st;
	int index, len;
public:
	scanner(const string &_st);
	
	string getNext(char c = ' ');
	
	bool hasNext(char c = ' ');
	
	string getNextArg();
	
	bool hasNextArg();
	
	string getNextString();
};

int stringToInt(const string &token);

string getString(const string &st, int &index);

#endif /* scanner_hpp */
