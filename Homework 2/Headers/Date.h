#ifndef _DATE_HEADER_
#define _DATE_HEADER_
#include "MyString.h"

class Date {
public:
	Date();
	Date(const Date&);
	Date& operator=(const MyString&);
	~Date();
public:
	int getYear() const { return YMD / 10000; }
	int getMonth() const { return (YMD / 100) % 100; }
	int getDay() const { return YMD % 100; }
public:
	operator int() const { return YMD; }
private:
	void fillDateBuffer(int& idx, MyString& dateBuffer, const MyString& source) const;
	void getYMDFromString(const MyString& buffer);
private:
	int YMD;
};

static std::ostream& operator<<(std::ostream& os, const Date& date) {
	os << date.getYear() << '-';
	if (!(date.getMonth() / 10)) {
		os << '0';
	}
	os << date.getMonth() << '-';
	if (!(date.getDay() / 10)) {
		os << '0';
	}
	os << date.getDay();
	return os;
}

#endif