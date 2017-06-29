//
//  program.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef program_hpp
#define program_hpp

#include <vector>
#include <map>
#include <string>
#include "CPU.hpp"
#include "memory.hpp"
#include "scanner.hpp"
#include "exception.hpp"
#include "statement.hpp"
#include <iostream>
using std::string;
using std::vector;
using std::map;
using std::istream;
using std::ostream;

class statement;
class Program;

class command
{
	Program &pro;
	Memory &mem;
public:
	string op;
	int data[3];
	string label;
	int state[3] = {0, 0, 0};      //0-empty, 1-CPU cache, 2-immediant number, 3-offset, 4-label
	command(const string &_op, Program &_pro, Memory &_mem);
	friend scanner& operator >> (scanner &scan, command &x);
};

class Program
{
	vector<command> commands;
	map<string, int> labels;
	istream &is; ostream &os;
	statement *cache[4];
	CPU &cpu;
	Memory &mem;
public:
	Program(CPU &_cpu, Memory &_mem, istream &_is, ostream &_os);
	
	int hazard = 0;

	void push_back(const command &x);
	
	void addLabel(const string &Label);
	
	int getLabel(const string &Label);
	
	command* getcommand(int index);
	
	command* getcommand(const string &label);
	
	void IF();
	void ID();
	void EX();
	void MA();
	void WB();
	
	int run();
};

#endif /* program_hpp */
