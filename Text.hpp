//
//  Text.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef Text_hpp
#define	Text_hpp

#include <string>
#include <istream>
#include "scanner.hpp"
#include "memory.hpp"
#include "program.hpp"
using std::string;
using std::istream;

class scanner;

class Decoder
{
private:
	bool flag; // 0 - data , 1 - text
	istream &is;
	Memory &memory;
	Program &pro;
	
	void removeComment(string &st);
	void getLabel(string &st);
	
	void align(scanner &scan);
	void ascii(scanner &scan);
	void asciiz(scanner &scan);
	void number(scanner &scan, int n);
	void space(scanner &scan);
	
	bool processLine();
public:
	Decoder(istream &_is, Memory &_memory, Program &_pro);
	
	void process();
};

#endif /* Text_hpp */
