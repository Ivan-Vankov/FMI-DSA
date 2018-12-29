#ifndef _FILE_TREE_HEADER_
#define _FILE_TREE_HEADER_
#include "BinaryTree.h"
#include "Directory.h"
#include "windows.h"
#include <exception>
#include "File.h"
#include "Vector.h"

class FileTree : public BinaryTree<File> {
	class DirectoryTree : public BinaryTree<Directory> {
	public:
		DirectoryTree();
		DirectoryTree(const MyString& dir, FileTree* parent);
		Directory* getFilesAndNextDirUnder(Directory* dir, FileTree* parent);
		void initialize(const MyString& dir, FileTree* parent);
	};
	class dirStack {
		struct Node {
			Node* next;
			Directory* value;
		};
	public:
		dirStack();
		~dirStack();

		void pop();
		void push(Directory*);
		Directory* getTop() const;
		bool isEmpty() const;
	private:
		void free();

		Node* top;
	};
public:
	FileTree();
	FileTree(const MyString& dir);
	FileTree& operator=(const FileTree& dir);
	FileTree& operator=(const MyString& dir);
	~FileTree();

	MyString getDir() const;

	bool isValid() const;

	void print(const MyString& filter) const;

	Vector<File> toVector(const MyString& filter) const;
private:
	void destroyDTreeAndKeepRoot();

	void print(Node*, const MyString& filter) const;

	void vectorFilter(Node*, Vector<File>& vec, const MyString& filter) const;

	DirectoryTree dTree;
};

#endif