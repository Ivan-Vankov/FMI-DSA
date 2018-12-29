#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_
#include "Tabs.h"

class Parser {
public:
	static void parseAndExecute(const MyString& input, Tabs& module, int& exitCode);
};

#endif