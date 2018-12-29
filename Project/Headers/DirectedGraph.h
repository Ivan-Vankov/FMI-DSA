#ifndef _DIRECTEDGRAPH_HEADER_
#define _DIRECTEDGRAPH_HEADER_
#include "Graph.h"

class DirectedGraph :public Graph {
	class NodeAndItsEdges {
	public:
		NodeAndItsEdges() {}
		NodeAndItsEdges(const NodeAndItsEdges& other);
		NodeAndItsEdges& operator=(const NodeAndItsEdges& other);
		NodeAndItsEdges(const String& ID) { nodeAndEdges.first = ID; }
		void readLine(std::ifstream&);

		bool operator<(const NodeAndItsEdges& other) const {
			return strcmp(nodeAndEdges.first, other.nodeAndEdges.first) < 0;
		}

		bool operator>(const NodeAndItsEdges& other) const {
			return strcmp(nodeAndEdges.first, other.nodeAndEdges.first) > 0;
		}

		std::pair<String, Vector<std::pair<String, int>>> nodeAndEdges;
		Vector<NodeAndItsEdges*> parents;
	};
	// Utility classes for the searching algorithms
	class VisitedNodesDfs {
	public:
		// This class has the same structure as DirectedGraph
		VisitedNodesDfs(int hashSize) :hashTable(hashSize) {}
		void makeVisited(const String& ID) {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			hashTable[hashIndex].add(ID);
		}
		// A check if a node is visited
		bool isVisited(const String& ID) const {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i] == ID) {
					return true;
				}
			}
			return false;
		}
		//			  NodeID
		Vector<Vector<String>> hashTable;
	};

	class VisitedNodesBfs {
	public:
		// This class has the same structure as DirectedGraph
		VisitedNodesBfs(int hashSize) :hashTable(hashSize) {}
		void makeVisited(const String& ID, const String& parent) {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			hashTable[hashIndex].add(std::make_pair(ID, parent));
		}
		// A check if a node is visited
		bool isVisited(const String& ID) const {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					return true;
				}
			}
			return false;
		}
		const String& getNodeParent(const String& ID) const {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					return hashTable[hashIndex][i].second;
				}
			}
			// Will never go here
			// but still write it to remove warnings
			return hashTable[0][0].first;
		}
		//                      NodeID ParentId
		Vector<Vector<std::pair<String, String>>> hashTable;
	};

	class VisitedNodesDijkstra {
	public:
		// This class has the same structure as DirectedGraph
		VisitedNodesDijkstra(int hashSize) : hashTable(hashSize) {}
		void makeVisited(const String& ID, const String& parent, int weight) {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			hashTable[hashIndex].add(std::make_pair(ID, std::make_pair(parent, weight)));
		}
		// A check if a node is visited
		bool isVisited(const String& ID) const {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					return true;
				}
			}
			return false;
		}
		const String& getNodeParent(const String& ID) const {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					return hashTable[hashIndex][i].second.first;
				}
			}
			// Will never go here
			// but will still write it to remove warnings
			return hashTable[0][0].first;
		}
		int getNodeWeight(const String& ID) const {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					return hashTable[hashIndex][i].second.second;
				}
			}
			// Will never go here
			// but will still write it to remove warnings
			return 0;
		}
		void setNodeWeight(const String& ID, int weight) {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					hashTable[hashIndex][i].second.second = weight;
					return;
				}
			}
		}
		void setFatherNode(const String& ID, const String& FatherID) {
			int hashIndex = fnvHash(ID) % hashTable.getAllocatedSize();
			for (int i = 0; i < hashTable[hashIndex].getSize(); ++i) {
				if (hashTable[hashIndex][i].first == ID) {
					hashTable[hashIndex][i].second.first = FatherID;
					return;
				}
			}
		}
		//                      NodeID           ParentId weight
		Vector<Vector<std::pair<String, std::pair<String, int>>>> hashTable;
	};
public:

	DirectedGraph();
	// We asume all files from which we read are valid and properly formated
	DirectedGraph(const String& fileName);
	virtual ~DirectedGraph();

	virtual void addNode(const String&);
	virtual void removeNode(const String&);
	virtual void addEdge(const String& from, const String& to, int weight = 1);
	virtual void removeEdge(const String& from, const String& to);
	virtual void search(const String& from, const String& to, const Algorithm algorithm = dfs) const;

	virtual void saveToFile() const;
	virtual void cleanGraph();
	virtual void refreshGraph();

	virtual const String& getGraphID() const { return GraphID; }
	virtual bool isDirected() const { return true; }

	virtual void print() const;

private:
	void readFromFile(const String&);
	void createFile(const String&) const;
	void removeFile(const String&) const;
	bool endsInDotTXT(const String&) const;

	bool fileExists(const String& src) const;

	void searchDfs(const String& from, const String& to,
		unsigned& fromHashIndex, int& fromIndex) const;
	void searchBfs(const String& from, const String& to,
		unsigned& fromHashIndex, int& fromIndex) const;
	void searchDijkstra(const String& from, const String& to,
		unsigned& fromHashIndex, int& fromIndex) const;

	void nullifyVariables();
	void rehash();

	void addNodeAndItsEdges(const NodeAndItsEdges&);

	int getNodeIndex(const String&, unsigned hashCode) const;

	void updateAllNodeParents();
	void addNodeAsParent(const String& child, NodeAndItsEdges& parent);

	void nodesExistCheck(const String& from, const String& to,
		unsigned& fromHashIndex, int& fromIndex,
		unsigned& toHashIndex, int& toIndex) const;

	// Returns the index of the edge if it is found
	int edgeExists(const String& to,
		unsigned& fromHashIndex, int& fromIndex) const;

	void setIDfromFileName(const String& fileName, int indexBeforeDotTXT);

	int vertexCount;
	String GraphID;
	Vector<Vector<NodeAndItsEdges>> directedGraph;
};

#endif