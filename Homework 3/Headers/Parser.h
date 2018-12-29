#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_
#include "Files.h"

class Parser {
public:
	//returns false if the command was quit and true in all other cases
	static bool parseAndExecute(const MyString& input, Files& files);
};

#endif