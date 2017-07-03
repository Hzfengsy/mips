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
	int data[3];
	char state[3];
//	OP op;
	int cache;
public:
	statement(Program *_pro);
	bool loadcache(int index);
	bool loadcache(string st, int i);
	void lockcache(int index);
	void lockcacheimm(int index);
	void lockcache(string st);
	void writecache(int index, int x);
	void writecacheimm(int index, int x);
	static statement* IF(Program *_pro);
	virtual statement* ID() = 0;
	virtual statement* EX() = 0;
	virtual statement* MA() = 0;
	virtual statement* WB() = 0;
	virtual ~statement() {}
};

class binary : public statement
{
public:
	binary(Program *pro) : statement(pro) {}
	virtual statement* ID();
	virtual statement* EX();
	virtual statement* MA();
	virtual statement* WB();
	virtual ~binary() {}
};

class complex : public statement
{
protected:
	int cache1, cache2;
public:
	complex(Program *pro) : statement(pro) {}
	virtual statement* ID();
	virtual statement* EX();
	virtual statement* MA();
	virtual statement* WB();
	virtual ~complex() {}
};

class bbase : public statement
{
public:
	bbase(Program *pro) : statement(pro) {}
	virtual statement* ID();
	virtual statement* EX();
	virtual statement* MA();
	virtual statement* WB();
	virtual ~bbase() {}
};

class bbasez : public bbase
{
public:
	bbasez(Program *pro) : bbase(pro) {}
	virtual statement* ID();
	virtual ~bbasez() {}
};

class lbase : public statement
{
public:
	lbase(Program *pro) : statement(pro) {}
	virtual statement* ID();
	virtual statement* EX();
	virtual statement* MA();
	virtual statement* WB();
	virtual ~lbase() {}
};

class add : public binary
{
public:
	add(Program *pro) : binary(pro) {}
	statement* EX();
};
class sub : public binary
{
public:
	sub(Program *pro) : binary(pro) {}
	statement* EX();
};

class mul : public complex
{
public:
	mul(Program *pro) : complex(pro) {}
	statement* EX();
};
class mulu : public complex
{
public:
	mulu(Program *pro) : complex(pro) {}
	statement* EX();
};
class Div : public complex
{
public:
	Div(Program *pro) : complex(pro) {}
	statement* EX();
};
class Divu : public complex
{
public:
	Divu(Program *pro) : complex(pro) {}
	statement* EX();
};

class Xor : public binary
{
public:
	Xor(Program *pro) : binary(pro) {}
	statement* EX();
};

class neg : public statement
{
public:
	neg(Program *pro) : statement(pro) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};

class rem : public binary
{
public:
	rem(Program *pro) : binary(pro) {}
	statement* EX();
};
class remu : public binary
{
public:
	remu(Program *pro) : binary(pro) {}
	statement* EX();
};

class li : public statement
{
public:
	li(Program *pro) : statement(pro) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};

class seq : public binary
{
public:
	seq(Program *pro) : binary(pro) {}
	statement* EX();
};
class sge : public binary
{
public:
	sge(Program *pro) : binary(pro) {}
	statement* EX();
};
class sgt : public binary
{
public:
	sgt(Program *pro) : binary(pro) {}
	statement* EX();
};
class sle : public binary
{
public:
	sle(Program *pro) : binary(pro) {}
	statement* EX();
};
class slt : public binary
{
public:
	slt(Program *pro) : binary(pro) {}
	statement* EX();
};
class sne : public binary
{
public:
	sne(Program *pro) : binary(pro) {}
	statement* EX();
};

class jmp : public statement
{
public:
	jmp(Program *pro) : statement(pro) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};
class jmpl : public statement
{
public:
	jmpl(Program *pro) : statement(pro) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};

class beq : public bbase
{
public:
	beq(Program *pro) : bbase(pro) {}
	statement* EX();
};
class bne : public bbase
{
public:
	bne(Program *pro) : bbase(pro) {}
	statement* EX();
};
class bge : public bbase
{
public:
	bge(Program *pro) : bbase(pro) {}
	statement* EX();
};
class ble : public bbase
{
public:
	ble(Program *pro) : bbase(pro) {}
	statement* EX();
};

class bgt : public bbase
{
public:
	bgt(Program *pro) : bbase(pro) {}
	statement* EX();
};
class blt : public bbase
{
public:
	blt(Program *pro) : bbase(pro) {}
	statement* EX();
};

class beqz : public bbasez
{
public:
	beqz(Program *pro) : bbasez(pro) {}
	statement* EX();
};
class bnez : public bbasez
{
public:
	bnez(Program *pro) : bbasez(pro) {}
	statement* EX();
};
class bgez : public bbasez
{
public:
	bgez(Program *pro) : bbasez(pro) {}
	statement* EX();
};
class blez : public bbasez
{
public:
	blez(Program *pro) : bbasez(pro) {}
	statement* EX();
};

class bgtz : public bbasez
{
public:
	bgtz(Program *pro) : bbasez(pro) {}
	statement* EX();
};
class bltz : public bbasez
{
public:
	bltz(Program *pro) : bbasez(pro) {}
	statement* EX();
};

class load : public lbase
{
	int num;
public:
	load(Program *pro, int t) : lbase(pro), num(t) {}
	statement* MA();
};

class la : public lbase
{
public:
	la(Program *pro) : lbase(pro) {}
	statement* EX();
};

class store : public statement
{
	int num;
public:
	store(Program *pro, int t) : statement(pro), num(t) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};

class Move : public statement
{
public:
	Move(Program *pro) : statement(pro) {}
	virtual statement* ID();
	virtual statement* EX();
	virtual statement* MA();
	virtual statement* WB();
};

class mfhi : public Move
{
public:
	mfhi(Program *pro) : Move(pro) {}
	statement* ID();
};

class mflo : public Move
{
public:
	mflo(Program *pro) : Move(pro) {}
	statement* ID();
};

class nop : public statement
{
public:
	nop(Program *pro) : statement(pro) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};

class syscall : public statement
{
	string cachestr;
public:
	syscall(Program *pro) : statement(pro) {}
	statement* ID();
	statement* EX();
	statement* MA();
	statement* WB();
};
#endif /* statement_hpp */
