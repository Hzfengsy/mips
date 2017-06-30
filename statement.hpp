//
//  statement.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef statement_hpp
#define statement_hpp

#include "program.hpp"
#include "scanner.hpp"
#include "CPU.hpp"
#include <iostream>
using std::ostream;
using std::istream;
using std::endl;
using std::cerr;
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;;


class Program;
class command;
class statement;

class statement
{
protected:
	CPU &cpu;
	Program *pro;
	Memory &mem;
	int data[3], state[3];
	OP op;
public:
	statement(Program *_pro);
	bool loadcache(int index);
	bool loadcache(string st, int i);
	void lockcache(int index);
	void lockcache(string st);
	void writecache(int index, int x);
	void writecacheimm(int index, int x);
	statement* IF();
	statement* ID();
	virtual statement* EXEC();
	virtual statement* MA();
	virtual statement* WB();
};

class add : public statement
{
	int cache;
public:
	add(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class sub : public statement
{
	int cache;
public:
	sub(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class mul : public statement
{
	int cache1, cache2;
public:
	mul(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class mulu : public statement
{
	int cache1, cache2;
public:
	mulu(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class Div : public statement
{
	int cache1, cache2;
public:
	Div(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class Divu : public statement
{
	int cache1, cache2;
public:
	Divu(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class Xor : public statement
{
	int cache;
public:
	Xor(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class neg : public statement
{
	int cache;
public:
	neg(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class rem : public statement
{
	int cache;
public:
	rem(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class remu : public statement
{
	int cache;
public:
	remu(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class li : public statement
{
public:
	li(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class seq : public statement
{
	int cache;
public:
	seq(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class sge : public statement
{
	int cache;
public:
	sge(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class sgt : public statement
{
	int cache;
public:
	sgt(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class sle : public statement
{
	int cache;
public:
	sle(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class slt : public statement
{
	int cache;
public:
	slt(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class sne : public statement
{
	int cache;
public:
	sne(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class jmp : public statement
{
public:
	jmp(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class jmpl : public statement
{
public:
	jmpl(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class beq : public statement
{
	int cache;
public:
	beq(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class bne : public statement
{
	int cache;
public:
	bne(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class bge : public statement
{
	int cache;
public:
	bge(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class ble : public statement
{
	int cache;
public:
	ble(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class bgt : public statement
{
	int cache;
public:
	bgt(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class blt : public statement
{
	int cache;
public:
	blt(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class load : public statement
{
	int cache, num;
public:
	load(const statement &x, int t);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class la : public statement
{
	int cache;
public:
	la(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class store : public statement
{
	int cache, num;
public:
	store(const statement &x, int t);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class Move : public statement
{
	int cache;
public:
	Move(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class nop : public statement
{
public:
	nop(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};

class syscall : public statement
{
	int cache;
	string cachestr;
public:
	syscall(const statement &x);
	statement* EXEC();
	statement* MA();
	statement* WB();
};
#endif /* statement_hpp */
