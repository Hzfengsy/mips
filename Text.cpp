//
//  Text.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "Text.hpp"
#include <iostream>
using std::endl;
using std::cerr;

Decoder::Decoder(istream &_is, Memory &_memory, Program &_pro) : is(_is), memory(_memory), pro(_pro) {}

void Decoder::removeComment(string &st)
{
	int len = int(st.length());
	for (int i = 0; i < len; i++)
	{
		if (st[i] == '#') {st = st.substr(0, i); return;}
		if (st[i] == '\"') getString(st, i);
	}
}

void Decoder::getLabel(string &st)
{
	int len = int(st.length()), i;
	for (i = 0; i < len; i++)
	{
		if (st[i] == ':') break;
		if (st[i] == '\"') getString(st, i);
	}
	if (i >= len) return;
	scanner scan(st.substr(0, i));
	st = st.substr(i + 1, st.length() - i - 1);
	string Label = scan.getNext();
	if (scan.hasNext()) throw invalid_program();
	if (flag) pro.addLabel(Label);
	else memory.setLabel(Label, memory.staticpos);
}

void Decoder::align(scanner &scan)
{
	if (flag) throw invalid_program();
	int n = stringToInt(scan.getNext());
	if (scan.hasNext()) throw invalid_program();
	n = 1<<n;
	memory.staticpos = (memory.staticpos / n + (memory.staticpos % n != 0)) * n;
}

void Decoder::ascii(scanner &scan)
{
	if (flag) throw invalid_program();
	string str = scan.getNextString();
	if (scan.hasNext()) throw invalid_program();
	for (int i = 0; i < str.length(); i++, memory.staticpos++)
		memory.store(memory.staticpos, str[i], 1);
}

void Decoder::asciiz(scanner &scan)
{
	ascii(scan);
	memory.store(memory.staticpos++, 0, 1);
}

void Decoder::number(scanner &scan, int n)
{
	if (flag) throw invalid_program();
	while (scan.hasNext())
	{
		string t = scan.getNextArg();
		memory.store(memory.staticpos, stringToInt(t), n);
		memory.staticpos += n;
	}
}

void Decoder::space(scanner &scan)
{
	string t = scan.getNext();
	int k = stringToInt(t);
	for (int i = 0; i < k; i++)
		memory.store(memory.staticpos++, 0, 1);
}

bool Decoder::processLine()
{
	string t;
	if (!getline(is, t)) return 0;
	//cerr << t << endl;
	removeComment(t);
	getLabel(t);
	scanner Line(t);
	if (!Line.hasNext()) return 1;
	string op = Line.getNext();
	if (op == ".align") align(Line);
	else if (op == ".ascii") ascii(Line);
	else if (op == ".asciiz") asciiz(Line);
	else if (op == ".byte") number(Line, 1);
	else if (op == ".half") number(Line, 2);
	else if (op == ".word") number(Line, 4);
	else if (op == ".data")
	{
		if (Line.hasNext()) throw invalid_program();
		flag = 0;
	}
	else if (op == ".text")
	{
		if (Line.hasNext()) throw invalid_program();
		flag = 1;
	}
	else
	{
		command com(op, pro, memory);
		Line >> com;
		pro.push_back(com);
	}
	return 1;
}

void Decoder::process()
{
	command::inti();
	while (processLine());
	pro.exchengLabel();
}


