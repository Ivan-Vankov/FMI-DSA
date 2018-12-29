#include "Files.h"

Files::Files(const MyString& dir) :fTree1(dir) {
}

void Files::print(const MyString& filter) {
	if (!fTree2.isValid()) {
		fTree1.print(filter);
	}
	else {
		diffAlgorithm(fTree1.toVector(filter), fTree2.toVector(filter));
	}
}

void Files::refresh() {
	if (!fTree2.isValid()) {
		fTree2 = fTree1.getDir();
	}
	else {
		fTree1 = fTree2;
		fTree2 = fTree1.getDir();
	}
}

void Files::diffAlgorithm(const Vector<File>& vec1, const Vector<File>& vec2) {
	if (!vec1.getSize() || !vec2.getSize()) {
		return;
	}
	int size1 = vec1.getSize() - 1;
	int size2 = vec2.getSize() - 1;
	int index1 = 0;
	int index2 = 0;
	do {
		int initIndex1 = index1;
		int initIndex2 = index2;
		searchForNextPair(index1, index2, vec1, vec2);
		if (initIndex1 == index1 && initIndex2 == index2 &&
			!(vec1[index1] == vec2[index2])) {
			break;
		}
		for (int i = initIndex1; i < index1; ++i) {
			std::cout << "(-)" << vec1[i] << std::endl;
		}
		for (int j = initIndex2; j < index2; ++j) {
			std::cout << "(+)" << vec2[j] << std::endl;
		}
		if (vec1[index1] == vec2[index2]) {
			std::cout << vec1[index1] << std::endl;
			if (index1 == size1 || index2 == size2) {
				++index1;
				++index2;
				break;
			}
		}
	} while (index1++ != size1 && index2++ != size2);
	for (; index1 <= size1; ++index1) {
		std::cout << "(-)" << vec1[index1] << std::endl;
	}
	for (; index2 <= size2; ++index2) {
		std::cout << "(+)" << vec2[index2] << std::endl;
	}
}

void Files::searchForNextPair(int& index1, int& index2,
	const Vector<File>& vec1, const Vector<File>& vec2) {
	int size1 = vec1.getSize() - 1;
	int size2 = vec2.getSize() - 1;
	std::queue<Pair> wave;
	if (vec1[index1] == vec2[index2]) {
		return;
	}
	wave.push(Pair(index1, index2));
	int init2 = index2;
	while (!wave.empty()) {
		int frontB = wave.front().b;
		int frontA = wave.front().a;
		if (frontB + 1 <= size2) {
			if (vec1[frontA] == vec2[frontB + 1]) {
				index1 = frontA;
				index2 = frontB + 1;
				return;
			}
			wave.push(Pair(frontA, frontB + 1));
		}
		if (frontA + 1 <= size1 && frontB == init2) {
			if (vec1[frontA + 1] == vec2[frontB]) {
				index1 = frontA + 1;
				index2 = frontB;
				return;
			}
			wave.push(Pair(frontA + 1, frontB));
		}
		wave.pop();
	}
}