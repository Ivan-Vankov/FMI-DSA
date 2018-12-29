/**
*
* Solution to homework task
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2017/2018
*
* @author Ivan Vankov
* @idnumber 45255
* @task 1
* @compiler VC
*
*/
#include "Parser.h"
#include <exception>

int main() {
	MyString userInput;
	Tabs tabModule;
	int exitCode = 0;
	while (true) {
		try {
			userInput.readLine(std::cin);
			userInput.removeUnnecessarySymbols(" \t");
			Parser::parseAndExecute(userInput, tabModule, exitCode);
			if (exitCode) { break; }
			userInput.free();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

	return 0;
}