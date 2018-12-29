#include "FileTree.h"

FileTree::FileTree() {
}

FileTree::FileTree(const MyString& dir) :dTree(dir, this) {
	destroyDTreeAndKeepRoot();
}

FileTree& FileTree::operator=(const FileTree& dir) {
	if (this != &dir) {
		this->BinaryTree::operator=(dir);
		dTree.DirectoryTree::BinaryTree::operator=(dir.dTree);
	}
	return *this;
}

FileTree& FileTree::operator=(const MyString& dir) {
	this->destroy();
	dTree.destroy();
	dTree.initialize(dir, this);
	destroyDTreeAndKeepRoot();

	return *this;
}

void FileTree::destroyDTreeAndKeepRoot() {
	Directory temp = dTree.getRoot();
	dTree.destroy();
	dTree.insert(temp);
}

FileTree::~FileTree() {
}

FileTree::DirectoryTree::DirectoryTree() {
}

MyString FileTree::getDir() const {
	return dTree.getRoot().getName();
}

bool FileTree::isValid() const {
	return !dTree.isEmpty();
}

void FileTree::print(const MyString& filter) const {
	try {
		int filterWordCount = filter.countWords();
		if (filterWordCount > 1) {
			throw std::exception("Too many words in filter.");
		}
		if (!filterWordCount) {
			return BinaryTree::print();
		}
		else {
			print(root, filter);
		}
	}
	catch (std::exception&) {
		throw;
	}
}

void FileTree::print(Node* r, const MyString& filter) const {
	if (!r) { return; }
	print(r->left, filter);
	if (r->value.hasExtention(filter)) {
		std::cout << r->value << std::endl;
	}
	print(r->right, filter);
}

Vector<File> FileTree::toVector(const MyString& filter) const {
	try {
		int filterWordCount = filter.countWords();
		if (filterWordCount > 1) {
			throw std::exception("Too many words in filter.");
		}
		if (!filterWordCount) {
			return BinaryTree::toVector();
		}
		else {
			Vector<File> temp;
			vectorFilter(root, temp, filter);
			return temp;
		}
	}
	catch (std::exception&) {
		throw;
	}
}

void FileTree::vectorFilter(Node* r, Vector<File>& vec, const MyString& filter) const {
	if (!r) { return; }
	vectorFilter(r->left, vec, filter);
	if (r->value.hasExtention(filter)) {
		vec.add(r->value);
	}
	vectorFilter(r->right, vec, filter);
}

FileTree::DirectoryTree::DirectoryTree(const MyString& dir, FileTree* parent) {
	initialize(dir, parent);
}

void FileTree::DirectoryTree::initialize(const MyString& dir, FileTree* parent) {
	try {
		Directory* start = new Directory(dir);
		dirStack stack;
		insert(*start);
		stack.push(start);
		do {
			Directory* curr = stack.getTop();
			Directory* next = getFilesAndNextDirUnder(curr, parent);
			if (next->isValid() && !isIn(*next)) {
				stack.push(next);
				insert(*next);
				continue;
			}
			stack.pop();
		} while (!stack.isEmpty());
	}
	catch (std::exception&) {
		throw;
	}
}

Directory* FileTree::DirectoryTree::getFilesAndNextDirUnder(Directory* dir, FileTree* parent) {
	try {
		MyString path = dir->getName();
		WIN32_FIND_DATA findFileData;
		while (dir->findNextFile(&findFileData)) {
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				return new Directory(path + "\\" + findFileData.cFileName);
			}
			else {
				MyString newPath = path;
				newPath + "\\" + findFileData.cFileName;
				parent->insert(File(newPath, findFileData.nFileSizeLow));
			}
		}
		if (GetLastError() == ERROR_NO_MORE_FILES) {
			return new Directory("", false);
		}
		if (GetLastError() != ERROR_NO_MORE_FILES) {
			throw std::exception("Couldn't read all files.");
		}
		return new Directory("");
	}
	catch (std::exception&) {
		throw;
	}
}

FileTree::dirStack::dirStack() :top(nullptr) {
}


FileTree::dirStack::~dirStack() {
	free();
}

void FileTree::dirStack::free() {
	while (top) {
		delete top->value;
		top = top->next;
	}
}

void FileTree::dirStack::pop() {
	Node* n = top->next;
	delete top;
	top = n;
}

void FileTree::dirStack::push(Directory* newLoad) {
	Node* n = new Node;
	n->value = newLoad;
	n->next = top;
	top = n;
}

Directory* FileTree::dirStack::getTop() const {
	return top->value;
}

bool FileTree::dirStack::isEmpty() const {
	return !top;
}