#include "Date.h"

Date::Date() :YMD(6660101) {
}

Date::Date(const Date& rhs) : YMD(rhs.YMD) {
}

Date& Date::operator=(const MyString& rhs) {
	getYMDFromString(rhs);
	return *this;
}

Date::~Date() {
}

void Date::getYMDFromString(const MyString& source) {
	int index = 0;
	MyString dateBuffer;
	fillDateBuffer(index, dateBuffer, source);
	YMD = atoi(dateBuffer) * 10000;
	dateBuffer.free();
	fillDateBuffer(++index, dateBuffer, source);
	YMD += atoi(dateBuffer) * 100;
	dateBuffer.free();
	fillDateBuffer(++index, dateBuffer, source);
	YMD += atoi(dateBuffer);
}

void Date::fillDateBuffer(int& index, MyString& dateBuffer, const MyString& source) const {
	while (source[index] != '-' && source[index] != '\0') {
		dateBuffer.add(source[index]);
		++index;
	}
}