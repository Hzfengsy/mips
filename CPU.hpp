//
//  CPU.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef CPU_hpp
#define CPU_hpp

#include <string>
#include "exception.hpp"
#include <map>
#include <iostream>
#include <thread>
using std::string;
using std::map;
using std::ostream;
using std::mutex;

class CPU
{
private:
	int cache[35];
	int used[35];
	static map<string, int> cache_code;
	int id[2], num[2];
	
public:
	CPU();
	
	static void inti();
	
	static int exchange(const string &x);
	
	bool valid(const int &x);
	bool valid(const string &x);
	
	void setused(const int &x, bool t);
	void setused(const string &x, bool t);
	
	int& operator [] (const string &x);
	int& operator [] (const int &x);
	
	void writetmp(int k, int _id, int _num);
	void cleantmp(int k);
	
	friend ostream& operator << (ostream &os, const CPU &cpu);
};


#endif /* CPU_hpp */
