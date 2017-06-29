//
//  statement.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "statement.hpp"

statement::statement(CPU &_cpu, Program *_pro, Memory *_mem, istream &_is, ostream &_os)
	: cpu(_cpu), pro(_pro), mem(_mem), is(_is), os(_os) {}

statement* statement::IF()
{
	if (pro->hazard) return NULL;
//	if (mem->lock) return NULL;
	com = pro->getcommand(cpu["$PC"]);
	cpu["$PC"]++;
//	cerr << cpu["$PC"] - 1 << " " << com->op << endl;
	return this;
}

void statement::delcom() { delete com; }

bool statement::loadcache(int i)
{
	if (com->state[i] != 1 && com->state[i] != 2) throw invalid_program();
	if (com->state[i] == 2) return 1;
	if (!cpu.valid(com->data[i])) return 0;
	com->state[i] = 2;
	com->data[i] = cpu[com->data[i]];
	return 1;
}

bool statement::loadcache(string st, int i)
{
	if (!cpu.valid(st)) return 0;
	com->state[i] = 2;
	com->data[i] = cpu[st];
	return 1;
}

void statement::lockcache(int i)
{
	if (com->state[i] != 1) throw invalid_program();
	cpu.setused(com->data[i], 1);
}

void statement::lockcache(string st)
{
	cpu.setused(st, 1);
}

void statement::writecache(int i, int x)
{
	if (com->state[i] != 1) throw invalid_program();
	cpu[com->data[i]] = x;
	cpu.setused(com->data[i], 0);
}

void statement::writecache(string st, int x)
{
	cpu[st] = x, cpu.setused(st, 0);
}

statement* statement::ID()
{
	if (com->op == "add" || com->op == "addi" || com->op == "addu" || com->op == "addui")
	{
		if (!loadcache(1) || !loadcache(2)) return NULL;
		lockcache(0);
		return new add(*this);
	}
	if (com->op == "sub" || com->op == "subi" || com->op == "subu" || com->op == "subui")
	{
		if (!loadcache(1) || !loadcache(2)) return NULL;
		lockcache(0);
		return new sub(*this);
	}
	if (com->op == "mul" || com->op == "mulu" || com->op == "div" || com->op == "divu")
	{
		if (com->state[2] == 0)
		{
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$HI"), lockcache("$LO");
		}
		else if (com->state[2] == 1 || com->state[2] == 2)
		{
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
		}
		else throw invalid_program();
		if (com->op == "mul") return new mul(*this);
		if (com->op == "mulu") return new mulu(*this);
		if (com->op == "div") return new Div(*this);
		if (com->op == "divu") return new Divu(*this);
		throw invalid_program();
		return NULL;
	}
	if (com->op == "xor" || com->op == "xoru")
	{
		if (!loadcache(1) || !loadcache(2)) return NULL;
		lockcache(0);
		return new Xor(*this);
	}
	if (com->op == "neg" || com->op == "negu")
	{
		if (!loadcache(1)) return NULL;
		lockcache(0);
		return new neg(*this);
	}
	if (com->op == "rem" || com->op == "remu")
	{
		if (!loadcache(1) || !loadcache(2)) return NULL;
		lockcache(0);
		if (com->op == "rem") return new rem(*this);
		else return new remu(*this);
	}
	if (com->op == "li")
	{
		lockcache(0);
		return new li(*this);
	}
	if (com->op == "seq" || com->op == "sge" || com->op == "sgt" ||
		com->op == "sle" || com->op == "slt" || com->op == "sne")
	{
		if (!loadcache(1) || !loadcache(2)) return NULL;
		lockcache(0);
		if (com->op == "seq") return new seq(*this);
		if (com->op == "sge") return new sge(*this);
		if (com->op == "sgt") return new sgt(*this);
		if (com->op == "sle") return new sle(*this);
		if (com->op == "slt") return new slt(*this);
		if (com->op == "sne") return new sne(*this);
		throw invalid_program();
		return NULL;
	}
	if (com->op == "b" || com->op == "j" || com->op == "jr" || com->op == "jal" || com->op == "jalr")
	{
		if (com->state[0] == 4)
		{
			try { com->data[0] = pro->getLabel(com->label); }
			catch(...) { com->data[0] = mem->getaddress(com->label); }
			com->state[0] = 2;
		}
		else if (!loadcache(0)) return NULL;
		pro->hazard = 1;
		if (com->op == "b" || com->op == "j" || com->op == "jr") {lockcache("$PC"); return new jmp(*this);}
		else
		{
			if (!loadcache("$PC", 1)) return NULL;
			lockcache("$PC");
			return new jmpl(*this);
		}
	}
	if (com->op == "beq" || com->op == "bne" || com->op == "bge"
		|| com->op == "ble" || com->op == "bgt" || com->op == "blt")
	{
		if (!loadcache(0) || !loadcache(1)) return NULL;
		try { com->data[2] = pro->getLabel(com->label); }
		catch(...) { com->data[2] = mem->getaddress(com->label); }
		com->state[2] = 2;
		lockcache("$PC");
		pro->hazard = 1;
		if (com->op == "beq") return new beq(*this);
		if (com->op == "bne") return new bne(*this);
		if (com->op == "bge") return new bge(*this);
		if (com->op == "ble") return new ble(*this);
		if (com->op == "bgt") return new bgt(*this);
		if (com->op == "blt") return new blt(*this);
	}
	if (com->op == "beqz" || com->op == "bnez" || com->op == "bgez"
		|| com->op == "blez" || com->op == "bgtz" || com->op == "bltz")
	{
		if (!loadcache(0)) return NULL;
		try { com->data[2] = pro->getLabel(com->label); }
		catch(...) { com->data[2] = mem->getaddress(com->label); }
		com->state[2] = 2;
		com->data[1] = com->state[1] = 0;
		lockcache("$PC");
		pro->hazard = 1;
		if (com->op == "beqz") return new beq(*this);
		if (com->op == "bnez") return new bne(*this);
		if (com->op == "bgez") return new bge(*this);
		if (com->op == "blez") return new ble(*this);
		if (com->op == "bgtz") return new bgt(*this);
		if (com->op == "bltz") return new blt(*this);
	}
	if (com->op == "la" || com->op == "lb" || com->op == "lh" || com->op == "lw")
	{
		if (com->state[1] == 4)
		{
			try { com->data[1] = mem->getaddress(com->label); }
			catch(...) { com->data[1] = pro->getLabel(com->label); }
			com->state[1] = 3;
			com->data[2] = 0, com->state[2] = 2;
		}
		else if (!loadcache(2)) return NULL;
		lockcache(0);
		if (com->op == "la") return new la(*this);
		if (com->op == "lb") return new load(*this, 1);
		if (com->op == "lh") return new load(*this, 2);
		if (com->op == "lw") return new load(*this, 4);
		throw invalid_program();
		return NULL;
	}
	if (com->op == "sb" || com->op == "sh" || com->op == "sw")
	{
		if (com->state[1] == 4)
		{
			try { com->data[1] = mem->getaddress(com->label); }
			catch(...) { com->data[1] = pro->getLabel(com->label); }
			com->state[1] = 3;
			com->data[2] = 0, com->state[2] = 2;
		}
		else if (!loadcache(2)) return NULL;
		if (!loadcache(0)) return NULL;
		if (com->op == "sb") return new store(*this, 1);
		if (com->op == "sh") return new store(*this, 2);
		if (com->op == "sw") return new store(*this, 4);
		throw invalid_program();
		return NULL;
	}
	if (com->op == "move" || com->op == "mfhi" || com->op == "mflo")
	{
		if (com->op == "move")
			if (!loadcache(1)) return NULL;
		if (com->op == "mfhi")
			if (!loadcache("$HI", 1)) return NULL;
		if (com->op == "mflo")
			if (!loadcache("$LO", 1)) return NULL;
		lockcache(0);
		return new Move(*this);
	}
	if (com->op == "nop") return new nop(*this);
	if (com->op == "syscall")
	{
		com->data[0] = CPU::exchange("$v0");
		com->data[1] = CPU::exchange("$a0");
		com->data[2] = CPU::exchange("$a1");
		com->state[0] = com->state[1] = com->state[2] = 1;
		if (!loadcache(0) || !loadcache(1) || !loadcache(2)) return NULL;
		lockcache("$v0");
		pro->hazard = 1;
		return new syscall(*this);
	}
	throw invalid_program();
	return NULL;
}

statement* statement::EXEC() { throw runtime_error(); return this; }
statement* statement::MA() { throw runtime_error(); return this; }
statement* statement::WB() { throw runtime_error(); return this; }

add::add(const statement &x) : statement(x) {}
statement* add::EXEC() { cache = com->data[1] + com->data[2]; return this; }
statement* add::MA() { return this; }
statement* add::WB() { writecache(0, cache); return this; }

sub::sub(const statement &x) : statement(x) {}
statement* sub::EXEC() { cache = com->data[1] - com->data[2]; return this; }
statement* sub::MA() { return this; }
statement* sub::WB() { writecache(0, cache); return this; }

mul::mul(const statement &x) : statement(x) {}
statement* mul::EXEC()
{
	ll ans;
	if (com->state[2] == 0) ans = (ll)com->data[0] * com->data[1];
	else if (com->state[2] == 1 || com->state[2] == 2) ans = (ll)com->data[1] * com->data[2];
	else throw invalid_program();
	cache1 = ans >> 32 & 0xffffffff;
	cache2 = ans & 0xffffffff;
	return this;
}
statement* mul::MA() { return this; }
statement* mul::WB()
{
	if (com->state[2] == 0) writecache("$HI", cache1), writecache("$LO", cache2);
	else if (com->state[2] == 1 || com->state[2] == 2) writecache(0, cache2);
	else throw invalid_program();
	return this;
}

mulu::mulu(const statement &x) : statement(x) {}
statement* mulu::EXEC()
{
	ull ans;
	if (com->state[2] == 0) ans = (ull)com->data[0] * (ull)com->data[1];
	else if (com->state[2] == 1 || com->state[2] == 2) ans = (ull)com->data[1] * (ull)com->data[2];
	else throw invalid_program();
	cache1 = ans >> 32 & 0xffffffff;
	cache2 = ans & 0xffffffff;
	return this;
}
statement* mulu::MA() { return this; }
statement* mulu::WB()
{
	if (com->state[2] == 0) writecache("$HI", cache1), writecache("$LO", cache2);
	else if (com->state[2] == 1 || com->state[2] == 2) writecache(0, cache2);
	else throw invalid_program();
	return this;
}

Div::Div(const statement &x) : statement(x) {}
statement* Div::EXEC()
{
	if (com->state[2] == 0)
		cache1 = com->data[0] % com->data[1], cache2 = com->data[0] / com->data[1];
	else if (com->state[2] == 1 || com->state[2] == 2)
		cache1 = com->data[1] % com->data[2], cache2 = com->data[1] / com->data[2];
	else throw invalid_program();
	return this;
}
statement* Div::MA() { return this; }
statement* Div::WB()
{
	if (com->state[2] == 0) writecache("$HI", cache1), writecache("$LO", cache2);
	else if (com->state[2] == 1 || com->state[2] == 2) writecache(0, cache2);
	else throw invalid_program();
	return this;
}

Divu::Divu(const statement &x) : statement(x) {}
statement* Divu::EXEC()
{
	if (com->state[2] == 0)
		cache1 = (uint)com->data[0] % (uint)com->data[1], cache2 = (uint)com->data[0] / (uint)com->data[1];
	else if (com->state[2] == 1 || com->state[2] == 2)
		cache1 = (uint)com->data[1] % (uint)com->data[2], cache2 = (uint)com->data[1] / (uint)com->data[2];
	else throw invalid_program();
	return this;
}
statement* Divu::MA() { return this; }
statement* Divu::WB()
{
	if (com->state[2] == 0) writecache("$HI", cache1), writecache("$LO", cache2);
	else if (com->state[2] == 1 || com->state[2] == 2) writecache(0, cache2);
	else throw invalid_program();
	return this;
}

Xor::Xor(const statement &x) : statement(x) {}
statement* Xor::EXEC() { cache = com->data[1] ^ com->data[2]; return this; }
statement* Xor::MA() { return this; }
statement* Xor::WB() { writecache(0, cache); return this; }

neg::neg(const statement &x) : statement(x) {}
statement* neg::EXEC()
{
	cache = -com->data[1];
	return this;
}
statement* neg::MA() { return this; }
statement* neg::WB() { writecache(0, cache); return this; }

rem::rem(const statement &x) : statement(x) {}
statement* rem::EXEC() { cache = com->data[1] % com->data[2]; return this; }
statement* rem::MA() { return this; }
statement* rem::WB() { writecache(0, cache); return this; }

remu::remu(const statement &x) : statement(x) {}
statement* remu::EXEC() { cache = (uint)com->data[1] % (uint)com->data[2]; return this; }
statement* remu::MA() { return this; }
statement* remu::WB() { writecache(0, cache); return this; }

li::li(const statement &x) : statement(x) {}
statement* li::EXEC() { return this; }
statement* li::MA() { return this; }
statement* li::WB() { writecache(0, com->data[1]); return this;}

seq::seq(const statement &x) : statement(x) {}
statement* seq::EXEC() { cache = com->data[1] == com->data[2]; return this; }
statement* seq::MA() { return this; }
statement* seq::WB() { writecache(0, cache); return this; }

sge::sge(const statement &x) : statement(x) {}
statement* sge::EXEC() { cache = com->data[1] >= com->data[2]; return this; }
statement* sge::MA() { return this; }
statement* sge::WB() { writecache(0, cache); return this; }

sgt::sgt(const statement &x) : statement(x) {}
statement* sgt::EXEC() { cache = com->data[1] > com->data[2]; return this; }
statement* sgt::MA() { return this; }
statement* sgt::WB() { writecache(0, cache); return this; }

sle::sle(const statement &x) : statement(x) {}
statement* sle::EXEC() { cache = com->data[1] <= com->data[2]; return this; }
statement* sle::MA() { return this; }
statement* sle::WB() { writecache(0, cache); return this; }

slt::slt(const statement &x) : statement(x) {}
statement* slt::EXEC() { cache = com->data[1] < com->data[2]; return this; }
statement* slt::MA() { return this; }
statement* slt::WB() { writecache(0, cache); return this; }

sne::sne(const statement &x) : statement(x) {}
statement* sne::EXEC() { cache = com->data[1] != com->data[2]; return this; }
statement* sne::MA() { return this; }
statement* sne::WB() { writecache(0, cache); return this; }

jmp::jmp(const statement &x) : statement(x) {}
statement* jmp::EXEC() { return this; }
statement* jmp::MA() { return this; }
statement* jmp::WB() { writecache("$PC", com->data[0]); pro->hazard = 0; return this; }

jmpl::jmpl(const statement &x) : statement(x) {}
statement* jmpl::EXEC() { return this; }
statement* jmpl::MA() { return this; }
statement* jmpl::WB()
{
	writecache("$31", com->data[1]);
	writecache("$PC", com->data[0]);
	pro->hazard = 0;
	return this;
}

beq::beq(const statement &x) : statement(x) {}
statement* beq::EXEC() { cache = com->data[0] == com->data[1]; return this; }
statement* beq::MA() { return this; }
statement* beq::WB()
{
	if (cache) writecache("$PC", com->data[2]);
	else cpu.setused("$PC", 0);
	pro->hazard = 0; return this;
}

bne::bne(const statement &x) : statement(x) {}
statement* bne::EXEC() { cache = com->data[0] != com->data[1]; return this; }
statement* bne::MA() { return this; }
statement* bne::WB()
{
	if (cache) writecache("$PC", com->data[2]);
	else cpu.setused("$PC", 0);
	pro->hazard = 0; return this;
}

bge::bge(const statement &x) : statement(x) {}
statement* bge::EXEC() { cache = com->data[0] >= com->data[1]; return this; }
statement* bge::MA() { return this; }
statement* bge::WB()
{
	if (cache) writecache("$PC", com->data[2]);
	else cpu.setused("$PC", 0);
	pro->hazard = 0; return this;
}

ble::ble(const statement &x) : statement(x) {}
statement* ble::EXEC() { cache = com->data[0] <= com->data[1]; return this; }
statement* ble::MA() { return this; }
statement* ble::WB()
{
	if (cache) writecache("$PC", com->data[2]);
	else cpu.setused("$PC", 0);
	pro->hazard = 0; return this;
}

bgt::bgt(const statement &x) : statement(x) {}
statement* bgt::EXEC() { cache = com->data[0] > com->data[1]; return this; }
statement* bgt::MA() { return this; }
statement* bgt::WB()
{
	if (cache) writecache("$PC", com->data[2]);
	else cpu.setused("$PC", 0);
	pro->hazard = 0; return this;
}

blt::blt(const statement &x) : statement(x) {}
statement* blt::EXEC() { cache = com->data[0] < com->data[1]; return this; }
statement* blt::MA() { return this; }
statement* blt::WB()
{
	if (cache) writecache("$PC", com->data[2]);
	else cpu.setused("$PC", 0);
	pro->hazard = 0; return this;
}

la::la(const statement &x) : statement(x) {}
statement* la::EXEC() { cache = com->data[1] + com->data[2]; return this; }
statement* la::MA() { return this; }
statement* la::WB() { writecache(0, cache); return this; }

load::load(const statement &x, int t) : statement(x), num(t) {}
statement* load::EXEC() { cache = com->data[1] + com->data[2]; return this; }
statement* load::MA() { cache = mem->load(cache, num); return this; }
statement* load::WB() { writecache(0, cache); return this; }

store::store(const statement &x, int t) : statement(x), num(t) {}
statement* store::EXEC() { cache = com->data[1] + com->data[2]; return this; }
statement* store::MA() {
	mem->store(cache, com->data[0] , num); return this;
}
statement* store::WB() { return this; }

Move::Move(const statement &x) : statement(x) {}
statement* Move::EXEC() { cache = com->data[1]; return this; }
statement* Move::MA() { return this; }
statement* Move::WB() { writecache(0, cache); return this; }

nop::nop(const statement &x) : statement(x) {}
statement* nop::EXEC() { return this; }
statement* nop::MA() { return this; }
statement* nop::WB() { return this; }

syscall::syscall(const statement &x) : statement(x) {}
statement* syscall::EXEC()
{
	int v0 = com->data[0];
	if (v0 == 1) os << com->data[1];
	if (v0 == 4) os << mem->getstring(com->data[1]);
	if (v0 == 5) is >> cache;
	if (v0 == 8) is >> cachestr;
	if (v0 == 10) pro->hazard = -1;
	if (v0 == 17) pro->hazard = -2;
	return this;
}
statement* syscall::MA()
{
	int v0 = com->data[0];
	if (v0 == 8)
	{
		int a0 = com->data[1], a1 = com->data[2];
		for (int i = 0; i < a1 - 1 && i < cachestr.length(); i++) mem->store(a0 + i, cachestr[i], 1);
	}
	if (v0 == 9)
	{
		int a0 = com->data[1];
		cache = mem->staticpos;
		for (int i = 0; i < a0; i++, mem->staticpos++) mem->store(mem->staticpos, 0, 0);
	}
	return this;
}
statement* syscall::WB()
{
	int v0 = com->data[0];
	if (v0 == 5 || v0 == 9) writecache("$v0", cache);
	else cpu.setused("$v0", 0);
	pro->hazard = 0;
	return this;
}
