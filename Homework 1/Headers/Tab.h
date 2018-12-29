#ifndef _TAB_HEADER_
#define _TAB_HEADER_
#include <ctime>
#include "MyString.h"

class Tab {
public:
	Tab();
	Tab(const Tab&);
	Tab(const MyString&);
	Tab& operator=(const Tab&);
	Tab& operator=(const MyString&);
	~Tab();
public:
	MyString getUrl()const;
	time_t getTimestamp()const;
	void setUrl(const MyString&);
	void setTimestamp(time_t);
public:
	bool operator==(const Tab&)const;
	bool operator!=(const Tab&)const;
private:
	void copyFrom(const Tab&);
private:
	MyString url;
	time_t timestamp;
};

#endif