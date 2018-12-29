#include <iostream>
#include <exception>
#include "MyString.h"

MyString::MyString() {
	arr = new char;
	arr[0] = '\0';
	allocSize = 1;
	length = 0;
}

MyString::MyString(const char* rhs) {
	int rhsSize = strlen(rhs);
	int newAllocSize = calculateClosestPowerOfTwo(rhsSize);
	arr = new char[newAllocSize];
	checkLessStrcat(arr, rhs);
	length = rhsSize;
	allocSize = newAllocSize;//trqbva check za zadelena pamet
}

MyString::MyString(MyString const & other) {
	copyFrom(other);
}

MyString& MyString::operator=(MyString const & other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}


MyString::~MyString() {
	delete[] arr;//ne e free()
	arr = NULL;
	allocSize = 0;
	length = 0;
}


void MyString::checkLessStrcat(char* str1, const char*str2) {
	int i = 0;
	do {
		str1[i] = str2[i];
	} while (str2[i++]);
}


void MyString::removeUnnecessarySymbols(const MyString& delims) {
	MyString buffer;
	int i = 0;
	int wordCount = countWords();
	int wordCounter = 0;
	do {
		while (arr[i] && delims.isIn(arr[i])) {
			++i;
		}
		while (arr[i] && !delims.isIn(arr[i])) {
			buffer.add(arr[i++]);
		}
		if (++wordCounter < wordCount) {
			buffer.add(' ');
		}
	} while (arr[i++]);
	delete[] arr;
	arr = NULL;
	*this = buffer;
}

int MyString::countWords()const {
	int cnt = 0;
	MyString delims("\t ");
	int i = 0;
	while (arr[i]) {
		while (arr[i] && delims.isIn(arr[i])) {
			++i;
		}
		if (arr[i] && !delims.isIn(arr[i])) {
			++cnt;
			++i;
		}
		while (arr[i] && !delims.isIn(arr[i])) {
			++i;
		}
	}
	return cnt;
}

bool MyString::isIn(char elem)const {
	int i = 0;
	while (arr[i]) {
		if (arr[i++] == elem) {
			return true;
		}
	}
	return false;
}

void MyString::copyFrom(MyString const & other) {
	arr = new char[other.allocSize];
	checkLessStrcat(arr, other.getArray());
	allocSize = other.allocSize;
	length = other.length;
}

void MyString::readLine(std::istream& is) {
	char c;
	while (is.get(c) && c != '\n') {
		if ((length + 1) >= allocSize) {
			reallocate(allocSize ? allocSize * 2 : 2);
		}
		arr[length++] = c;
	}
	if (length) {
		arr[length] = '\0';
	}
}

void MyString::free() {
	delete[] arr;
	arr = new char;
	arr[0] = '\0';
	allocSize = 1;
	length = 0;
}

MyString& MyString::concatenate(const char* rhs) {
	int rhsLength = strlen(rhs);
	for (int i = 0; i < rhsLength; ++i) {
		add(rhs[i]);
	}
	return *this;
}


void MyString::reallocate(int newAllocSize) {
	char* temp = new char[newAllocSize];
	for (int i = 0; i <= length; i++) {
		temp[i] = arr[i];
	}
	delete[] arr;
	arr = temp;
	allocSize = newAllocSize;
}


void MyString::add(char element) {
	if ((length + 1) >= allocSize) {
		reallocate(allocSize ? allocSize * 2 : 2);
	}
	arr[length++] = element;
	if (element) {
		arr[length] = '\0';
	}
}

void MyString::remove(int index) {
	if (index >= length || index < 0) {
		std::cerr << "Invalid index at MyString::remove.";;
		return;
	}
	if (length--) {
		for (int i = index; i <= length; ++i) {
			arr[i] = arr[i + 1];
		}
	}
	int halfAllocSize = allocSize / 2;
	if ((length + 1) == halfAllocSize) {
		reallocate(halfAllocSize);
	}
}

int MyString::getAllocatedSize()const {
	return allocSize;
}


int MyString::getLength()const {
	return length;
}


const char* MyString::getArray()const {
	return arr;
}


void MyString::setArray(const char* rhs) {
	if (arr != rhs) {
		free();
		concatenate(rhs);
	}
}


MyString::operator const char*() {
	return arr;
}


void MyString::print() const {
	if (length > 0) {
		for (int i = 0; i < length; i++) {
			std::cout << arr[i];
		}
		std::cout << "\n";
	}
}


int MyString::calculateClosestPowerOfTwo(int other)const {
	int closestPowerOfTwo = 1;
	for (int i = 0; i < 32; ++i) {
		if (other&(1 << i)) {
			closestPowerOfTwo = (1 << (i + 1));
		}
	}
	return closestPowerOfTwo;
}


MyString MyString::wordNumber(int index)const {
	if (index<1 || index>countWords()) {
		throw std::exception("Improper index in MyString::wordNumber.\n");
	}

	int cnt = 0;
	MyString delims("\t \n");
	int i = 0;
	while (cnt != index) {
		while (arr[i] && delims.isIn(arr[i])) {
			++i;
		}
		if (arr[i] && !delims.isIn(arr[i])) {
			++cnt;
		}
		if (cnt != index) {
			while (arr[i] && !delims.isIn(arr[i])) {
				++i;
			}
		}
	}

	int j = i;
	while (!delims.isIn(arr[i]) && arr[i]) {
		++i;
	}
	MyString word;
	while (j != i) {
		word.add(arr[j++]);
	}
	return word;
}

char& MyString::operator[](int index) {
	if (index > length || index < 0) {
		throw std::out_of_range("Invalid index at MyString::operator[].");
	}
	return arr[index];
}

char MyString::operator[](int index) const {
	if (index > length || index < 0) {
		throw std::out_of_range("Invalid index at MyString::operator[] const.");
	}
	return arr[index];
}

bool MyString::operator==(const MyString & rhs) const {
	if (!strcmp(arr, rhs.arr)) {
		return true;
	}
	return false;
}

bool MyString::operator!=(const MyString & rhs) const {
	return !(*this == rhs);
}

MyString& MyString::operator+(const char* rhs) {
	concatenate(rhs);
	return *this;
}