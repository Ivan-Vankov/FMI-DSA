#include "Parser.h"

int main() {
	MyString userInput;
	while (true) {
		try {
			userInput.readLine(std::cin);
			userInput.removeUnnecessarySymbols(" \t");
			Parser::parseAndExecute(userInput);
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		userInput.free();
	}
	return 0;
}