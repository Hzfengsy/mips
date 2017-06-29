//
//  scanner.cpp
//  mips
//
//  Created by 冯思远 on 2017/6/26.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#include "scanner.hpp"

scanner::scanner(const string &_st) : st(_st), index(0) { len = int(st.length()); }

string scanner::getNext(char c)
{
	string ans = "";
	for (int &i = index; i < len; i++)
		if (st[i] != c && st[i] != '\t') ans += st[i];
		else if (ans != "") break;
	index++;
	return ans;
}

string scanner::getNextArg()
{
	string ans = "";
	for (int &i = index; i < len; i++)
		if (st[i] != ',' && st[i] != ' ' && st[i] != '\t') ans += st[i];
		else if (ans != "") break;
	index++;
	return ans;
}

string scanner::getNextString()
{
	return getString(st, index);
}

string getString(const string &st, int &index)
{
	bool flag = 0; string ans = "";
	int len = int(st.length());
	for (int &i = index; i < len; i++)
	{
		if (flag)
		{
			if (st[i] == '\\')
			{
				i++;
				if (i >= len) throw invalid_program();
				if (st[i] == '\"' || st[i] == '\'' || st[i] == '\\' || st[i] == '\?' ) ans += st[i];
				if (st[i] == 'n') ans += '\n';
				if (st[i] == 't') ans += '\t';
				if (st[i] == 'r') ans += '\r';
				if (st[i] == '0') ans += '\0';
				continue;
			}
			else if (st[i] == '"') {i++; return ans;}
			else ans += st[i];
		}
		if (st[i] == '"')
		{
			flag ^= 1;
			if (flag == 0) { i++; return ans;}
		}
	}
	throw invalid_program();
	return ans;
}

bool scanner::hasNext(char c)
{
	for (int &i = index; i < len; i++)
		if (st[i] != c && st[i] != '\t') return 1;
	return 0;
}

bool scanner::hasNextArg()
{
	for (int &i = index; i < len; i++)
		if (st[i] != ' ' && st[i] != ',' && st[i] != '\t') return 1;
	return 0;
}

int stringToInt(const string &token)
{
	int ans = 0;
	bool flag = token[0] == '-';
	for (int i = token[0] == '-' ? 1 : 0; i < token.length(); i++)
		if (token[i] <'0' || token[i] > '9') throw invalid_program();
		else ans = ans * 10 + token[i] - '0';
	return flag ? -ans : ans;
}

