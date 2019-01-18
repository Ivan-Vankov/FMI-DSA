#ifndef _MYSTRING_HEADER_
#define _MYSTRING_HEADER_

#include <iostream>

class MyString {
public:
	MyString();
	MyString(const char*);
	MyString(MyString const &);
	MyString& operator=(MyString const &);
	~MyString();
public:
	void free();
	void add(char);
	void remove(int);
	MyString& concatenate(const char*);
public:
	int getAllocatedSize() const;
	int getLength() const;
	void print() const;
public:
	const char* getArray() const;
	void setArray(const char*);
public:
	bool isIn(char)const;
public:
	int countWords()const;
private:
	void checkLessStrcat(char* str1, const char*str2);
public:
	void removeUnnecessarySymbols(const MyString&);
public:
	void readLine(std::istream&);
public:
	operator const char*();
	char operator[](int) const;
	char& operator[](int);
	bool operator==(const MyString &) const;
	bool operator!=(const MyString &) const;
	MyString& operator+(const char*);
private:
	void reallocate(int NewSize);
	void copyFrom(const MyString &);
private:
	int calculateClosestPowerOfTwo(int)const;
public:
	MyString wordNumber(int)const;
private:
	char* arr;
	int allocSize;
	int length;
};

static std::ostream& operator<<(std::ostream& os, const MyString& stuff) {
	os << stuff.getArray();
	return os;
}

#endif