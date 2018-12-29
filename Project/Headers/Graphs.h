#ifndef _GRAPHS_HEADER_
#define _GRAPHS_HEADER_
#include "DirectedGraph.h"
#include "UndirectedGraph.h"

enum Directed {
	isUndirected,
	isDirected
};

class Graphs {
public:
	Graphs();
	Graphs(const Graphs&);
	Graphs& operator=(const Graphs&);
	~Graphs();

	void createGraph(const String& ID, const Directed directed = isUndirected);
	void useGraph(const String& ID);
	void deleteGraph(const String& ID);

	void createNode(const String& ID);
	void deleteNode(const String& ID);
	void createEdge(const String& from, const String& to, int weight = 1);
	void deleteEdge(const String& from, const String& to);
	void search(const String& from, const String& to, const Algorithm algorithm = dfs) const;

	void saveGraphToFile() const;

	void printUsedGraph() const;

	int getSize() const;
	String getCurrGraphID() const;
private:
	void reallocate(int);
	void copyFrom(const Graphs&);
	void clear();
	int isIn(const String& ID) const;

	Graph** graphs;
	int currentGraph;
	int size;
	int allocSize;
};
#endif