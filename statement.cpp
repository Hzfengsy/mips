//
//  statement.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "statement.hpp"

statement::statement(Program *_pro)	: pro(_pro), cpu(_pro->cpu), mem(pro->mem) {}

statement* statement::IF()
{
	if (pro->hazard) return NULL;
	op = pro->getcommand(cpu[32]++, data, state);
//	cerr << cpu["$PC"] - 1 << " " << op << " " << data[0] << " " << data[1] << endl;
	return this;
}

inline bool statement::loadcache(int i)
{
	if (state[i] == 2) return 1;
	if (!cpu.valid(data[i])) return 0;
//	int t = data[i];
	data[i] = cpu[data[i]];
	state[i] = 2;
	return 1;
}

inline bool statement::loadcache(string st, int i)
{
	if (!cpu.valid(st)) return 0;
	data[i] = cpu[st];
	state[i] = 2;
	return 1;
}

inline void statement::lockcache(int i) { cpu.setused(data[i], 1); }

inline void statement::lockcache(string st) { cpu.setused(st, 1); }

inline void statement::writecache(int i, int x)
{
	cpu[data[i]] = x;
	cpu.setused(data[i], 0);
}

inline void statement::writecacheimm(int i, int x)
{
	cpu[i] = x;
	cpu.setused(i, 0);
}

statement* statement::ID()
{
	switch (op)
	{
		case ADD:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new add(*this);
			break;
		case SUB:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new sub(*this);
			break;
		case MUL:
		case MULU:
		case DIV:
		case DIVU:
			if (state[2] == 0)
			{
				if (!loadcache(0) || !loadcache(1)) return NULL;
				lockcache("$HI"), lockcache("$LO");
			}
			else
			{
				if (!loadcache(1) || !loadcache(2)) return NULL;
				lockcache(0);
			}
			if (op == MUL) return new mul(*this);
			if (op == MULU) return new mulu(*this);
			if (op == DIV) return new Div(*this);
			if (op == DIVU) return new Divu(*this);
			break;
		case XOR:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new Xor(*this);
			break;
		case NEG:
			if (!loadcache(1)) return NULL;
			lockcache(0);
			return new neg(*this);
			break;
		case REM:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new rem(*this);
			break;
		case REMU:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new remu(*this);
			break;
		case LI:
			lockcache(0);
			return new li(*this);
			break;
		case SEQ:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new seq(*this);
			break;
		case SGE:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new sge(*this);
			break;
		case SGT:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new sgt(*this);
			break;
		case SLE:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new sle(*this);
			break;
		case SLT:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new slt(*this);
			break;
		case SNE:
			if (!loadcache(1) || !loadcache(2)) return NULL;
			lockcache(0);
			return new sne(*this);
			break;
		case JMP:
			if (!loadcache(0)) return NULL;
			pro->hazard = 1;
			lockcache("$PC");
			return new jmp(*this);
			break;
		case JMPL:
			if (!loadcache(0) || !loadcache("$PC", 1)) return NULL;
			pro->hazard = 1;
			lockcache("$PC");
			lockcache("$ra");
			return new jmpl(*this);
			break;
		case BEQ:
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$PC");
			pro->hazard = 1;
			return new beq(*this);
			break;
		case BNE:
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$PC");
			pro->hazard = 1;
			return new bne(*this);
			break;
		case BGE:
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$PC");
			pro->hazard = 1;
			return new bge(*this);
			break;
		case BLE:
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$PC");
			pro->hazard = 1;
			return new ble(*this);
			break;
		case BGT:
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$PC");
			pro->hazard = 1;
			return new bgt(*this);
			break;
		case BLT:
			if (!loadcache(0) || !loadcache(1)) return NULL;
			lockcache("$PC");
			pro->hazard = 1;
			return new blt(*this);
			break;
		case BEQZ:
			if (!loadcache(0)) return NULL;
			data[2] = data[1];
			data[1] = 0;
			lockcache("$PC");
			pro->hazard = 1;
			return new beq(*this);
			break;
		case BNEZ:
			if (!loadcache(0)) return NULL;
			data[2] = data[1];
			data[1] = 0;
			lockcache("$PC");
			pro->hazard = 1;
			return new bne(*this);
			break;
		case BGEZ:
			if (!loadcache(0)) return NULL;
			data[2] = data[1];
			data[1] = 0;
			lockcache("$PC");
			pro->hazard = 1;
			return new bge(*this);
			break;
		case BLEZ:
			if (!loadcache(0)) return NULL;
			data[2] = data[1];
			data[1] = 0;
			lockcache("$PC");
			pro->hazard = 1;
			return new ble(*this);
			break;
		case BGTZ:
			if (!loadcache(0)) return NULL;
			data[2] = data[1];
			data[1] = 0;
			lockcache("$PC");
			pro->hazard = 1;
			return new bgt(*this);
			break;
		case BLTZ:
			if (!loadcache(0)) return NULL;
			data[2] = data[1];
			data[1] = 0;
			lockcache("$PC");
			pro->hazard = 1;
			return new blt(*this);
			break;
		case LA:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			lockcache(0);
			return new la(*this);
			break;
		case LB:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			lockcache(0);
			return new load(*this, 1);
			break;
		case LH:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			lockcache(0);
			return new load(*this, 2);
			break;
		case LW:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			lockcache(0);
			return new load(*this, 4);
			break;
		case SB:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			if (!loadcache(0)) return NULL;
			return new store(*this, 1);
			break;
		case SH:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			if (!loadcache(0)) return NULL;
			return new store(*this, 2);
			break;
		case SW:
			if (state[2] == 0) data[2] = 0;
			else if (!loadcache(2)) return NULL;
			if (!loadcache(0)) return NULL;
			return new store(*this, 4);
			break;
		case MOVE:
			if (!loadcache(1)) return NULL;
			lockcache(0);
			return new Move(*this);
			break;
		case MFHI:
			if (!loadcache("$HI", 1)) return NULL;
			lockcache(0);
			return new Move(*this);
			break;
		case MFLO:
			if (!loadcache("$LO", 1)) return NULL;
			lockcache(0);
			return new Move(*this);
			break;
		case NOP:
			return new nop(*this);
		case SYS:
			data[0] = 2;
			data[1] = 4;
			data[2] = 5;
			state[0] = state[1] = state[2] = 1;
			if (!loadcache(0) || !loadcache(1) || !loadcache(2)) return NULL;
			lockcache("$v0");
			pro->hazard = 1;
			return new syscall(*this);
			break;
		default:
			throw invalid_program();
			break;
	}
	return NULL;
}

statement* statement::EXEC() { throw runtime_error(); return this; }
statement* statement::MA() { throw runtime_error(); return this; }
statement* statement::WB() { throw runtime_error(); return this; }

add::add(const statement &x) : statement(x) {}
statement* add::EXEC() { cache = data[1] + data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* add::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* add::WB() { writecache(0, cache); return this; }

sub::sub(const statement &x) : statement(x) {}
statement* sub::EXEC() { cache = data[1] - data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* sub::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* sub::WB() { writecache(0, cache); return this; }

mul::mul(const statement &x) : statement(x) {}
statement* mul::EXEC()
{
	ll ans;
	if (state[2] == 0) ans = (ll)data[0] * data[1];
	else ans = (ll)data[1] * data[2];
	cache1 = ans >> 32 & 0xffffffff;
	cache2 = ans & 0xffffffff;
	if (state[2] != 0) cpu.writetmp(0, data[0], cache2);
	return this;
}
statement* mul::MA() { if (state[2] != 0) cpu.writetmp(1, data[0], cache2); return this; }
statement* mul::WB()
{
	if (state[2] == 0) writecacheimm(33, cache1), writecacheimm(34, cache2);
	else writecache(0, cache2);
	return this;
}

mulu::mulu(const statement &x) : statement(x) {}
statement* mulu::EXEC()
{
	ull ans;
	if (state[2] == 0) ans = (ull)data[0] * (ull)data[1];
	else ans = (ull)data[1] * (ull)data[2];
	cache1 = ans >> 32 & 0xffffffff;
	cache2 = ans & 0xffffffff;
	if (state[2] != 0) cpu.writetmp(0, data[0], cache2);
	return this;
}
statement* mulu::MA() { if (state[2] != 0) cpu.writetmp(1, data[0], cache2); return this; }
statement* mulu::WB()
{
	if (state[2] == 0) writecacheimm(33, cache1), writecacheimm(34, cache2);
	else writecache(0, cache2);
	return this;
}

Div::Div(const statement &x) : statement(x) {}
statement* Div::EXEC()
{
	if (state[2] == 0) cache1 = data[0] % data[1], cache2 = data[0] / data[1];
	else cache1 = data[1] % data[2], cache2 = data[1] / data[2];
	if (state[2] != 0) cpu.writetmp(0, data[0], cache2);
	return this;
}
statement* Div::MA() { if (state[2] != 0) cpu.writetmp(1, data[0], cache2); return this; }
statement* Div::WB()
{
	if (state[2] == 0) writecacheimm(33, cache1), writecacheimm(34, cache2);
	else writecache(0, cache2);
	return this;
}

Divu::Divu(const statement &x) : statement(x) {}
statement* Divu::EXEC()
{
	if (state[2] == 0) cache1 = (uint)data[0] % (uint)data[1], cache2 = (uint)data[0] / (uint)data[1];
	else cache1 = (uint)data[1] % (uint)data[2], cache2 = (uint)data[1] / (uint)data[2];
	if (state[2] != 0) cpu.writetmp(0, data[0], cache2);
	return this;
}
statement* Divu::MA() { if (state[2] != 0) cpu.writetmp(1, data[0], cache2); return this; }
statement* Divu::WB()
{
	if (state[2] == 0) writecacheimm(33, cache1), writecacheimm(34, cache2);
	else writecache(0, cache2);
	return this;
}

Xor::Xor(const statement &x) : statement(x) {}
statement* Xor::EXEC() { cache = data[1] ^ data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* Xor::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* Xor::WB() { writecache(0, cache); return this; }

neg::neg(const statement &x) : statement(x) {}
statement* neg::EXEC() { cache = -data[1], cpu.writetmp(0, data[0], cache);	return this; }
statement* neg::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* neg::WB() { writecache(0, cache); return this; }

rem::rem(const statement &x) : statement(x) {}
statement* rem::EXEC() { cache = data[1] % data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* rem::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* rem::WB() { writecache(0, cache); return this; }

remu::remu(const statement &x) : statement(x) {}
statement* remu::EXEC() { cache = (uint)data[1] % (uint)data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* remu::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* remu::WB() { writecache(0, cache); return this; }

li::li(const statement &x) : statement(x) {}
statement* li::EXEC() { cpu.writetmp(0, data[0], data[1]); return this; }
statement* li::MA() { cpu.writetmp(1, data[0], data[1]); return this; }
statement* li::WB() { writecache(0, data[1]); return this;}

seq::seq(const statement &x) : statement(x) {}
statement* seq::EXEC() { cache = data[1] == data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* seq::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* seq::WB() { writecache(0, cache); return this; }

sge::sge(const statement &x) : statement(x) {}
statement* sge::EXEC() { cache = data[1] >= data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* sge::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* sge::WB() { writecache(0, cache); return this; }

sgt::sgt(const statement &x) : statement(x) {}
statement* sgt::EXEC() { cache = data[1] > data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* sgt::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* sgt::WB() { writecache(0, cache); return this; }

sle::sle(const statement &x) : statement(x) {}
statement* sle::EXEC() { cache = data[1] <= data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* sle::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* sle::WB() { writecache(0, cache); return this; }

slt::slt(const statement &x) : statement(x) {}
statement* slt::EXEC() { cache = data[1] < data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* slt::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* slt::WB() { writecache(0, cache); return this; }

sne::sne(const statement &x) : statement(x) {}
statement* sne::EXEC() { cache = data[1] != data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* sne::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* sne::WB() { writecache(0, cache); return this; }

jmp::jmp(const statement &x) : statement(x) {}
statement* jmp::EXEC() { return this; }
statement* jmp::MA() { return this; }
statement* jmp::WB() { writecacheimm(32, data[0]); pro->hazard = 0; return this; }

jmpl::jmpl(const statement &x) : statement(x) {}
statement* jmpl::EXEC() { return this; }
statement* jmpl::MA() { return this; }
statement* jmpl::WB()
{
	writecacheimm(31, data[1]);
	writecacheimm(32, data[0]);
	pro->hazard = 0;
	return this;
}

beq::beq(const statement &x) : statement(x) {}
statement* beq::EXEC() { cache = data[0] == data[1]; return this; }
statement* beq::MA() { return this; }
statement* beq::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

bne::bne(const statement &x) : statement(x) {}
statement* bne::EXEC() { cache = data[0] != data[1]; return this; }
statement* bne::MA() { return this; }
statement* bne::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

bge::bge(const statement &x) : statement(x) {}
statement* bge::EXEC() { cache = data[0] >= data[1]; return this; }
statement* bge::MA() { return this; }
statement* bge::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

ble::ble(const statement &x) : statement(x) {}
statement* ble::EXEC() { cache = data[0] <= data[1]; return this; }
statement* ble::MA() { return this; }
statement* ble::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

bgt::bgt(const statement &x) : statement(x) {}
statement* bgt::EXEC() { cache = data[0] > data[1]; return this; }
statement* bgt::MA() { return this; }
statement* bgt::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

blt::blt(const statement &x) : statement(x) {}
statement* blt::EXEC() { cache = data[0] < data[1]; return this; }
statement* blt::MA() { return this; }
statement* blt::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

la::la(const statement &x) : statement(x) {}
statement* la::EXEC() { cache = data[1] + data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* la::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* la::WB() { writecache(0, cache); return this; }

load::load(const statement &x, int t) : statement(x), num(t) {}
statement* load::EXEC() { cache = data[1] + data[2]; return this; }
statement* load::MA() { cache = mem.load(cache, num), cpu.writetmp(1, data[0], cache); return this; }
statement* load::WB() { writecache(0, cache); return this; }

store::store(const statement &x, int t) : statement(x), num(t) {}
statement* store::EXEC() { cache = data[1] + data[2]; return this; }
statement* store::MA() { mem.store(cache, data[0] , num); return this; }
statement* store::WB() { return this; }

Move::Move(const statement &x) : statement(x) {}
statement* Move::EXEC() { cache = data[1], cpu.writetmp(0, data[0], cache); return this; }
statement* Move::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* Move::WB() { writecache(0, cache); return this; }

nop::nop(const statement &x) : statement(x) {}
statement* nop::EXEC() { return this; }
statement* nop::MA() { return this; }
statement* nop::WB() { return this; }

syscall::syscall(const statement &x) : statement(x) {}
statement* syscall::EXEC()
{
	int v0 = data[0];
	if (v0 == 1) pro->os << data[1];
	if (v0 == 4) pro->os << mem.getstring(data[1]);
	if (v0 == 5) pro->is >> cache;
	if (v0 == 8) pro->is >> cachestr;
	if (v0 == 10) pro->globl_return = 0, pro->globl = 1;
	if (v0 == 17) pro->globl_return = data[1], pro->globl = 1;
	return this;
}
statement* syscall::MA()
{
	int v0 = data[0];
	if (v0 == 8)
	{
		int a0 = data[1], a1 = data[2];
		for (int i = 0; i < a1 - 1 && i < cachestr.length(); i++) mem.store(a0 + i, cachestr[i], 1);
	}
	if (v0 == 9)
	{
		int a0 = data[1];
		cache = mem.staticpos;
		for (int i = 0; i < a0; i++, mem.staticpos++) mem.store(mem.staticpos, 0, 0);
	}
	return this;
}
statement* syscall::WB()
{
	int v0 = data[0];
	if (v0 == 5 || v0 == 9) writecacheimm(2, cache);
	else cpu.setused(2, 0);
	pro->hazard = 0;
	return this;
}
