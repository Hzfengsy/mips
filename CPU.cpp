//
//  CPU.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "CPU.hpp"

map<string, int> CPU::cache_code;

CPU::CPU()
{
	for (int i = 0; i < 35; i++) cache[i] = 0;
	cache[32] = 0;
	cache[29] = 4 * 1024 * 1024;
}

void CPU::inti()
{
	cache_code["$0"] = cache_code["$zero"] = 0;
	
	cache_code["$1"] = cache_code["$at"] = 1;
	
	cache_code["$2"] = cache_code["$v0"] = 2;
	cache_code["$3"] = cache_code["$v1"] = 3;
	
	cache_code["$4"] = cache_code["$a0"] = 4;
	cache_code["$5"] = cache_code["$a1"] = 5;
	cache_code["$6"] = cache_code["$a2"] = 6;
	cache_code["$7"] = cache_code["$a3"] = 7;
	
	cache_code["$8"] = cache_code["$t0"] = 8;
	cache_code["$9"] = cache_code["$t1"] = 9;
	cache_code["$10"] = cache_code["$t2"] = 10;
	cache_code["$11"] = cache_code["$t3"] = 11;
	cache_code["$12"] = cache_code["$t4"] = 12;
	cache_code["$13"] =	cache_code["$t5"] = 13;
	cache_code["$14"] = cache_code["$t6"] = 14;
	cache_code["$15"] = cache_code["$t7"] = 15;
	
	cache_code["$16"] = cache_code["$s0"] = 16;
	cache_code["$17"] = cache_code["$s1"] = 17;
	cache_code["$18"] = cache_code["$s2"] = 18;
	cache_code["$19"] = cache_code["$s3"] = 19;
	cache_code["$20"] = cache_code["$s4"] = 20;
	cache_code["$21"] =	cache_code["$s5"] = 21;
	cache_code["$22"] = cache_code["$s6"] = 22;
	cache_code["$23"] = cache_code["$s7"] = 23;
	
	cache_code["$24"] = cache_code["$t8"] = 24;
	cache_code["$25"] = cache_code["$t9"] = 25;
	
	cache_code["$26"] = cache_code["$k0"] = 26;
	cache_code["$27"] = cache_code["$k1"] = 27;
	
	cache_code["$28"] = cache_code["$gp"] = 28;
	
	cache_code["$29"] = cache_code["$sp"] = 29;
	
	cache_code["$30"] = cache_code["$fp"] = 30;
	
	cache_code["$31"] = cache_code["$ra"] = 31;
	
	cache_code["$PC"] = 32;
	
	cache_code["$HI"] = 33;
	
	cache_code["$LO"] = 34;
}

int CPU::exchange(const string &x)
{
	if (cache_code.find(x) == cache_code.end()) throw invalid_identifier();
	return cache_code[x];
}

//bool CPU::valid(const int &x) { return !used[x]; }

//bool CPU::valid(const string &x) { return !used[exchange(x)]; }

//void CPU::setused(const int &x, bool t) { used[x] = t; }

//void CPU::setused(const string &x, bool t) { used[exchange(x)] = t; }

int& CPU::operator[](const int &x) { return cache[x]; }
int& CPU::operator[](const string &x) { return cache[exchange(x)]; }

ostream& operator << (ostream &os, const CPU &cpu)
{
	for (int i = 0; i < 32; i++) os << cpu.cache[i] << " ";
	return os;
}
