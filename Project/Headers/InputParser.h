#ifndef _INPUTPARSER_HEADER_
#define _INPUTPARSER_HEADER_
#include "Graphs.h"

class InputParser {
public:
	// Returns false if the input is "Quit"
	// and true in all other cases
	static bool parseAndExecute(const String&, Graphs&);
};

#endif