#include "Files.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Please add a directory to explore in the command line.\n";
		return 1;
	}
	try {
		Files files(argv[1]);
		MyString userInput;
		do {
			userInput.free();
			userInput.readLine(std::cin);
			userInput.removeUnnecessarySymbols(" \t");
		} while (Parser::parseAndExecute(userInput, files));
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}