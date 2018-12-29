#ifndef _TABLE_HEADER_
#define _TABLE_HEADER_
#include "Vector.h"
#include "MyString.h"
#include "Student.h"

typedef bool(*ComparerInt)(int, int);
typedef bool(*Comparer)(const MyString&, const MyString&);

static bool isLessThanInt(int a, int b) {
	return a < b;
}

static bool isGreaterThanInt(int a, int b) {
	return a > b;
}

static bool isLessThan(const MyString& a, const MyString& b) {
	return strcmp(a, b) < 0;
}

static bool isGreaterThan(const MyString& a, const MyString& b) {
	return strcmp(a, b) > 0;
}

class Table {
private:
	void siftUp(int index);
	void siftDown(int n, int index);
	void heapify();
	void reverseSiftUp(int index);
	void reverseSiftDown(int n, int index);
	void reverseHeapify();
public:
	void heapSort();
	void reverseHeapSort();
private:
	void merge(int padding, int firstSize, int secondSize, Comparer comp);
public:
	void mergeSort(int padding, int size, Comparer comp);
private:
	void countingSortAge(int exp, int ascOrDesc);
	void countingSortDate(int exp, int ascOrDesc);
	void radixSortAge(int ascOrDesc);
	void radixSortDate(int ascOrDesc);
public:
	Table();
	Table(const MyString&);
	~Table();
public:
	void sortByName(bool ascOrDesc);
	void sortByAge(bool ascOrDesc);
	void sortBySubject(bool ascOrDesc);
	void sortByGrade(bool ascOrDesc);
	void sortByDate(bool ascOrDesc);
public:
	void print() const;
	void readFile(const MyString&);
private:
	Vector<Student> table;
};

#endif