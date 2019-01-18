#ifndef _TABS_HEADER_
#define _TABS_HEADER_
#include "List.h"
#include "Tab.h"

class Tabs {
public:
	Tabs();
	Tabs(const Tabs&);
	Tabs& operator=(const Tabs&);
	~Tabs();
public:
	void go(const MyString& newUrl);
	void insert(const MyString& newUrl);
	void back();
	void forward();
	void remove();
	void print();
	void sortByUrl();
	void sortByTimestamp();
private:
	bool isFirstTab(const iterator<Tab>&);
	bool isLastTab(const iterator<Tab>&);
private:
	List<Tab> tabs;
	iterator<Tab> currTab;
};

#endif