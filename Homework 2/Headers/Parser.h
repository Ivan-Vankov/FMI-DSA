#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_
#include "Table.h"

class Parser {
public:
	static void parseAndExecute(const MyString& input);
private:
	static bool toBool(const MyString& ascOrDesc);
};

#endif