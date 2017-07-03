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

OP Program::getcommand(int index, int data[], int state[3])
{
	for (int i = 0; i < 3; i++)
	{
		data[i] = commands[index].data[i];
		state[i] = commands[index].state[i];
	}
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
	if (cache[0] != NULL) return;
	statement *st = new statement(this);
	statement *ans = st->IF();
	if (ans) cache[0] = ans;
	else delete st;
}

void Program::ID()
{
	if (cache[1] != NULL || cache[0] == NULL) return;
	statement *ans = cache[0]->ID();
	if (ans == NULL) return;
	cache[1] = ans;
	delete cache[0]; cache[0] = NULL;
}

void Program::EX()
{
	if (cache[2] != NULL || cache[1] == NULL) return;
	statement *ans = cache[1]->EXEC();
	if (ans == NULL) return;
	cache[2] = ans;
	cache[1] = NULL;
}

void Program::MA()
{
	if (cache[3] != NULL || cache[2] == NULL) return;
	statement *ans = cache[2]->MA();
	if (ans == NULL) return;
	cache[3] = ans;
	cache[2] = NULL;
}

void Program::WB()
{
	
	if (cache[3] == NULL) return;
	statement *ans = cache[3]->WB();
	if (ans == NULL) return;
	delete cache[3]; cache[3] = NULL;
	
}

int Program::run()
{
	cache[0] = cache[1] = cache[2] = cache[3] = NULL;
	cpu["$PC"] = getLabel("main");
//
	globl = 0;
//	thread _WB(std::bind(&Program::WB, this)); _WB.detach();
//	thread _MA(std::bind(&Program::MA, this)); _MA.detach();
//	thread _EX(std::bind(&Program::EX, this)); _EX.detach();
//	thread _ID(std::bind(&Program::ID, this)); _ID.detach();
//	thread _IF(std::bind(&Program::IF, this)); _IF.detach();
//	std::ofstream fout("/Users/fengsiyuan/Onedrive/OI/SJTU/mips/Test.out");

	while (globl == 0)
	{
		cpu.cleantmp(0), cpu.cleantmp(1);
		WB();
		MA();
		EX();
		ID();
		IF();
//		for (int i = 0; i < 35; i++) fout << cpu[i] << " ";
//		fout<<endl;
	}
//	cerr << endl << clocks << endl;
	return globl_return;
}
