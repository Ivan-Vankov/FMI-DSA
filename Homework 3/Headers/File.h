#ifndef _FILE_HEADER_
#define _FILE_HEADER_
#include "MyString.h"
#include <iostream>

class File {
public:
	File();
	File(const MyString& file, int size);

	const MyString& getName() const;
	int getSize() const;

	bool operator<(const File& rhs) const;
	bool operator>(const File& rhs) const;
	bool operator<=(const File& rhs) const;
	bool operator>=(const File& rhs) const;
	bool operator==(const File& rhs) const;

	bool hasExtention(const MyString& filter) const;
private:
	MyString name;
	int sizeInBytes;
};

static std::ostream& operator<<(std::ostream& os, const File& dir) {
	int size = dir.getSize();
	os << dir.getName() << "   " << size;
	if (size == 1) {
		os << "Byte";
	}
	else {
		os << "Bytes";
	}
	return os;
}

#endif