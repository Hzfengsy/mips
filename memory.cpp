//
//  memory.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "memory.hpp"

Memory::Memory()
{
	Q = new char[4 * 1024 *1024]();
}

Memory::~Memory()
{
	delete[] Q;
}

int Memory::load(int address, int t)
{
	int ans = 0;
	for (int i = 0; i < t; i++)
	{
//		if (address + i >= Q.size()) throw index_out_of_bound();
		ans|= (Q[address + i] & 0xff) << (8 * i);
	}
	
	return ans;
}

void Memory::store(int address, int x, int t)
{
	for (int i = 0; i < t; i++)
	{
//		if (address + i >= Q.size()) throw index_out_of_bound();
		Q[address + i] = (x >> (8*i)) & 0xff;
	}
}

int Memory::getaddress(string Label)
{
	if (label.find(Label) == label.end()) throw invalid_identifier();
	return label[Label];
}

void Memory::setLabel(string Label, int address)
{
	if (label.find(Label) != label.end()) throw invalid_identifier();
	label[Label] = address;
}

string Memory::getstring(int address)
{
	string ans = "";
	for (; Q[address] != 0; address++) ans += Q[address];
	return ans;
}
