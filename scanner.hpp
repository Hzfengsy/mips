//
//  scanner.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef scanner_hpp
#define scanner_hpp

#include <string>
#include "exception.hpp"
using std::string;

class scanner
{
	string st;
	int index, len;
public:
	scanner(const string &_st);
	
	string getNext(char c = ' ');
	
	bool hasNext(char c = ' ');
	
	string getNextArg();
	
	bool hasNextArg();
	
	string getNextString();
};

int stringToInt(const string &token);

string getString(const string &st, int &index);

#endif /* scanner_hpp */
