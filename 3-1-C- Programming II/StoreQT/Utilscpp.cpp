/*************************************************************************
* Copyright (c)2017, by Beijing University of Posts and Telecommunications
* All rights reserved.
* FileName:		Utilscpp.cpp
* System:		Windows 10
* Author:		Xiao Yunming
* Date:			2017.12.15
* Version:		1.0
* Description:
Some useful functions.
*

* Last Modified:

2017.12.22, 	By Xiao Yunming
*************************************************************************/

#include <Store.h>

vector<string> StringParse(string s, string delim)
{
	// split the string s by delim to this->str
	vector<string> result;

	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0)
		return result;

	// parse by delim
	while (pos < len) {
		int find_pos = s.find(delim, pos);
		if (find_pos < 0) {
			result.push_back(s.substr(pos, len - pos));
			break;
		}
		result.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	
	// erase the blanks
	vector<string>::iterator it = result.begin();
	while (it != result.end()) {
		it->erase(0, it->find_first_not_of(' '));
		it++;
	}

	return result;
}