#include "Parser.h"

void Parser::parseAndExecute(const MyString& input, Tabs& module, int& exitCode) {
	int wordCount = input.countWords();
	if (!wordCount) {
		return;
	}
	MyString firstWord = input.wordNumber(1);
	if (!strcmp(firstWord, "go")) {
		if (wordCount > 2) {
			std::cout << "Incorrect URL." << std::endl;
			return;
		}
		module.go(input.wordNumber(2));
		return;
	}
	if (!strcmp(firstWord, "insert")) {
		if (wordCount > 2) {
			std::cout << "Incorrect URL." << std::endl;
			return;
		}
		module.insert(input.wordNumber(2));
		return;
	}
	if (!strcmp(firstWord, "sort")) {
		if (wordCount > 2 || wordCount == 1) {
			std::cout << "Unclear sorting method." << std::endl;
			return;
		}
		MyString secondWord = input.wordNumber(2);
		if (!strcmp(secondWord, "url")) {
			module.sortByUrl();
			return;
		}
		if (!strcmp(secondWord, "time")) {
			module.sortByTimestamp();
			return;
		}
		std::cout << "Unclear sorting method." << std::endl;
		return;
	}
	if (wordCount == 1) {
		if (!strcmp(firstWord, "back")) {
			module.back();
			return;
		}
		if (!strcmp(firstWord, "forward")) {
			module.forward();
			return;
		}
		if (!strcmp(firstWord, "remove")) {
			module.remove();
			return;
		}
		if (!strcmp(firstWord, "print")) {
			module.print();
			return;
		}
		if (!strcmp(firstWord, "exit")) {
			exitCode = 1;
			return;
		}
	}
	std::cout << "Unknown command." << std::endl;
}