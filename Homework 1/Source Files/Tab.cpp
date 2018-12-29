#include "Tab.h"

Tab::Tab() :url("about:blank"), timestamp(time(NULL)) {
}

Tab::Tab(const Tab& rhs) {
	copyFrom(rhs);
}

Tab::Tab(const MyString& newUrl) :url(newUrl), timestamp(time(NULL)) {
}

Tab& Tab::operator=(const Tab& rhs) {
	if (this != &rhs) {
		copyFrom(rhs);
	}
	return *this;
}

Tab& Tab::operator=(const MyString& newUrl) {
	url = newUrl;
	timestamp = time(NULL);
	return *this;
}

Tab::~Tab() {}

void Tab::copyFrom(const Tab& rhs) {
	url = rhs.url;
	timestamp = time(NULL);
}

MyString Tab::getUrl()const {
	return url;
}

time_t Tab::getTimestamp()const {
	return timestamp;
}

void Tab::setUrl(const MyString& newUrl) {
	url = newUrl;
}

void Tab::setTimestamp(time_t newTimestamp) {
	timestamp = newTimestamp;
}

bool Tab::operator==(const Tab& rhs)const {
	return url == rhs.url;
}

bool Tab::operator!=(const Tab& rhs)const {
	return !(*this == rhs);
}