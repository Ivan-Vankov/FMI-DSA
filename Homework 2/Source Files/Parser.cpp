#include "Parser.h"

void Parser::parseAndExecute(const MyString& input) {
	int wordCount = input.countWords();
	if (!wordCount) {
		return;
	}
	if (wordCount > 3) {
		throw std::exception("Unknown command.");
	}
	MyString path = input.wordNumber(1);
	MyString orderBy;
	bool ascOrDesc;
	if (wordCount == 1) {
		orderBy = "grade";
		ascOrDesc = 1;
	}
	if (wordCount == 2) {
		orderBy = input.wordNumber(2);
		ascOrDesc = 1;
	}
	if (wordCount == 3) {
		orderBy = input.wordNumber(2);
		ascOrDesc = toBool(input.wordNumber(3));
	}
	Table currTable(path);
	if (!strcmp(orderBy, "name")) {
		currTable.sortByName(ascOrDesc);
		currTable.print();
		return;
	}
	if (!strcmp(orderBy, "age")) {
		currTable.sortByAge(ascOrDesc);
		currTable.print();
		return;
	}
	if (!strcmp(orderBy, "subject")) {
		currTable.sortBySubject(ascOrDesc);
		currTable.print();
		return;
	}
	if (!strcmp(orderBy, "grade")) {
		currTable.sortByGrade(ascOrDesc);
		currTable.print();
		return;
	}
	if (!strcmp(orderBy, "date")) {
		currTable.sortByDate(ascOrDesc);
		currTable.print();
		return;
	}
	throw std::exception("Unknown way to sort by.");
}

bool Parser::toBool(const MyString& ascOrDesc) {
	if (!strcmp("asc", ascOrDesc.getArray())) {
		return 0;
	}
	else if (!strcmp("desc", ascOrDesc.getArray())) {
		return 1;
	}
	else {
		throw std::exception("Unknown order for sorting.");
	}
}