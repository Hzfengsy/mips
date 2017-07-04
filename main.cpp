//
//  main.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Text.hpp"
#include "program.hpp"
#include "memory.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	Memory memory;
	CPU cpu;
//	ifstream f("/Users/fengsiyuan/Onedrive/OI/SJTU/mips/mips/testsuit-1/array_test1-mahaojun.in");
	Program program(cpu, memory, cin, cout);
	CPU::inti();
	ifstream fin("/Users/fengsiyuan/Onedrive/OI/SJTU/mips/mips/testsuit-1/pi-5090379042-jiaxiao.s");
//	ifstream fin(argv[1]);
	Decoder text(fin, memory, program);
	text.process();
	return program.run();
	return 0;
}
