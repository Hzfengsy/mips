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

command::command(const string &_op, Program &_pro, Memory &_mem) : op(_op), pro(_pro), mem(_mem) {}

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

void Program::push_back(const command &x)
{
	commands.push_back(x);
}

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

command* Program::getcommand(int index) { return new command(commands[index]); }

command* Program::getcommand(const string &label) { return new command(commands[getLabel(label)]); }

void Program::IF()
{
	if (cache[0] != NULL) return;
	statement *st = new statement(cpu, this, &mem, is, os);
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
	cache[3]->delcom();	delete cache[3]; cache[3] = NULL;
}

int Program::run()
{
	cache[0] = cache[1] = cache[2] = cache[3] = NULL;
//	mem.lock = 0;
	cpu["$PC"] = getLabel("main");
	int i = 0;
	std::ofstream fout("/Users/fengsiyuan/Onedrive/OI/SJTU/mips/Test.out");
	while (1)
	{
		WB();
		MA();
		EX();
		ID();
		IF();
		if (hazard == -1) return 0;
		if (hazard == -2) return cpu["$a0"];
		fout << i++ << endl;
	}
	return 0;
}
