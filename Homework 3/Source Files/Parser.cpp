#include "Parser.h"

bool Parser::parseAndExecute(const MyString& input, Files& fTree) {
	int wordCount = input.countWords();
	if (!wordCount) {
		return true;
	}
	MyString command = input.wordNumber(1);
	if (!strcmp(command, "print")) {
		if (wordCount > 2) {
			std::cerr << "Too many words in filter." << std::endl;
			return true;
		}
		MyString filter = input.wordNumber(2);
		fTree.print(filter);
		return true;
	}
	if (wordCount > 2) {
		std::cerr << "Unknown command." << std::endl;
		return true;
	}
	if (!strcmp(command, "refresh")) {
		fTree.refresh();
		return true;
	}
	if (!strcmp(command, "quit")) {
		return false;
	}
	std::cerr << "Unknown command." << std::endl;
	return true;
}