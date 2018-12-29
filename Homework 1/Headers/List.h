#ifndef _LIST_HEADER_
#define _LIST_HEADER_

template <typename T>
class List {//source used: Ivan Mollov GitHub
	class Node {
	public:
		Node() {}
		Node(const Node& rhs) :value(rhs.value), next(rhs.next), prev(rhs.prev) {}
		T value;
		Node* next;
		Node* prev;
	};
public:
	class Iterator {
		friend class List;
		Iterator(Node* newIter) :iter(newIter) {}
	public:
		Iterator(const Iterator& rhs) { iter = rhs.iter; }
		Iterator& operator=(const Iterator& rhs) {
			if (this != &rhs) {
				iter = rhs.iter;
			}
			return *this;
		}
		T& operator*()const;
		Iterator operator--(int);
		Iterator& operator--();
		Iterator operator++(int);
		Iterator& operator++();
		bool operator==(const Iterator&)const;
		bool operator!=(const Iterator&)const;
	private:
		Node * iter;
	};
public:
	List();
	List(const T&);
	List(const List&);
	List& operator=(const List&);
	~List();
public:
	void append(const List&);
	void free();
	bool isEmpty()const;
	int getSize()const;
public:
	void pushFront(const T&);
	void pushBack(const T&);
	void popFront();
	void popBack();
	void pop(Iterator&);
	void insertAfter(Iterator&, const T&);
	void insertBefore(Iterator&, const T&);
private:
	void insertAfter(Node*, const T&);
	void insertBefore(Node*, const T&);
	void deleteNode(Node*);
public:
	Iterator begin();
	Iterator end();
private:
	Node dummy;
	int size;
};

#include "List.hpp"
#endif