#ifndef _GRAPH_HEADER_
#define _GRAPH_HEADER_
#include "Vector.h"
#include "String.h"
#include "fnv.h"
#include <fstream>
#include <exception>
#include <queue>
#include <list>

// |
// v Vector		
// [aa],0                                           ([ade],1) <- NeighbourAndWeight
// [ba],0                                               ^
// [ca],0                                               |
// [da],0                                           ([abc],1)
// [ea],0                                               ^
// [fa],0 -> Vector<String,pair<string,Vector<NandW>>>  | Vector
// [ga],0 -> [gb],1	->                                [gc],2
// [ha],0
// 

enum Algorithm {
	bfs,
	dfs,
	dijkstra
};

class Graph {
public:
	virtual void addNode(const String&) = 0;
	virtual void removeNode(const String&) = 0;
	virtual void addEdge(const String& from, const String& to, int weight) = 0;
	virtual void removeEdge(const String& from, const String& to) = 0;
	virtual void search(const String& from, const String& to, const Algorithm algorithm = dfs) const = 0;

	// Saves the graph to a text file
	virtual void saveToFile() const = 0;
	// Cleaning preserves the graph's ID and vertex count
	virtual void cleanGraph() = 0;
	// Reads the graph from a file with name <GraphID>
	virtual void refreshGraph() = 0;

	virtual const String& getGraphID() const = 0;
	virtual bool isDirected() const = 0;

	virtual void print() const = 0;

	virtual ~Graph() {}
};

#endif
