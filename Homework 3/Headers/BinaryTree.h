#ifndef _BINARY_TREE_HEADER_
#define _BINARY_TREE_HEADER_
#include <iostream>
#include "Vector.h"

template <typename T>
class BinaryTree {
protected:
	class Node {
	public:
		Node* left;
		Node* right;
		T value;
		Node(const T& newVal, Node* newLeft = nullptr, Node* newRight = nullptr) :
			left(newLeft), right(newRight), value(newVal) {}
	};
public:
	BinaryTree();
	BinaryTree(const BinaryTree&);
	BinaryTree& operator=(const BinaryTree&);
	~BinaryTree();

	const T& getRoot() const;
	bool isEmpty() const;

	Vector<T> toVector() const;

	void insert(const T&);
	bool isIn(const T&);
	void print() const;
	void destroy();
private:
	void toVector(Node*, Vector<T>&) const;

	void copyFrom(const BinaryTree<T>&);
	void copyFrom(Node *);
	void insert(const T&, Node*);
	bool isIn(const T&, Node*);
	void print(Node*) const;
	void destroy(Node*);
protected:
	Node* root;
};

#include "BinaryTree.hpp"
#endif