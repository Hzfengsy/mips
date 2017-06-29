//
//  memory.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef memory_hpp
#define memory_hpp

#include <stdio.h>
#include <deque>
#include <string>
#include <map>
#include "exception.hpp"
using std::deque;
using std::string;
using std::map;

class Memory
{
private:
	const int MaxSize = 4 * 1024 * 1024;
	
	map<string, int> label;
public:
	char *Q;
	int staticpos = 0;
	
	bool lock = 0;
	
	Memory();
	
	~Memory();
	
	int load(int address, int t);
	
	void store(int address, int x, int t);
	
	int getaddress(string Label);
	
	void setLabel(string Label, int address);
	
	string getstring(int address);
};

#endif /* memory_hpp */
