#include "File.h"

File::File() :name("Invalid file"), sizeInBytes(0) {
}

File::File(const MyString& file, int size) : name(file), sizeInBytes(size) {
}

const MyString& File::getName() const {
	return name;
}

int File::getSize() const {
	return sizeInBytes;
}

bool File::operator<(const File& rhs) const {
	return strcmp(name, rhs.name) < 0;
}

bool File::operator>(const File& rhs) const {
	return strcmp(name, rhs.name) > 0;
}

bool File::operator<=(const File& rhs) const {
	return !(*this > rhs);
}

bool File::operator>=(const File& rhs) const {
	return !(*this < rhs);
}

bool File::operator==(const File& rhs) const {
	return name == rhs.name;
}

bool File::hasExtention(const MyString& filter) const {
	int nameSize = name.getLength();
	int filterSize = filter.getLength();
	if (!filterSize) {
		return true;
	}
	if (filterSize >= nameSize) {
		return false;
	}
	else {
		if (name[nameSize - filterSize - 1] != '.') {
			return false;
		}
		for (int i = nameSize - filterSize, j = 0; i < nameSize; ++i, ++j) {
			if (name[i] != filter[j]) {
				return false;
			}
		}
		return true;
	}
}