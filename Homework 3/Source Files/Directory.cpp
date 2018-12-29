#include "Directory.h"

Directory::Directory(const MyString& fileName, bool isValid) :
	name(fileName), valid(isValid) {
	updateHandle();
}

Directory::Directory(const Directory& rhs) {
	copyFrom(rhs);
}

Directory& Directory::operator=(const Directory& rhs) {
	if (this != &rhs) {
		FindClose(handle);
		copyFrom(rhs);
	}
	return *this;
}

Directory::~Directory() {
	free();
}

const MyString& Directory::getName() const {
	return name;
}

HANDLE Directory::getHandle() const {
	return handle;
}

void Directory::updateHandle() {
	try {
		MyString path = name;
		path + "\\*";
		WIN32_FIND_DATA findFileData;
		handle = FindFirstFile(path, &findFileData);
		if (handle == INVALID_HANDLE_VALUE) {
			throw std::exception("Couldn't find the first file.");
		}
		FindNextFile(handle, &findFileData);
	}
	catch (std::exception&) {
		throw;
	}
}

bool Directory::isValid() const {
	return valid;
}

bool Directory::operator<(const Directory& rhs) const {
	return strcmp(name, rhs.name) < 0;
}

bool Directory::operator>(const Directory& rhs) const {
	return strcmp(name, rhs.name) > 0;
}

bool Directory::operator<=(const Directory& rhs) const {
	return !(*this > rhs);
}

bool Directory::operator>=(const Directory& rhs) const {
	return !(*this < rhs);
}

bool Directory::operator==(const Directory& rhs) const {
	return name == rhs.name;
}

Directory::operator const char*() const {
	return name;
}

BOOL Directory::findNextFile(LPWIN32_FIND_DATA findFileData) {
	return FindNextFile(handle, findFileData);
}

void Directory::copyFrom(const Directory& rhs) {
	name = rhs.name;
	valid = rhs.valid;
}

void Directory::free() {
	name.free();
	FindClose(handle);
	valid = false;
}