#include "DirectedGraph.h"

DirectedGraph::DirectedGraph() :vertexCount(0), GraphID("UnknownGraph"), directedGraph() {}

DirectedGraph::DirectedGraph(const String& fileName) {
	nullifyVariables();
	readFromFile(fileName);
}

DirectedGraph::~DirectedGraph() {
	nullifyVariables();
}

// In the copy constructor and operator= of NodeAndItsEdges we only copy the nodeAndEdges vector
// and not the parents vector
// That is because these methods are only used in rehash() where copying the parents vector is wasteful 
DirectedGraph::NodeAndItsEdges::NodeAndItsEdges(const NodeAndItsEdges& other) :nodeAndEdges(other.nodeAndEdges) {}

DirectedGraph::NodeAndItsEdges& DirectedGraph::NodeAndItsEdges::operator=(const NodeAndItsEdges& other) {
	if (this != &other) {
		nodeAndEdges = other.nodeAndEdges;
	}
	return *this;
}

void DirectedGraph::addNode(const String& nodeID) {
	// Hash the nodeID
	unsigned hashIndex = fnvHash(nodeID) % directedGraph.getAllocatedSize();
	// Search for it in the graph
	for (int i = 0; i < directedGraph[hashIndex].getSize(); ++i) {
		// If we find a node with the same id in the graph
		// then we say so and do nothing more
		if (directedGraph[hashIndex][i].nodeAndEdges.first == nodeID) {
			String except = "A node with ID <";
			throw std::exception(except + nodeID + "> already exists in the graph.");
		}
	}
	// If the nodeID in new then we add a node with that ID to the graph
	directedGraph[hashIndex].add(nodeID);
	++vertexCount;
}

void DirectedGraph::removeNode(const String& nodeID) {
	// Hash the nodeID
	unsigned hashIndex = fnvHash(nodeID) % directedGraph.getAllocatedSize();
	// Search for it in the graph
	int nodeIndex = getNodeIndex(nodeID, hashIndex);
	// If we don't find a node with that ID in the graph
	// then we say so and do nothing more
	if (nodeIndex == -1) {
		String except = "A node with ID <";
		throw  std::exception(except + nodeID + "> doesn't exist in the graph.");
	}
	// If there is a node with that id in the graph
	// then we remove all of the adges that end in it
	for (int i = 0; i < directedGraph[hashIndex][nodeIndex].parents.getSize(); ++i) {
		// Get the hash of the i-th parent
		unsigned currParentHash =
			fnvHash(directedGraph[hashIndex][nodeIndex].parents[i]->nodeAndEdges.first)
			% directedGraph.getAllocatedSize();
		// Get the number of children of the current parent
		int currParentChildrenCount = directedGraph[hashIndex][nodeIndex].parents[i]->nodeAndEdges.second.getSize();
		// Search for our node in the current parent's children
		for (int j = 0; j < currParentChildrenCount; ++j) {
			// When we find it we remove it
			if (directedGraph[hashIndex][nodeIndex].parents[i]->nodeAndEdges.second[j].first
				== nodeID) {
				directedGraph[hashIndex][nodeIndex].parents[i]->nodeAndEdges.second.remove(j);
				break;
			}
		}
	}
	// and itself
	directedGraph[hashIndex].remove(nodeIndex);
	--vertexCount;
}

void DirectedGraph::addEdge(const String& from, const String& to, int weight) {
	unsigned fromHashIndex, toHashIndex;
	int fromIndex, toIndex;
	// Check if the edges exist and get their hashes and indexes
	nodesExistCheck(from, to, fromHashIndex, fromIndex, toHashIndex, toIndex);
	if (fromIndex == -1 || toIndex == -1) {
		return;
	}
	// Check if the edge already exists
	int isIn = edgeExists(to, fromHashIndex, fromIndex);
	if (isIn == -1) {
		// If both nodes exist and the edge doesn't then we add the edge between them
		std::pair<String, int> newEdge(to, weight);
		directedGraph[fromHashIndex][fromIndex].nodeAndEdges.second.add(newEdge);
		// and we add <from> to the parents of <to>
		directedGraph[toHashIndex][toIndex].parents.add(&directedGraph[fromHashIndex][fromIndex]);
		std::cout << "Edge from <" << from << "> to <" << to << "> is created." << std::endl;
	}
	else {
		// If the edge exists then we simply update its weight
		directedGraph[fromHashIndex][fromIndex].nodeAndEdges.second[isIn].second = weight;
		std::cout << "Edge from <" << from << "> to <" << to << "> is updated with weight " << weight << "." << std::endl;
	}
}

void DirectedGraph::removeEdge(const String& from, const String& to) {
	unsigned fromHashIndex, toHashIndex;
	int fromIndex, toIndex;
	// Check if the edges exist and get their hashes and indexes
	nodesExistCheck(from, to, fromHashIndex, fromIndex, toHashIndex, toIndex);
	if (fromIndex == -1 || toIndex == -1) {
		return;
	}
	// Check if the edge already exists
	int isIn = edgeExists(to, fromHashIndex, fromIndex);
	if (isIn == -1) {
		String except = "No edge from <";
		except + from + "> to <";
		throw  std::exception(except + to + "> exists.");
	}
	// If both the nodes and the exist then we remove the edge:
	// First we remove the edge: <from> <to>
	directedGraph[fromHashIndex][fromIndex].nodeAndEdges.second.remove(isIn);

	// and we remove <from> from the parents of <to>:
	// First we look for <from> in the parents of <to>
	int toParentsCount = directedGraph[toHashIndex][toIndex].parents.getSize();
	for (int j = 0; j < directedGraph[toHashIndex][toIndex].parents.getSize(); ++j) {
		// When it is found
		if (directedGraph[toHashIndex][toIndex].parents[j]->nodeAndEdges.first == to) {
			// It is removed
			directedGraph[toHashIndex][toIndex].parents.remove(j);
			break;
		}
	}
}

void DirectedGraph::search(const String& from, const String& to, const Algorithm algorithm) const {
	unsigned fromHashIndex, toHashIndex;
	int fromIndex, toIndex;
	// Check if the edges exist and get their hashes and indexes
	nodesExistCheck(from, to, fromHashIndex, fromIndex, toHashIndex, toIndex);
	if (fromIndex == -1 || toIndex == -1) {
		return;
	}
	// Look through the known algorithms and execute one
	if (algorithm == bfs) {
		searchBfs(from, to,
			fromHashIndex, fromIndex);
	}
	if (algorithm == dfs) {
		searchDfs(from, to,
			fromHashIndex, fromIndex);
	}
	if (algorithm == dijkstra) {
		searchDijkstra(from, to,
			fromHashIndex, fromIndex);
	}
}

void DirectedGraph::searchDfs(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	if (from == to) {
		std::cout << from << std::endl;
		return;
	}
	// Here we store the visited nodes
	VisitedNodesDfs visited(directedGraph.getAllocatedSize());
	visited.makeVisited(from);
	Vector<NodeAndItsEdges> specialStack;
	specialStack.add(directedGraph[fromHashIndex][fromIndex]);
	while (specialStack.getSize()) {
		NodeAndItsEdges temp = specialStack.last();
		// We search through all of temps edges
		int i = 0;
		for (; i < temp.nodeAndEdges.second.getSize(); ++i) {
			// If we find <to>
			if (temp.nodeAndEdges.second[i].first == to) {
				// We print out how we got here
				std::cout << "The path from <" << from << "> to <" << to << "> is:" << std::endl;
				for (int i = 0; i < specialStack.getSize(); ++i) {
					std::cout << specialStack[i].nodeAndEdges.first << " -> ";
				}
				// and finish
				std::cout << to << std::endl;
				return;
			}
			// If we find an unvisited child
			if (!visited.isVisited(temp.nodeAndEdges.second[i].first)) {
				// Find it in directedGraph
				int hashIndex = fnvHash(temp.nodeAndEdges.second[i].first) % directedGraph.getAllocatedSize();
				int actualIndex = getNodeIndex(temp.nodeAndEdges.second[i].first, hashIndex);
				// and we start dfs with it again
				specialStack.add(directedGraph[hashIndex][actualIndex]);
				// and mark it as visited
				visited.makeVisited(temp.nodeAndEdges.second[i].first);
				break;
			}
		}
		// If temp had no unvisited children
		if (i == temp.nodeAndEdges.second.getSize()) {
			// Pop it from the specialStack
			specialStack.remove(specialStack.getSize() - 1);
		}
	}
	std::cerr << "No path from <" << from << "> to <" << to << "> exists." << std::endl;
}

void DirectedGraph::searchBfs(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	if (from == to) {
		std::cout << from << std::endl;
		return;
	}
	// Here we store the visited nodes
	VisitedNodesBfs visited(directedGraph.getAllocatedSize());
	std::queue<NodeAndItsEdges> queue;
	queue.push(directedGraph[fromHashIndex][fromIndex]);
	visited.makeVisited(from, "NoParent");
	while (!queue.empty()) {
		NodeAndItsEdges temp = queue.front();
		// We search through all of temps edges
		for (int i = 0; i < temp.nodeAndEdges.second.getSize(); ++i) {
			// If we find <to>
			if (temp.nodeAndEdges.second[i].first == to) {
				// We trace back the visited nodes through their parents
				std::list<String> trace;
				String currNode = temp.nodeAndEdges.first;
				while (currNode != from) {
					trace.push_front(currNode);
					currNode = visited.getNodeParent(currNode);
				}
				trace.push_front(currNode);
				// print them
				std::cout << "The path from <" << from << "> to <" << to << "> is:" << std::endl;
				for (std::list<String>::iterator i = trace.begin(); i != trace.end(); ++i) {
					std::cout << *i << " -> ";
				}
				// and finish
				std::cout << to << std::endl;
				return;
			}
			// When we find an unvisited one
			if (!visited.isVisited(temp.nodeAndEdges.second[i].first)) {
				// Find it int directedGraph
				int hashIndex = fnvHash(temp.nodeAndEdges.second[i].first) % directedGraph.getAllocatedSize();
				int actualIndex = getNodeIndex(temp.nodeAndEdges.second[i].first, hashIndex);
				// push it to the queue
				queue.push(directedGraph[hashIndex][actualIndex]);
				// and mark it as visited (and make temp its parent)
				visited.makeVisited(directedGraph[hashIndex][actualIndex].nodeAndEdges.first,
					temp.nodeAndEdges.first);
			}
		}
		// Finally pop temp from the queue
		queue.pop();
	}
	std::cerr << "No path from <" << from << "> to <" << to << "> exists." << std::endl;
}

void DirectedGraph::searchDijkstra(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	if (from == to) {
		std::cout << from << std::endl
			<< 0 << std::endl;
		return;
	}
	VisitedNodesDijkstra visited(directedGraph.getAllocatedSize());
	visited.makeVisited(from, "NoParent", 0);
	std::priority_queue<NodeAndItsEdges> queue;
	queue.push(directedGraph[fromHashIndex][fromIndex]);
	while (!queue.empty()) {
		NodeAndItsEdges temp = queue.top();
		queue.pop();
		// We search through all of temps edges
		for (int i = 0; i < temp.nodeAndEdges.second.getSize(); ++i) {
			// Find the i-th neighbour
			int hashIndex = fnvHash(temp.nodeAndEdges.second[i].first) % directedGraph.getAllocatedSize();
			int actualIndex = getNodeIndex(temp.nodeAndEdges.second[i].first, hashIndex);
			// If its not visited
			if (!visited.isVisited(temp.nodeAndEdges.second[i].first)) {
				// Mark it as visited, make temp its parent 
				// and set its weight to infinity
				visited.makeVisited(temp.nodeAndEdges.second[i].first,
					temp.nodeAndEdges.first, INT_MAX);
			}
			// If the weight of the temp + 
			// the weight of the edge from temp to the current child
			// is < the current childs weight
			int tempWeight = visited.getNodeWeight(temp.nodeAndEdges.first);
			int edgeWeight = temp.nodeAndEdges.second[i].second;
			int currChildWeight = visited.getNodeWeight(temp.nodeAndEdges.second[i].first);
			if (tempWeight + edgeWeight < currChildWeight) {
				// Set its weight to tempWeight + edgeWeight
				visited.setNodeWeight(temp.nodeAndEdges.second[i].first, tempWeight + edgeWeight);
				// update its father node
				visited.setFatherNode(temp.nodeAndEdges.second[i].first, temp.nodeAndEdges.first);
				// and push it to the queue
				queue.push(directedGraph[hashIndex][actualIndex]);
			}
		}
	}
	// If we find <to>
	if (visited.isVisited(to)) {
		// We trace back the visited nodes through their parents
		std::list<String> trace;
		String currNode = visited.getNodeParent(to);
		while (currNode != from) {
			trace.push_front(currNode);
			currNode = visited.getNodeParent(currNode);
		}
		trace.push_front(currNode);
		// print them
		std::cout << "The path from <" << from << "> to <" << to << "> is:" << std::endl;
		for (std::list<String>::iterator j = trace.begin(); j != trace.end(); ++j) {
			std::cout << *j << " -> ";
		}
		// and finish
		std::cout << to << std::endl <<
			visited.getNodeWeight(to) << std::endl;
		return;
	}
	else {
		std::cerr << "No path from <" << from << "> to <" << to << "> exists." << std::endl;
	}
}

int DirectedGraph::edgeExists(const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	// For every edge starting from <from>
	int edgeCount = directedGraph[fromHashIndex][fromIndex].nodeAndEdges.second.getSize();
	for (int i = 0; i < edgeCount; ++i) {
		// If the edge is found
		// then its index is returned
		if (directedGraph[fromHashIndex][fromIndex].nodeAndEdges.second[i].first == to) {
			return i;
		}
	}
	// If the edge is not found
	return -1;
}

void DirectedGraph::nodesExistCheck(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex,
unsigned& toHashIndex, int& toIndex) const {
	// Hash the <from> nodeID
	fromHashIndex = fnvHash(from) % directedGraph.getAllocatedSize();
	// Search for it in the graph
	fromIndex = getNodeIndex(from, fromHashIndex);
	// If <from> doesn't exist in the graph
	// then we say so and do nothing more
	if (fromIndex == -1) {
		String except = "A node with ID <";
		throw  std::exception(except + from + "> doesn't exist in the graph.");
	}
	// We do the same for <to>
	toHashIndex = fnvHash(to) % directedGraph.getAllocatedSize();
	toIndex = getNodeIndex(to, toHashIndex);
	if (toIndex == -1) {
		String except = "A node with ID <";
		throw  std::exception(except + to + "> doesn't exist in the graph.");
	}
}

void DirectedGraph::saveToFile() const {
	// If the file doesn't exist
	// we create it
	String fileName = "DirectedGraphs\\";
	fileName + GraphID + ".txt";
	if (!fileExists(fileName)) {
		createFile(fileName);
	}
	std::ofstream ofile(fileName, std::ios::trunc);
	// If we couldn't oped the file we say so
	if (!ofile.good()) {
		String except = "Couldn't open file <";
		throw std::exception(except + fileName + "> for saving.");
	}
	try {
		// First we write the vertex count
		ofile << vertexCount;
		if (!ofile.good()) {
			String except = "Couldn't write to file <";
			throw std::exception(except + fileName + ">.");
		}
		// Then we write all nodes:
		// For all indexes of the hash map
		for (int i = 0; i < directedGraph.getAllocatedSize(); ++i) {
			// For all nodes mapped to the current index
			for (int j = 0; j < directedGraph[i].getSize(); ++j) {
				// Write the node ID
				ofile << '\n' << directedGraph[i][j].nodeAndEdges.first;
				if (!ofile.good()) {
					String except = "Couldn't write to file <";
					throw std::exception(except + GraphID + ".txt>.");
				}
				// For all edges that start from the current node
				for (int k = 0; k < directedGraph[i][j].nodeAndEdges.second.getSize(); ++k) {
					// Write the edge's ID
					ofile << ' ' << directedGraph[i][j].nodeAndEdges.second[k].first;
					if (!ofile.good()) {
						String except = "Couldn't write to file <";
						throw std::exception(except + GraphID + ".txt>.");
					}
					// And the edge's weight
					ofile << ' ' << directedGraph[i][j].nodeAndEdges.second[k].second;
				}
			}
		}
	}
	catch (std::exception& e) {
		// If we couldn't write to the file
		// then we say so
		std::cerr << e.what() << std::endl;
		// and we remove the file
		removeFile(fileName);
	}
}

// indexBeforeDotTXT is the last index before .txt in fileName (fileName.getLength() - 1 if there is no .txt)
void DirectedGraph::setIDfromFileName(const String& fileName, int indexBeforeDotTXT) {
	// First we search for the last separator of type '\\' or '/'
	int startIndex = 0;
	for (int i = indexBeforeDotTXT; i >= 0; --i) {
		if (fileName[i] == '\\' || fileName[i] == '/') {
			startIndex = i + 1;
			break;
		}
	}
	// And we make GraphID be the string from that separator's index to indexBeforeDotTXT
	GraphID.free();
	for (int j = startIndex; j <= indexBeforeDotTXT; ++j) {
		GraphID.add(fileName[j]);
	}
}

void DirectedGraph::readFromFile(const String& fName) {
	try {
		String fileName = fName;
		// If the file name doesn't end in .txt
		if (!endsInDotTXT(fName)) {
			// We make it end in .txt
			fileName + ".txt";
			// And we get the ID form fName without (.txt)
			setIDfromFileName(fName, fName.getLength() - 1);
		}
		else {
			setIDfromFileName(fName, fName.getLength() - 5);
		}
		if (!fileExists(fileName)) {
			createFile(fileName);
			// We make the hashSize be a nice prime number for low collision rates
			directedGraph.resize(31);
			return;
		}
		std::ifstream ifile(fileName);
		// Get the vertex count
		unsigned vertexCountInFile;
		ifile >> vertexCountInFile;
		if (!ifile.good()) {
			// Reaching eof when the vertex count in the file is 0 is not an exception
			if (ifile.eof()) {
				if (!vertexCount) {
					// We make the hashSize be a nice prime number for low collision rates
					directedGraph.resize(31);
					return;
				}
				else {
					throw std::exception("Improper vertex count in file.");
				}
			}
			String except = "Couldn't read from file <";
			throw std::exception(except + fileName + ">.");
		}
		// Initialize the hash map
		// look for a good formula aka not vertCnt*2+1
		directedGraph.resize(vertexCountInFile * 2 + 1);
		// Initialize all nodes and their edges
		for (unsigned i = 0; i < vertexCountInFile; ++i) {
			NodeAndItsEdges node;
			node.readLine(ifile);
			addNodeAndItsEdges(node);
		}
		// Finally we update all of our nodes' parents
		updateAllNodeParents();
	}
	catch (std::exception&) {
		nullifyVariables();
		throw;
	}
}

void DirectedGraph::addNodeAndItsEdges(const NodeAndItsEdges& newNode) {
	// Hash it and mod it
	unsigned hashIndex = fnvHash(newNode.nodeAndEdges.first) % directedGraph.getAllocatedSize();
	// Insert into graph
	unsigned otherNodesAtIndex = directedGraph[hashIndex].getSize();
	for (unsigned i = 0; i < otherNodesAtIndex; ++i) {
		// If there is a node in the graph with the same ID as the new one
		// then we do nothing
		if (directedGraph[hashIndex][i].nodeAndEdges.first == newNode.nodeAndEdges.first) {
			throw std::exception("There already exists a node with the same ID in the graph.");
		}
	}
	// If the hash table is filled to at around 75% capacity
	if (vertexCount++ >= 3 * (directedGraph.getAllocatedSize() / 4)) {
		// Resize the hash table and rehash all elements
		rehash();
		// Make a new hash because the hashmap has been resized
		unsigned newHashIndex = fnvHash(newNode.nodeAndEdges.first) % directedGraph.getAllocatedSize();
		// And insert the new node into the graph
		directedGraph[newHashIndex].add(newNode);
		return;
	}
	// If the hash table is not filled to around 75% capacity
	// Insert the node at the old hash index
	directedGraph[hashIndex].add(newNode);
}

void DirectedGraph::rehash() {
	// Make a buffer hash map with twice the size of the the previous one + 1
	int newAllocSize = 2 * directedGraph.getAllocatedSize() + 1;
	Vector<Vector<NodeAndItsEdges>> buff(newAllocSize);
	// And copy all of the nodes and edges from the last hash map into it
	// For all of the previous hash map's indexes
	for (int i = 0; i < directedGraph.getAllocatedSize(); ++i) {
		// For all nodes hashed to those indexes
		for (int j = 0; j < directedGraph[i].getSize(); ++j) {
			// Add the current node and its edges to the buffer
			// Hash the nodeID
			unsigned hashIndex = fnvHash(directedGraph[i][j].nodeAndEdges.first) % newAllocSize;
			buff[hashIndex].add(directedGraph[i][j]);
		}
	}
	// Finally we make directedGraph equal the buffer
	directedGraph.resize(newAllocSize);
	for (int j = 0; j < newAllocSize; ++j) {
		directedGraph[j] = buff[j];
	}
	updateAllNodeParents();
}

void DirectedGraph::NodeAndItsEdges::readLine(std::ifstream& ifile) {
	// Reading this node's name
	String buff;
	ifile >> buff;
	if (!ifile.good()) {
		// Reaching the end of the file is not an exception
		if (ifile.eof()) {
			nodeAndEdges.first = buff;
			return;
		}
		String except = "Couldn't read from file.";
		throw std::exception(except);
	}
	nodeAndEdges.first = buff;
	buff.free();
	char c;
	std::pair<String, int> currNeighbourAndWeight;
	while (ifile.get(c)) {
		// If c is ' ' or '\t' then we skip it
		if (c == ' ' || c == '\t') { continue; }
		// Check if we have reached the end of the line
		if (c == '\n') { break; }
		// Insert the current c into the NandW id
		currNeighbourAndWeight.first.add(c);
		// Check if the NandW id is only one char long
		char check;
		ifile.get(check);
		if (!ifile.good()) {
			String except = "Couldn't read from file.";
			throw std::exception(except);
		}
		if (check != ' ' && check != '\t') {
			ifile.unget();
			// Get NandW id from the file
			ifile >> buff;
			if (!ifile.good()) {
				String except = "Couldn't read from file.";
				throw std::exception(except);
			}
			// Insert the rest of the id 
			currNeighbourAndWeight.first + buff;
		}
		// Get NandW weight
		int currWeight;
		ifile >> currWeight;
		if (!ifile.good()) {
			// Reaching the end of the file is not an exception
			if (ifile.eof()) {
				currNeighbourAndWeight.second = currWeight;
				// Add NandW to the edges
				nodeAndEdges.second.add(currNeighbourAndWeight);
				return;
			}
			String except = "Couldn't read from file.";
			throw std::exception(except);
		}
		// Insert the weight
		currNeighbourAndWeight.second = currWeight;
		// Add NandW to the edges
		nodeAndEdges.second.add(currNeighbourAndWeight);
		// Clear the current neighbour's id
		currNeighbourAndWeight.first.free();
	}
}

void DirectedGraph::createFile(const String& fileName) const {
	std::ofstream ofile(fileName);
	if (!ofile.good()) {
		String except = "Couldn't create file <";
		except + fileName + ">.";
		throw std::exception(except);
	}
	try {
		// To make the file valid we say that there are 0 vertices
		ofile << 0;
		if (!ofile.good()) {
			String except = "Couldn't write to file <";
			throw std::exception(except + fileName + "<.");
		}
		ofile.close();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		removeFile(fileName);
	}
}

void DirectedGraph::removeFile(const String& fileName) const {
	if (remove(fileName) != 0) {
		String except = "Couldn't remove file <";
		throw std::exception(except + fileName + ">."); // ;(
	}
}

bool DirectedGraph::fileExists(const String& fileName) const {
	bool fileOpenedSuccessfully;
	std::ifstream ifile(fileName);
	fileOpenedSuccessfully = ifile.good();
	ifile.close();
	return fileOpenedSuccessfully;
}

bool DirectedGraph::endsInDotTXT(const String& str) const {
	int strLength = str.getLength();
	// If the string isn't even 5 chars long then it can't end in .txt
	if (strLength < 5) {
		return false;
	}
	if (str[strLength - 4] != '.') { return false; }
	if (str[strLength - 3] != 't') { return false; }
	if (str[strLength - 2] != 'x') { return false; }
	if (str[strLength - 1] != 't') { return false; }
	return true;
}

void DirectedGraph::nullifyVariables() {
	vertexCount = 0;
	GraphID = "UnknownID";
	directedGraph.clean();
}

void DirectedGraph::cleanGraph() {
	directedGraph.clean();
}

void DirectedGraph::refreshGraph() {
	String fileName = "DirectedGraphs\\";
	readFromFile(fileName + GraphID + ".txt");
}

int DirectedGraph::getNodeIndex(const String& nodeID, unsigned hashCode) const {
	int nodeIndex = -1;
	for (int i = 0; i < directedGraph[hashCode].getSize(); ++i) {
		if (directedGraph[hashCode][i].nodeAndEdges.first == nodeID) {
			nodeIndex = i;
			break;
		}
	}
	// If we return -1 then we haven't found the node in the graph
	return nodeIndex;
}

void DirectedGraph::updateAllNodeParents() {
	// For all of our hashMap's indexes
	for (int i = 0; i < directedGraph.getAllocatedSize(); ++i) {
		// For all nodes hashed to those indexes
		for (int j = 0; j < directedGraph[i].getSize(); ++j) {
			unsigned edgeCount = directedGraph[i][j].nodeAndEdges.second.getSize();
			// For every edge of the current node
			for (unsigned k = 0; k < edgeCount; ++k) {
				// Add the current node as a parent of the node on the other side of the edge
				//// The name of the node on the other side of the edge is 
				//// directedGraph[i][j].nodeAndEdges.second[k].first
				addNodeAsParent(directedGraph[i][j].nodeAndEdges.second[k].first, directedGraph[i][j]);
			}
		}
	}
}

void DirectedGraph::addNodeAsParent(const String& child, NodeAndItsEdges& parent) {
	// Hash it and mod it
	unsigned childHashIndex = fnvHash(child) % directedGraph.getAllocatedSize();
	unsigned otherNodesAtIndex = directedGraph[childHashIndex].getSize();
	for (unsigned i = 0; i < otherNodesAtIndex; ++i) {
		// When the child node is found
		// add the adress of the parent to its parents vector
		if (directedGraph[childHashIndex][i].nodeAndEdges.first == child) {
			directedGraph[childHashIndex][i].parents.add(&parent);
			return;
		}
	}
}

void DirectedGraph::print() const {
	// For all of our hashMap's indexes
	for (int i = 0; i < directedGraph.getAllocatedSize(); ++i) {
		// For all nodes hashed to those indexes
		for (int j = 0; j < directedGraph[i].getSize(); ++j) {
			// Print their ids
			std::cout << directedGraph[i][j].nodeAndEdges.first;
			int edgeCount = directedGraph[i][j].nodeAndEdges.second.getSize();
			// and their edges
			for (int k = 0; k < edgeCount; ++k) {
				std::cout << ' ' << directedGraph[i][j].nodeAndEdges.second[k].first
					<< ' ' << directedGraph[i][j].nodeAndEdges.second[k].second;
			}
			std::cout << std::endl;
		}
	}
}