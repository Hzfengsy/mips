//
//  program.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "program.hpp"
#include <iostream>
#include <fstream>
using std::cerr;
using std::endl;

class statement;
using namespace std::chrono;

map<string, OP> command::ops;

command::command(const string &_op, Program &_pro, Memory &_mem) : pro(_pro), mem(_mem)
{ op = command::ops[_op]; }

void command::inti()
{
	ops["add"] = ops["addi"] = ops["addu"] = ops["addui"] = ADD;
	ops["sub"] = ops["subi"] = ops["subu"] = ops["subui"] = SUB;
	ops["mul"] = MUL, ops["mulu"] = MULU;
	ops["div"] = DIV, ops["divu"] = DIVU;
	ops["xor"] = ops["xoru"] = XOR;
	ops["neg"] = ops["negu"] = NEG;
	ops["rem"] = REM, ops["remu"] = REMU;
	ops["li"] = LI;
	ops["seq"] = SEQ, ops["sne"] = SNE;
	ops["sgt"] = SGT, ops["slt"] = SLT;
	ops["sge"] = SGE, ops["sle"] = SLE;
	ops["j"] = ops["b"] = ops["jr"] = JMP;
	ops["jal"] = ops["jalr"] = JMPL;
	ops["beq"] = BEQ, ops["beqz"] = BEQZ;
	ops["bne"] = BNE, ops["bnez"] = BNEZ;
	ops["bgt"] = BGT, ops["bgtz"] = BGTZ;
	ops["blt"] = BLT, ops["bltz"] = BLTZ;
	ops["bge"] = BGE, ops["bgez"] = BGEZ;
	ops["ble"] = BLE, ops["blez"] = BLEZ;
	ops["la"] = LA, ops["lb"] = LB;
	ops["lh"] = LH, ops["lw"] = LW;
	ops["sb"] = SB, ops["sh"] = SH, ops["sw"] = SW;
	ops["move"] = MOVE;
	ops["mfhi"] = MFHI;
	ops["mflo"] = MFLO;
	ops["nop"] = NOP, ops["syscall"] = SYS;
}

scanner& operator >> (scanner &scan, command &x)
{
	for (int i = 0; scan.hasNextArg(); i++)
	{
		string t = scan.getNextArg();
		//1-CPU cache
		if (t[0] == '$')
		{
			x.data[i] = CPU::exchange(t);
			x.state[i] = 1;
			continue;
		}
		
		//2-ime num
		try
		{
			x.data[i] = stringToInt(t);
			x.state[i] = 2;
			continue;
		}
		catch (...) {}
		
		//3-offset
		try
		{
			int l = int(t.find('(')), r = int(t.find(')'));
			if (l == string::npos || r == string::npos) throw invalid_program();
			x.data[i] = stringToInt(t.substr(0, l));
			x.state[i] = 3;
			x.data[++i] = CPU::exchange(t.substr(l + 1, r - l - 1));
			x.state[i] = 1;
			continue;
		}
		catch (...) {}
		
		//4-label
		x.state[i] = 4;
		x.label = t;
	}
	return scan;
}

Program::Program(CPU &_cpu, Memory &_mem, istream &_is, ostream &_os)
	:cpu(_cpu), mem(_mem), is(_is), os(_os) {}

void Program::push_back(const command &x) { commands.push_back(x); }

void Program::addLabel(const string &Label)
{
	if (labels.find(Label) != labels.end()) throw invalid_identifier();
	labels[Label] = int(commands.size());
}

int Program::getLabel(const string &Label)
{
	if (labels.find(Label) == labels.end()) throw invalid_identifier();
	return labels[Label];
}

OP Program::getcommand(int index, int data[], bool &imm, int &num)
{
	imm = num = 0;
	for (int i = 0; i < 3; i++)
	{
		data[i] = commands[index].data[i];
		if (commands[index].state[i] == 2) imm = 1;
		if (commands[index].state[i]) num++;
	}
	if (commands[index].op == BEQ || commands[index].op == BNE || commands[index].op == BLE ||
		commands[index].op == BLT || commands[index].op == BGE || commands[index].op == BGT)
		imm = commands[index].state[1] == 2;
	return commands[index].op;
}

command* Program::getcommand(const string &label) { return new command(commands[getLabel(label)]); }

void Program::exchengLabel()
{
	for (int i = 0; i < commands.size(); i++)
		for (int j = 0; j < 3; j++)
			if (commands[i].state[j] == 4)
			{
				try { commands[i].data[j] = getLabel(commands[i].label); }
				catch(...) { commands[i].data[j] = mem.getaddress(commands[i].label); }
				commands[i].state[j] = 2;
			}
}

void Program::IF()
{
	while(1)
	{
		std::unique_lock<mutex> lock0(_lock[0]);
		while (cache[0] != NULL) empty[0].wait(lock0);
//		empty_lock[0].lock();
		statement *st = new statement(this);
		cache[0] = st->IF();
		full[0].notify_all();
//		full_lock[0].unlock();
	}
}

void Program::ID()
{
	while (1)
	{
		std::unique_lock<mutex> lock0(_lock[0]);
		while (cache[0] == NULL) full[0].wait(lock0);
		std::unique_lock<mutex> lock1(_lock[1]);
		while (cache[1] != NULL) empty[1].wait(lock1);
//		empty_lock[1].lock(), full_lock[0].lock();
		statement *ans = cache[0]->ID();
		cache[1] = ans;
		delete cache[0]; cache[0] = NULL;
		full[1].notify_all(), empty[0].notify_all();
		//empty_lock[0].unlock(), full_lock[1].unlock();
		clocks++;
	}
}

void Program::EX()
{
	while(1)
	{
		std::unique_lock<mutex> lock1(_lock[1]);
		while (cache[1] == NULL) full[1].wait(lock1);
		std::unique_lock<mutex> lock2(_lock[2]);
		while (cache[2] != NULL) empty[2].wait(lock2);
//		empty_lock[2].lock(), full_lock[1].lock();
		statement *ans = cache[1]->EXEC();
		cache[2] = ans;
		cache[1] = NULL;
		full[2].notify_all(), empty[1].notify_all();
//		empty_lock[1].unlock(), full_lock[2].unlock();
	}
}

void Program::MA()
{
	while(1)
	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(900));
		std::unique_lock<mutex> lock2(_lock[2]);
		while (cache[2] == NULL) full[2].wait(lock2);
		std::unique_lock<mutex> lock3(_lock[3]);
		while (cache[3] != NULL) empty[3].wait(lock3);
//		empty_lock[3].lock(), full_lock[2].lock();
		statement *ans = cache[2]->MA();
		cache[3] = ans;
		cache[2] = NULL;
		full[3].notify_all(), empty[2].notify_all();
//		empty_lock[2].unlock(), full_lock[3].unlock();
	}
}

void Program::WB()
{
//	std::ofstream fout("/Users/fengsiyuan/Onedrive/OI/SJTU/mips/Test.out");
	while(1)
	{
		std::unique_lock<mutex> lock3(_lock[3]);
		while (cache[3] == NULL) full[3].wait(lock3);
//		full_lock[3].lock();
		cache[3]->WB();
		delete cache[3]; cache[3] = NULL;
		empty[3].notify_all();
//		empty_lock[3].unlock();
//		for (int i = 0; i < 35; i++) fout << cpu[i] << " ";
//		fout << endl;
	}
}

int Program::run()
{
	cache[0] = cache[1] = cache[2] = cache[3] = NULL;
//	mem.lock = 0;
	cpu["$PC"] = getLabel("main");
//
	globl.lock();
	for (int i = 0; i < 4; i++) full_lock[i].lock();
	thread _WB(std::bind(&Program::WB, this)); _WB.detach();
	thread _MA(std::bind(&Program::MA, this)); _MA.detach();
	thread _EX(std::bind(&Program::EX, this)); _EX.detach();
	thread _ID(std::bind(&Program::ID, this)); _ID.detach();
	thread _IF(std::bind(&Program::IF, this)); _IF.detach();
	
	globl.lock();
	globl.unlock();
	cerr << endl << clocks << endl;
	return globl_return;
}
