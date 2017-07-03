//
//  statement.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "statement.hpp"

statement::statement(Program *_pro)	: pro(_pro), cpu(_pro->cpu), mem(pro->mem) {}

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

statement* statement::IF(Program *pro)
{
	statement *t;
	OP op = pro->getOp(pro->cpu[32]);
	
	//	cerr << cpu["$PC"] - 1 << " " << op << " " << data[0] << " " << data[1] << endl;
	switch (op) {
		case ADD:
			t = new add(pro);
			break;
		case SUB:
			t = new sub(pro);
			break;
		case MUL:
			t = new mul(pro);
			break;
		case MULU:
			t = new mulu(pro);
			break;
		case DIV:
			t = new Div(pro);
			break;
		case DIVU:
			t = new Divu(pro);
			break;
		case XOR:
			t = new Xor(pro);
			break;
		case NEG:
			t = new neg(pro);
			break;
		case REM:
			t = new rem(pro);
			break;
		case REMU:
			t = new remu(pro);
			break;
		case LI:
			t = new li(pro);
			break;
		case SEQ:
			t = new seq(pro);
			break;
		case SGE:
			t = new sge(pro);
			break;
		case SGT:
			t = new sgt(pro);
			break;
		case SLE:
			t = new sle(pro);
			break;
		case SLT:
			t = new slt(pro);
			break;
		case SNE:
			t = new sne(pro);
			break;
		case JMP:
			t = new jmp(pro);
			break;
		case JMPL:
			t = new jmpl(pro);
			break;
		case BEQ:
			t = new beq(pro);
			break;
		case BNE:
			t = new bne(pro);
			break;
		case BGE:
			t = new bge(pro);
			break;
		case BLE:
			t = new ble(pro);
			break;
		case BGT:
			t = new bgt(pro);
			break;
		case BLT:
			t = new blt(pro);
			break;
		case BEQZ:
			t = new beqz(pro);
			break;
		case BNEZ:
			t = new bnez(pro);
			break;
		case BGEZ:
			t = new bgez(pro);
			break;
		case BLEZ:
			t = new blez(pro);
			break;
		case BGTZ:
			t = new bgtz(pro);
			break;
		case BLTZ:
			t = new bltz(pro);
			break;
		case LA:
			t = new la(pro);
			break;
		case LB:
			t = new load(pro, 1);
			break;
		case LH:
			t = new load(pro, 2);
			break;
		case LW:
			t = new load(pro, 4);
			break;
		case SB:
			t = new store(pro, 1);
			break;
		case SH:
			t = new store(pro, 2);
			break;
		case SW:
			t = new store(pro, 4);
			break;
		case MOVE:
			t = new Move(pro);
			break;
		case MFHI:
			t = new mfhi(pro);
			break;
		case MFLO:
			t = new mflo(pro);
			break;
		case NOP:
			t = new nop(pro);
		case SYS:
			t = new syscall(pro);
			break;
		default:
			throw invalid_program();
			break;
	}
	t->op = pro->getcommand(pro->cpu[32]++, t->data, t->state);
	return t;
}

//statement* statement::ID()
//{
//	switch (op)
//	{
//		case ADD:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new add(*this);
//			break;
//		case SUB:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new sub(*this);
//			break;
//		case MUL:
//		case MULU:
//		case DIV:
//		case DIVU:
//			if (state[2] == 0)
//			{
//				if (!loadcache(0) || !loadcache(1)) return NULL;
//				lockcache("$HI"), lockcache("$LO");
//			}
//			else
//			{
//				if (!loadcache(1) || !loadcache(2)) return NULL;
//				lockcache(0);
//			}
//			if (op == MUL) return new mul(*this);
//			if (op == MULU) return new mulu(*this);
//			if (op == DIV) return new Div(*this);
//			if (op == DIVU) return new Divu(*this);
//			break;
//		case XOR:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new Xor(*this);
//			break;
//		case NEG:
//			if (!loadcache(1)) return NULL;
//			lockcache(0);
//			return new neg(*this);
//			break;
//		case REM:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new rem(*this);
//			break;
//		case REMU:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new remu(*this);
//			break;
//		case LI:
//			lockcache(0);
//			return new li(*this);
//			break;
//		case SEQ:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new seq(*this);
//			break;
//		case SGE:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new sge(*this);
//			break;
//		case SGT:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new sgt(*this);
//			break;
//		case SLE:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new sle(*this);
//			break;
//		case SLT:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new slt(*this);
//			break;
//		case SNE:
//			if (!loadcache(1) || !loadcache(2)) return NULL;
//			lockcache(0);
//			return new sne(*this);
//			break;
//		case JMP:
//			if (!loadcache(0)) return NULL;
//			pro->hazard = 1;
//			lockcache("$PC");
//			return new jmp(*this);
//			break;
//		case JMPL:
//			if (!loadcache(0) || !loadcache("$PC", 1)) return NULL;
//			pro->hazard = 1;
//			lockcache("$PC");
//			lockcache("$ra");
//			return new jmpl(*this);
//			break;
//		case BEQ:
//			if (!loadcache(0) || !loadcache(1)) return NULL;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new beq(*this);
//			break;
//		case BNE:
//			if (!loadcache(0) || !loadcache(1)) return NULL;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new bne(*this);
//			break;
//		case BGE:
//			if (!loadcache(0) || !loadcache(1)) return NULL;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new bge(*this);
//			break;
//		case BLE:
//			if (!loadcache(0) || !loadcache(1)) return NULL;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new ble(*this);
//			break;
//		case BGT:
//			if (!loadcache(0) || !loadcache(1)) return NULL;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new bgt(*this);
//			break;
//		case BLT:
//			if (!loadcache(0) || !loadcache(1)) return NULL;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new blt(*this);
//			break;
//		case BEQZ:
//			if (!loadcache(0)) return NULL;
//			data[2] = data[1];
//			data[1] = 0;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new beq(*this);
//			break;
//		case BNEZ:
//			if (!loadcache(0)) return NULL;
//			data[2] = data[1];
//			data[1] = 0;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new bne(*this);
//			break;
//		case BGEZ:
//			if (!loadcache(0)) return NULL;
//			data[2] = data[1];
//			data[1] = 0;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new bge(*this);
//			break;
//		case BLEZ:
//			if (!loadcache(0)) return NULL;
//			data[2] = data[1];
//			data[1] = 0;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new ble(*this);
//			break;
//		case BGTZ:
//			if (!loadcache(0)) return NULL;
//			data[2] = data[1];
//			data[1] = 0;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new bgt(*this);
//			break;
//		case BLTZ:
//			if (!loadcache(0)) return NULL;
//			data[2] = data[1];
//			data[1] = 0;
//			lockcache("$PC");
//			pro->hazard = 1;
//			return new blt(*this);
//			break;
//		case LA:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			lockcache(0);
//			return new la(*this);
//			break;
//		case LB:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			lockcache(0);
//			return new load(*this, 1);
//			break;
//		case LH:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			lockcache(0);
//			return new load(*this, 2);
//			break;
//		case LW:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			lockcache(0);
//			return new load(*this, 4);
//			break;
//		case SB:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			if (!loadcache(0)) return NULL;
//			return new store(*this, 1);
//			break;
//		case SH:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			if (!loadcache(0)) return NULL;
//			return new store(*this, 2);
//			break;
//		case SW:
//			if (state[2] == 0) data[2] = 0;
//			else if (!loadcache(2)) return NULL;
//			if (!loadcache(0)) return NULL;
//			return new store(*this, 4);
//			break;
//		case MOVE:
//			if (!loadcache(1)) return NULL;
//			lockcache(0);
//			return new Move(*this);
//			break;
//		case MFHI:
//			if (!loadcache("$HI", 1)) return NULL;
//			lockcache(0);
//			return new Move(*this);
//			break;
//		case MFLO:
//			if (!loadcache("$LO", 1)) return NULL;
//			lockcache(0);
//			return new Move(*this);
//			break;
//		case NOP:
//			return new nop(*this);
//		case SYS:
//			data[0] = 2;
//			data[1] = 4;
//			data[2] = 5;
//			state[0] = state[1] = state[2] = 1;
//			if (!loadcache(0) || !loadcache(1) || !loadcache(2)) return NULL;
//			lockcache("$v0");
//			pro->hazard = 1;
//			return new syscall(*this);
//			break;
//		default:
//			throw invalid_program();
//			break;
//	}
//	return NULL;
//}

statement* statement::EX() { throw runtime_error(); return this; }
statement* statement::MA() { throw runtime_error(); return this; }
statement* statement::WB() { throw runtime_error(); return this; }

statement* binary::ID()
{
	if (!loadcache(1) || !loadcache(2)) return NULL;
	lockcache(0);
	return this;
}
statement* binary::EX() { cpu.writetmp(0, data[0], cache); return this; }
statement* binary::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* binary::WB() { writecache(0, cache); return this; }

statement* bbase::ID()
{
	if (!loadcache(0) || !loadcache(1)) return NULL;
	lockcache("$PC");
	pro->hazard = 1;
	return this;
}
statement* bbase::EX() { return this; }
statement* bbase::MA() { return this; }
statement* bbase::WB()
{
	if (cache) writecacheimm(32, data[2]);
	else cpu.setused(32, 0);
	pro->hazard = 0;
	return this;
}

statement* bbasez::ID()
{
	if (!loadcache(0)) return NULL;
	data[2] = data[1];
	data[1] = 0;
	lockcache("$PC");
	pro->hazard = 1;
	return this;
}

statement* complex::ID()
{
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
	return this;
}
statement* complex::EX() { if (state[2] != 0) cpu.writetmp(0, data[0], cache2); return this; }
statement* complex::MA() { if (state[2] != 0) cpu.writetmp(1, data[0], cache2); return this; }
statement* complex::WB()
{
	if (state[2] == 0) writecacheimm(33, cache1), writecacheimm(34, cache2);
	else writecache(0, cache2);
	return this;}

statement* lbase::ID()
{
	if (state[2] == 0) data[2] = 0;
	else if (!loadcache(2)) return NULL;
	lockcache(0);
	return this;
}
statement* lbase::EX() { cache = data[1] + data[2]; return this; }
statement* lbase::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* lbase::WB() { writecache(0, cache); return this; }

statement* add::EX() { cache = data[1] + data[2]; return binary::EX(); }
statement* sub::EX() { cache = data[1] - data[2]; return binary::EX(); }

statement* mul::EX()
{
	ll ans;
	if (state[2] == 0) ans = (ll)data[0] * data[1];
	else ans = (ll)data[1] * data[2];
	cache1 = ans >> 32 & 0xffffffff;
	cache2 = ans & 0xffffffff;
	return complex::EX();
}

statement* mulu::EX()
{
	ull ans;
	if (state[2] == 0) ans = (ull)data[0] * (ull)data[1];
	else ans = (ull)data[1] * (ull)data[2];
	cache1 = ans >> 32 & 0xffffffff;
	cache2 = ans & 0xffffffff;
	return complex::EX();

}
statement* Div::EX()
{
	if (state[2] == 0) cache1 = data[0] % data[1], cache2 = data[0] / data[1];
	else cache1 = data[1] % data[2], cache2 = data[1] / data[2];
	return complex::EX();
}
statement* Divu::EX()
{
	if (state[2] == 0) cache1 = (uint)data[0] % (uint)data[1], cache2 = (uint)data[0] / (uint)data[1];
	else cache1 = (uint)data[1] % (uint)data[2], cache2 = (uint)data[1] / (uint)data[2];
	return complex::EX();

}

statement* Xor::EX() { cache = data[1] ^ data[2]; return binary::EX(); }

statement* neg::ID()
{
	if (!loadcache(1)) return NULL;
	lockcache(0);
	return this;
}
statement* neg::EX() { cache = -data[1], cpu.writetmp(0, data[0], cache);	return this; }
statement* neg::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* neg::WB() { writecache(0, cache); return this; }

statement* rem::EX() { cache = data[1] % data[2]; return binary::EX(); }
statement* remu::EX() { cache = (uint)data[1] % (uint)data[2]; return binary::EX(); }

statement* li::ID() { lockcache(0); return this; }
statement* li::EX() { cpu.writetmp(0, data[0], data[1]); return this; }
statement* li::MA() { cpu.writetmp(1, data[0], data[1]); return this; }
statement* li::WB() { writecache(0, data[1]); return this;}

statement* seq::EX() { cache = data[1] == data[2]; return binary::EX(); }
statement* sge::EX() { cache = data[1] >= data[2]; return binary::EX(); }
statement* sgt::EX() { cache = data[1] > data[2];  return binary::EX(); }
statement* sle::EX() { cache = data[1] <= data[2]; return binary::EX(); }
statement* slt::EX() { cache = data[1] < data[2];  return binary::EX(); }
statement* sne::EX() { cache = data[1] != data[2]; return binary::EX(); }

statement* jmp::ID()
{
	if (!loadcache(0)) return NULL;
	pro->hazard = 1;
	lockcache("$PC");
	return this;
}
statement* jmp::EX() { return this; }
statement* jmp::MA() { return this; }
statement* jmp::WB() { writecacheimm(32, data[0]); pro->hazard = 0; return this; }

statement* jmpl::ID()
{
	if (!loadcache(0) || !loadcache("$PC", 1)) return NULL;
	pro->hazard = 1;
	lockcache("$PC");
	lockcache("$ra");
	return this;
}
statement* jmpl::EX() { return this; }
statement* jmpl::MA() { return this; }
statement* jmpl::WB()
{
	writecacheimm(31, data[1]);
	writecacheimm(32, data[0]);
	pro->hazard = 0;
	return this;
}

statement* beq::EX() { cache = data[0] == data[1]; return bbase::EX(); }
statement* bne::EX() { cache = data[0] != data[1]; return bbase::EX(); }
statement* bge::EX() { cache = data[0] >= data[1]; return bbase::EX(); }
statement* ble::EX() { cache = data[0] <= data[1]; return bbase::EX(); }
statement* bgt::EX() { cache = data[0] > data[1];  return bbase::EX(); }
statement* blt::EX() { cache = data[0] < data[1];  return bbase::EX(); }

statement* beqz::EX() { cache = data[0] == data[1]; return bbasez::EX(); }
statement* bnez::EX() { cache = data[0] != data[1]; return bbasez::EX(); }
statement* bgez::EX() { cache = data[0] >= data[1]; return bbasez::EX(); }
statement* blez::EX() { cache = data[0] <= data[1]; return bbasez::EX(); }
statement* bgtz::EX() { cache = data[0] > data[1];  return bbasez::EX(); }
statement* bltz::EX() { cache = data[0] < data[1];  return bbasez::EX(); }

statement* la::EX() { cache = data[1] + data[2], cpu.writetmp(0, data[0], cache); return this; }
statement* load::MA() { cache = mem.load(cache, num);  return lbase::MA(); }

statement* store::ID()
{
	if (state[2] == 0) data[2] = 0;
	else if (!loadcache(2)) return NULL;
	if (!loadcache(0)) return NULL;
	return this;
}
statement* store::EX() { cache = data[1] + data[2]; return this; }
statement* store::MA() { mem.store(cache, data[0] , num); return this; }
statement* store::WB() { return this; }

statement* Move::ID()
{
	if (!loadcache(1)) return NULL;
	lockcache(0);
	return this;
}
statement* Move::EX() { cache = data[1], cpu.writetmp(0, data[0], cache); return this; }
statement* Move::MA() { cpu.writetmp(1, data[0], cache); return this; }
statement* Move::WB() { writecache(0, cache); return this; }

statement* mfhi::ID() { data[1] = 33; return Move::ID(); }
statement* mflo::ID() { data[1] = 34; return Move::ID(); }

statement* nop::ID() { return this; }
statement* nop::EX() { return this; }
statement* nop::MA() { return this; }
statement* nop::WB() { return this; }

statement* syscall::ID()
{
	data[0] = 2;
	data[1] = 4;
	data[2] = 5;
	state[0] = state[1] = state[2] = 1;
	if (!loadcache(0) || !loadcache(1) || !loadcache(2)) return NULL;
	lockcache("$v0");
	pro->hazard = 1;
	return this;
}
statement* syscall::EX()
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
