#ifndef _FILES_HEADER_
#define _FILES_HEADER_
#include "FileTree.h"
#include <queue>

class Files {
	class Pair {
	public:
		Pair(int A, int B) :a(A), b(B) {}
		int a;
		int b;
	};
public:
	Files(const MyString&);

	void print(const MyString& filter);
	void refresh();
private:
	// loosely based on https://blog.jcoglan.com/2017/02/12/the-myers-diff-algorithm-part-1/
	void diffAlgorithm(const Vector<File>& vec1, const Vector<File>& vec2);
	void searchForNextPair(int& index1, int& index2,
		const Vector<File>& vec1, const Vector<File>& vec2);

	FileTree fTree1;
	FileTree fTree2;
};

#endif