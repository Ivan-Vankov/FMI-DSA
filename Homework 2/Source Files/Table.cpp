#include "Table.h"
#include <fstream>

Table::Table() {}

Table::Table(const MyString& path) {
	readFile(path);
}

Table::~Table() {}

void Table::merge(int padding, int firstSize, int secondSize, Comparer comp) {
	int totalSize = firstSize + secondSize;
	Vector<Student> temp;
	Vector<Student> second;
	for (int m = padding + firstSize; m < padding + totalSize; ++m) {
		second.add(table[m]);
	}
	int j = padding, k = 0;
	while (j < firstSize + padding && k < secondSize) {
		if (comp(table[j].getName(), second[k].getName())) {
			temp.add(table[j++]);
		}
		else {
			temp.add(second[k++]);
		}
	}
	while (j < firstSize + padding) {
		temp.add(table[j++]);
	}
	while (k < secondSize) {
		temp.add(second[k++]);
	}
	for (int f = padding, l = 0; f < padding + totalSize; ++f, ++l) {
		table[f] = temp[l];
	}
}

void Table::mergeSort(int padding, int size, Comparer comp) {
	if (size <= 1) {
		return;
	}
	int firstSize = size / 2;
	int secondSize = size - size / 2;
	mergeSort(0, firstSize, comp);
	mergeSort(firstSize, secondSize, comp);
	merge(padding, firstSize, secondSize, comp);
}

void Table::sortByName(bool ascOrDesc) {
	if (ascOrDesc) {
		mergeSort(0, table.getSize(), isGreaterThan);
	}
	else {
		mergeSort(0, table.getSize(), isLessThan);
	}
}

void Table::countingSortAge(int exp, int ascOrDesc) {
	Vector<Student> buckets[10];
	int size = table.getSize();
	for (int i = 0; i < size; ++i) {
		buckets[(table[i].getAge() / exp) % 10].add(table[i]);
	}
	int currIndex = 0;
	if (ascOrDesc) {
		for (int j = 9; j >= 0; --j) {
			for (int k = 0; k < buckets[j].getSize(); ++k) {
				table[currIndex++] = buckets[j][k];
			}
		}
	}
	else {
		for (int j = 0; j < 10; ++j) {
			for (int k = 0; k < buckets[j].getSize(); ++k) {
				table[currIndex++] = buckets[j][k];
			}
		}
	}
}

void Table::countingSortDate(int exp, int ascOrDesc) {
	Vector<Student> buckets[10];
	int size = table.getSize();
	for (int i = 0; i < size; ++i) {
		buckets[(table[i].getDate() / exp) % 10].add(table[i]);
	}
	int currIndex = 0;
	if (ascOrDesc) {
		for (int j = 9; j >= 0; --j) {
			for (int k = 0; k < buckets[j].getSize(); ++k) {
				table[currIndex++] = buckets[j][k];
			}
		}
	}
	else {
		for (int j = 0; j < 10; ++j) {
			for (int k = 0; k < buckets[j].getSize(); ++k) {
				table[currIndex++] = buckets[j][k];
			}
		}
	}
}

void Table::radixSortAge(int ascOrDesc) {
	for (int exp = 1, cnt = 1; cnt <= 3; exp *= 10, ++cnt) {
		countingSortAge(exp, ascOrDesc);
	}
}

void Table::radixSortDate(int ascOrDesc) {
	for (int exp = 1, cnt = 1; cnt <= 8; exp *= 10, ++cnt) {
		countingSortDate(exp, ascOrDesc);
	}
}

void Table::sortByAge(bool ascOrDesc) {
	radixSortAge(ascOrDesc);
}

void Table::sortBySubject(bool ascOrDesc) {
	if (ascOrDesc) {
		heapSort();
	}
	else {
		reverseHeapSort();
	}
}

// CountingSort
void Table::sortByGrade(bool ascOrDesc) {
	Vector<Student>* buckets = new Vector<Student>[5];
	for (int i = 0; i < table.getSize(); ++i) {
		buckets[table[i].getGrade() - 2].add(table[i]);
	}
	int currIndex = 0;
	if (ascOrDesc) {
		for (int j = 4; j >= 0; --j) {
			for (int k = buckets[j].getSize() - 1; k >= 0; --k) {
				table[currIndex++] = buckets[j][k];
			}
		}
	}
	else {
		for (int j = 0; j < 5; ++j) {
			for (int k = 0; k < buckets[j].getSize(); ++k) {
				table[currIndex++] = buckets[j][k];
			}
		}
	}
	delete[] buckets;
}

// radixSort ltm with a 'w' of 8
void Table::sortByDate(bool ascOrDesc) {
	radixSortDate(ascOrDesc);
}

void Table::print() const {
	int size = table.getSize();
	for (int i = 0; i < size; ++i) {
		std::cout << table[i] << std::endl;
	}
}

void Table::readFile(const MyString& path) {
	std::ifstream ifile(path);
	if (!ifile.is_open()) {
		std::cerr << "Coulnd't open file \"" << path << "\" for reading.\n";
		return;
	}
	Vector<Student> prev = table;
	char c;
	MyString buffer;
	// skips the first row
	ifile.seekg(33);
	while (ifile.get(c)) {
		if (!ifile.good()) {
			std::cerr << "Couldn't read from file.\n";
			ifile.clear();
			table = prev;
			return;
		}
		buffer.add(c);
		if (c == '\n') {
			Student temp(buffer);
			table.add(temp);
			buffer.free();
		}
	}
	if (buffer.getLength()) {
		Student temp(buffer);
		table.add(temp);
	}
	ifile.close();
}

void Table::siftUp(int index) {
	Student hand = table[index];
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (strcmp(hand.getSubject(), table[parent].getSubject()) < 0) {
			table[index] = table[parent];
			index = parent;
		}
		else {
			break;
		}
	}
	table[index] = hand;
}


void Table::siftDown(int n, int index) {
	Student hand = table[index];
	int succ = 2 * index + 1;
	if (succ < n - 1 && strcmp(table[succ + 1].getSubject(), table[succ].getSubject()) < 0) {
		++succ;
	}
	while (succ < n) {
		if (strcmp(hand.getSubject(), table[succ].getSubject()) > 0) {
			table[index] = table[succ];
			index = succ;
		}
		else {
			break;
		}
		succ = 2 * index + 1;
		if (succ < n - 1 && strcmp(table[succ + 1].getSubject(), table[succ].getSubject()) < 0) {
			++succ;
		}
	}
	table[index] = hand;
}

void Table::heapify() {
	int n = table.getSize();
	for (int i = 1; i < n; ++i) {
		siftUp(i);
	}
}

void Table::heapSort() {
	heapify();
	int n = table.getSize();
	for (int i = n - 1; i > 0; --i) {
		Student temp = table[0];
		table[0] = table[i];
		table[i] = temp;
		siftDown(--n, 0);
	}
}

void Table::reverseSiftUp(int index) {
	Student hand = table[index];
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (strcmp(hand.getSubject(), table[parent].getSubject()) > 0) {
			table[index] = table[parent];
			index = parent;
		}
		else {
			break;
		}
	}
	table[index] = hand;
}


void Table::reverseSiftDown(int n, int index) {
	Student hand = table[index];
	int succ = 2 * index + 1;
	if (succ < n - 1 && strcmp(table[succ + 1].getSubject(), table[succ].getSubject()) > 0) {
		++succ;
	}
	while (succ < n) {
		if (strcmp(hand.getSubject(), table[succ].getSubject()) < 0) {
			table[index] = table[succ];
			index = succ;
		}
		else {
			break;
		}
		succ = 2 * index + 1;
		if (succ < n - 1 && strcmp(table[succ + 1].getSubject(), table[succ].getSubject()) > 0) {
			++succ;
		}
	}
	table[index] = hand;
}

void Table::reverseHeapify() {
	int n = table.getSize();
	for (int i = 1; i < n; ++i) {
		reverseSiftUp(i);
	}
}

void Table::reverseHeapSort() {
	reverseHeapify();
	int n = table.getSize();
	for (int i = n - 1; i > 0; --i) {
		Student temp = table[0];
		table[0] = table[i];
		table[i] = temp;
		reverseSiftDown(--n, 0);
	}
}