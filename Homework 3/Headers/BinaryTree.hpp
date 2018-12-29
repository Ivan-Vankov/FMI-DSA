#include "BinaryTree.h"

template <typename T>
BinaryTree<T>::BinaryTree() :root(nullptr) {
}

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& rhs) : root(nullptr) {
	copyFrom(rhs);
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree<T>& rhs) {
	if (this != &rhs) {
		destroy();
		copyFrom(rhs);
	}
	return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
	destroy();
}

template <typename T>
const T& BinaryTree<T>::getRoot() const {
	return root->value;
}

template <typename T>
bool BinaryTree<T>::isEmpty() const {
	return !root;
}

template <typename T>
Vector<T> BinaryTree<T>::toVector() const {
	Vector<T> temp;
	toVector(root, temp);
	return temp;
}

template <typename T>
void BinaryTree<T>::toVector(Node* r, Vector<T>& v) const {
	if (!r) { return; }
	toVector(r->left, v);
	v.add(r->value);
	toVector(r->right, v);
}

template <typename T>
void BinaryTree<T>::copyFrom(const BinaryTree<T>& rhs) {
	copyFrom(rhs.root);
}

template <typename T>
void BinaryTree<T>::copyFrom(Node* r) {
	if (!r) { return; }
	insert(r->value);
	copyFrom(r->left);
	copyFrom(r->right);
}

template <typename T>
void BinaryTree<T>::insert(const T& elem) {
	if (!root) {
		root = new Node(elem);
	}
	else {
		insert(elem, root);
	}
}

template <typename T>
void BinaryTree<T>::insert(const T& elem, Node* r) {
	if (elem <= r->value) {
		if (r->left) {
			insert(elem, r->left);
		}
		else {
			r->left = new Node(elem);
		}
	}
	else if (elem > r->value) {
		if (r->right) {
			insert(elem, r->right);
		}
		else {
			r->right = new Node(elem);
		}
	}
}

template <typename T>
bool BinaryTree<T>::isIn(const T& val) {
	if (!root) {
		return false;
	}
	else {
		return isIn(val, root);
	}
}

template <typename T>
bool BinaryTree<T>::isIn(const T& val, Node* curr) {
	while (curr) {
		if (val < curr->value) {
			curr = curr->left;
			continue;
		}
		if (val > curr->value) {
			curr = curr->right;
			continue;
		}
		if (val == curr->value) {
			return true;
		}
	}
	return false;
}

template <typename T>
void BinaryTree<T>::print() const {
	print(root);
}

template <typename T>
void BinaryTree<T>::print(Node* r) const {
	if (!r) { return; }
	print(r->left);
	std::cout << r->value << std::endl;
	print(r->right);
}

template <typename T>
void BinaryTree<T>::destroy() {
	destroy(root);
	root = nullptr;
}

template <typename T>
void BinaryTree<T>::destroy(Node* curr) {
	if (curr) {
		destroy(curr->left);
		destroy(curr->right);
		delete curr;
	}
}