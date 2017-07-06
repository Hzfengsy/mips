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
#include <condition_variable>
#include "statement.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
using std::string;
using std::vector;
using std::map;
using std::istream;
using std::ostream;
using std::mutex;
using std::thread;
using std::atomic;


class statement;
class Program;

class command
{
	Program &pro;
	Memory &mem;
	static map<string, OP> ops;
public:
	OP op;
	int data[3];
	string label;
	int state[3] = {0, 0, 0};      //0-empty, 1-CPU cache, 2-immediant number, 3-offset, 4-label
	command(const string &_op, Program &_pro, Memory &_mem);
	friend scanner& operator >> (scanner &scan, command &x);
	static void inti();
};

class Program
{
	vector<command> commands;
	vector<statement*> instructions;
	map<string, int> labels;
	istream &is; ostream &os;
	statement* cache[4], *tmp[4];
	atomic<bool> ready[5];
	atomic<int> count;
	CPU &cpu;
	Memory &mem;
	int clocks = 0;
	friend class statement;
	friend class syscall;
public:
	Program(CPU &_cpu, Memory &_mem, istream &_is, ostream &_os);
	~Program();
	
	bool globl;
	atomic<bool> hazard;
	int globl_return;

	void push_back(const command &x);
	
	void addLabel(const string &Label);
	
	int getLabel(const string &Label);
	
	OP getcommand(int index, int data[], char state[]);
	OP getOp(int index);
	
	statement* getInstruction(int index);
	command* getcommand(const string &label);
	
	void exchengLabel();
	
	void IF();
	void ID();
	void EX();
	void MA();
	void WB();
	
	int run();
};

#endif /* program_hpp */
