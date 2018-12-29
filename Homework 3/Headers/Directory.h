#ifndef _DIRECTORY_HEADER_
#define _DIRECTORY_HEADER_
#include "MyString.h"
#include <iostream>
#include <windows.h>

class Directory {
public:
	Directory(const MyString&, bool isValid = true);
	Directory(const Directory&);
	Directory& operator=(const Directory&);
	~Directory();

	BOOL findNextFile(LPWIN32_FIND_DATA);

	const MyString& getName() const;
	HANDLE getHandle() const;
	void updateHandle();
	bool isValid() const;

	bool operator<(const Directory& rhs) const;
	bool operator>(const Directory& rhs) const;
	bool operator<=(const Directory& rhs) const;
	bool operator>=(const Directory& rhs) const;
	bool operator==(const Directory& rhs) const;

	operator const char*() const;
private:
	void copyFrom(const Directory&);
	void free();

	bool valid;
	HANDLE handle;
	MyString name;
};

static std::ostream& operator<<(std::ostream& os, const Directory& dir) {
	os << dir.getName();
	return os;
}

#endif