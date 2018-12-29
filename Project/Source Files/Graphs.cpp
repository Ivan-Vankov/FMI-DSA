#include "Graphs.h"

Graphs::Graphs() :graphs(nullptr), currentGraph(-1), size(0), allocSize(0) {}

Graphs::Graphs(const Graphs& other) {
	copyFrom(other);
}

Graphs& Graphs::operator=(const Graphs& other) {
	if (this != &other) {
		clear();
		copyFrom(other);
	}
	return *this;
}

Graphs::~Graphs() {
	clear();
}

void Graphs::createGraph(const String& ID, const Directed directed) {
	try {
		// Check if a graph with the same ID exists already
		if (isIn(ID) != -1) {
			throw std::exception("A graph with the same ID is already created.");
		}
		// If graphs will overflow with one more graph
		// we resize
		if (size == allocSize) {
			reallocate(size ? 2 * size : 1);
		}
		// Add a graph with 
		if (directed) {
			String fileName = "DirectedGraphs\\";
			graphs[size++] = new DirectedGraph(fileName + ID);
		}
		else {
			String fileName = "UndirectedGraphs\\";
			graphs[size++] = new UndirectedGraph(fileName + ID);
		}
		currentGraph = size - 1;
	}
	catch (std::bad_alloc&) {
		--size;
		throw;
	}
}

void Graphs::useGraph(const String& ID) {
	// Check if a graph with this ID is in graphs
	int index = isIn(ID);
	// If it isn't we say so
	if (index == -1) {
		String except = "No graph with with ID <";
		throw std::exception(except + ID + "> is created.");
	}
	// If we try to use our current graph
	// then we do nothing
	if (index == currentGraph) {
		String except = "Graph <";
		throw std::exception(except + ID + "> is already in use.");
	}
	// Otherwise we save the currently used graph to its file
	// and then clean it
	graphs[currentGraph]->saveToFile();
	graphs[currentGraph]->cleanGraph();
	// Current graph becomes that graph's index
	graphs[index]->refreshGraph();
	currentGraph = index;
}

void Graphs::deleteGraph(const String& ID) {
	// Check if a graph with this ID is in graphs
	int index = isIn(ID);
	// If it isn't we say so
	// and do nothing else
	if (index == -1) {
		String except = "No graph with with ID <";
		throw std::exception(except + ID + "> is created.");
	}
	// Otherwise we save it and delete it
	graphs[index]->saveToFile();
	delete graphs[index];
	graphs[index] = nullptr;
	// If the deleted graph was the current on
	// then we will set the current one to be invalid
	if (index == currentGraph) {
		currentGraph = -1;
	}
	// Switch the deleted graph and the last one
	graphs[index] = graphs[--size];
	// And if the current graph was the last one
	// then we switch currentGraph as well
	if (currentGraph == size) {
		currentGraph = index;
	}
	// And we make it so that there is no graph selected
	currentGraph = -1;
}

void Graphs::createNode(const String& ID) {
	// Check if there is a selected graph
	if (currentGraph == -1) {
		throw std::exception("No graph is selected.");
	}
	graphs[currentGraph]->addNode(ID);
}

void Graphs::deleteNode(const String& ID) {
	// Check if there is a selected graph
	if (currentGraph == -1) {
		throw std::exception("No graph is selected.");
	}
	graphs[currentGraph]->removeNode(ID);
}

void Graphs::createEdge(const String& from, const String& to, int weight) {
	// Check if there is a selected graph
	if (currentGraph == -1) {
		throw std::exception("No graph is selected.");
	}
	graphs[currentGraph]->addEdge(from, to, weight);
}

void Graphs::deleteEdge(const String& from, const String& to) {
	// Check if there is a selected graph
	if (currentGraph == -1) {
		throw std::exception("No graph is selected.");
	}
	graphs[currentGraph]->removeEdge(from, to);
}

void Graphs::search(const String& from, const String& to, const Algorithm algorithm) const {
	// If there is no currently selected graph
	// we say so and do nothing more
	if (currentGraph == -1) {
		throw std::exception("No graph currently in use.");
	}
	graphs[currentGraph]->search(from, to, algorithm);
}

void Graphs::saveGraphToFile() const {
	// If there is no currently selected graph
	// we say so and do nothing more
	if (currentGraph == -1) {
		throw std::exception("No graph currently in use.");
	}
	graphs[currentGraph]->saveToFile();
}

void Graphs::printUsedGraph() const {
	// If there is no currently selected graph
	// we say so and do nothing more
	if (currentGraph == -1) {
		throw std::exception("No graph currently in use.");
	}
	graphs[currentGraph]->print();
}

int Graphs::getSize() const {
	return size;
}

String Graphs::getCurrGraphID() const {
	if (currentGraph == -1) {
		throw std::exception("No graph currently in use.");
	}
	return graphs[currentGraph]->getGraphID();
}

void Graphs::reallocate(int newAllocSize) {
	try {
		// Make a buffer of graph pointers
		Graph** buff = new Graph*[newAllocSize];
		// Add all possible valid graphs to it
		int lowerBound = size < newAllocSize ? size : newAllocSize;
		for (int i = 0; i < lowerBound; ++i) {
			if (graphs[i]->isDirected()) {
				buff[i] = new DirectedGraph(graphs[i]->getGraphID());
				delete graphs[i];
			}
			else {
				buff[i] = new UndirectedGraph(graphs[i]->getGraphID());
				delete graphs[i];
			}
		}
		// Delete graphs and set them equal to buffer
		delete[] graphs;
		graphs = buff;
		if (size > newAllocSize) {
			size = newAllocSize;
		}
		allocSize = newAllocSize;
	}
	catch (std::bad_alloc&) {
		clear();
		throw;
	}
}

void Graphs::copyFrom(const Graphs& other) {
	try {
		// Allocate a buffer of Graph pointers
		Graph** buff = new Graph*[other.allocSize];
		// For each graph in other
		for (int i = 0; i < other.size; ++i) {
			// Allocate a new one in the buffer
			if (other.graphs[i]->isDirected()) {
				buff[i] = new DirectedGraph(other.graphs[i]->getGraphID());
			}
			else {
				buff[i] = new UndirectedGraph(other.graphs[i]->getGraphID());
			}
		}
		graphs = buff;
		size = other.size;
		allocSize = other.allocSize;
		currentGraph = other.currentGraph;
	}
	catch (std::bad_alloc&) {
		clear();
		throw;
	}
}

void Graphs::clear() {
	for (int i = 0; i < size; ++i) {
		delete graphs[i];
	}
	delete[] graphs;
	graphs = nullptr;
	size = 0;
	allocSize = 0;
	currentGraph = -1;
}

int Graphs::isIn(const String& ID) const {
	for (int i = 0; i < size; ++i) {
		if (graphs[i]->getGraphID() == ID) {
			return i;
		}
	}
	return -1;
}