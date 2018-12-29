#include "Tabs.h"

Tabs::Tabs() : tabs((MyString)"about:blank"),
currTab(tabs.begin()) {
}

Tabs::Tabs(const Tabs& rhs) : tabs(rhs.tabs),
currTab(rhs.currTab) {
}

Tabs& Tabs::operator=(const Tabs& rhs) {
	if (this != &rhs) {
		tabs.free();
		tabs = rhs.tabs;
		currTab = rhs.currTab;
	}
	return *this;
}

Tabs::~Tabs() {
	tabs.free();
}

void Tabs::go(const MyString& newUrl) {
	(*currTab) = newUrl;
}

void Tabs::insert(const MyString& newUrl) {
	tabs.insertAfter(currTab, newUrl);
	++currTab;
}

void Tabs::back() {
	if (!isFirstTab(currTab)) {
		--currTab;
	}
}

void Tabs::forward() {
	if (!isLastTab(currTab)) {
		++currTab;
	}
}

void Tabs::remove() {
	if (tabs.getSize() > 1) {
		if (!isLastTab(currTab)) {
			tabs.pop(currTab++);
		}
		else {
			tabs.pop(currTab--);
		}
	}
	else {
		tabs.popFront();
		tabs.pushBack((MyString)"about:blank");
		currTab = tabs.begin();
	}
}

void Tabs::print() {
	int tabsSize = tabs.getSize();
	iterator<Tab> iter = tabs.begin();
	for (; iter != currTab; ++iter) {
		std::cout << (*iter).getUrl() << ' ' << (*iter).getTimestamp() << std::endl;
	}
	++iter;
	std::cout << "> " << (*currTab).getUrl() << ' ' << (*currTab).getTimestamp() << std::endl;
	for (; iter != tabs.end(); ++iter) {
		std::cout << (*iter).getUrl() << ' ' << (*iter).getTimestamp() << std::endl;
	}
}

bool Tabs::isFirstTab(const iterator<Tab>& iter) {
	return iter == tabs.begin();
}

bool Tabs::isLastTab(const iterator<Tab>& iter) {
	return iter == --(tabs.end());
}

void Tabs::sortByUrl() {
	iterator<Tab> firstEnd = --(tabs.end());
	iterator<Tab> secondEnd = firstEnd;
	for (iterator<Tab> i = tabs.begin(); i != firstEnd; ++i) {
		for (iterator<Tab> j = tabs.begin(); j != secondEnd; ++j) {
			iterator<Tab> afterJ = j;
			if (strcmp((*j).getUrl(), (*(++afterJ)).getUrl()) < 0) {
				MyString tempUrl = (*j).getUrl();
				time_t tempStamp = (*j).getTimestamp();
				(*j).setUrl((*afterJ).getUrl());
				(*j).setTimestamp((*afterJ).getTimestamp());
				(*afterJ).setUrl(tempUrl);
				(*afterJ).setTimestamp(tempStamp);
			}
		}
		--secondEnd;
	}
}

void Tabs::sortByTimestamp() {
	iterator<Tab> firstEnd = --(tabs.end());
	iterator<Tab> secondEnd = firstEnd;
	for (iterator<Tab> i = tabs.begin(); i != firstEnd; ++i) {
		for (iterator<Tab> j = tabs.begin(); j != secondEnd; ++j) {
			iterator<Tab> afterJ = j;
			if ((*j).getTimestamp() < (*(++afterJ)).getTimestamp()) {
				MyString tempUrl = (*j).getUrl();
				time_t tempStamp = (*j).getTimestamp();
				(*j).setUrl((*afterJ).getUrl());
				(*j).setTimestamp((*afterJ).getTimestamp());
				(*afterJ).setUrl(tempUrl);
				(*afterJ).setTimestamp(tempStamp);
			}
		}
		--secondEnd;
	}
}