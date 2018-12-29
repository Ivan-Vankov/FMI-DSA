#include "InputParser.h"

int main() {
	String input;
	Graphs graphs;
	while (true) {
		try {
			input.readLine(std::cin);
			input.removeUnnecessarySymbols(" \t");
			if (!InputParser::parseAndExecute(input, graphs)) { break; }
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		input.free();
	}

	return 0;
}