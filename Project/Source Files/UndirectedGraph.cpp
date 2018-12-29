#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph() :vertexCount(0), GraphID("UnknownGraph"), undirectedGraph() {}

UndirectedGraph::UndirectedGraph(const String& fileName) {
	nullifyVariables();
	readFromFile(fileName);
}

UndirectedGraph::~UndirectedGraph() {
	nullifyVariables();
}

UndirectedGraph::NodeAndItsEdges::NodeAndItsEdges(const NodeAndItsEdges& other) :nodeAndEdges(other.nodeAndEdges) {}

UndirectedGraph::NodeAndItsEdges& UndirectedGraph::NodeAndItsEdges::operator=(const NodeAndItsEdges& other) {
	if (this != &other) {
		nodeAndEdges = other.nodeAndEdges;
	}
	return *this;
}

void UndirectedGraph::addNode(const String& nodeID) {
	// Hash the nodeID
	unsigned hashIndex = fnvHash(nodeID) % undirectedGraph.getAllocatedSize();
	// Search for it in the graph
	for (int i = 0; i < undirectedGraph[hashIndex].getSize(); ++i) {
		// If we find a node with the same id in the graph
		// then we say so and do nothing more
		if (undirectedGraph[hashIndex][i].nodeAndEdges.first == nodeID) {
			String except = "A node with ID <";
			throw  std::exception(except + nodeID + "> already exists in the graph.");
		}
	}
	// If the nodeID in new then we add a node with that ID to the graph
	undirectedGraph[hashIndex].add(nodeID);
	++vertexCount;
}

void UndirectedGraph::removeNode(const String& nodeID) {
	// Hash the nodeID
	unsigned hashIndex = fnvHash(nodeID) % undirectedGraph.getAllocatedSize();
	// Search for it in the graph
	int nodeIndex = getNodeIndex(nodeID, hashIndex);
	// If we don't find a node with that ID in the graph
	// then we say so and do nothing more
	if (nodeIndex == -1) {
		String except = "A node with ID <";
		throw  std::exception(except + nodeID + "> doesn't exist in the graph.");
	}
	// If there is a node with that id in the graph
	// then we remove all of the edges connected to it
	// For all nodes adjacent to our node
	int adjacentNodes = undirectedGraph[hashIndex][nodeIndex].nodeAndEdges.second.getSize();
	for (int i = 0; i < adjacentNodes; ++i) {
		// Hash the nodeID
		unsigned currNodeHash =
			fnvHash(undirectedGraph[hashIndex][nodeIndex].nodeAndEdges.second[i].first)
			% undirectedGraph.getAllocatedSize();
		// Get which one of the nodes hashed to currNodeHash is the current one
		int currNodeIndex =
			getNodeIndex(undirectedGraph[hashIndex][nodeIndex].nodeAndEdges.second[i].first,
				currNodeHash);
		// Search tor the edge: <currNode> <nodeID>
		int currEdgeCount = undirectedGraph[currNodeHash][currNodeIndex].nodeAndEdges.second.getSize();
		for (int j = 0; j < currEdgeCount; ++j) {
			// When the edge is found
			if (undirectedGraph[currNodeHash][currNodeIndex].nodeAndEdges.second[j].first
				== nodeID) {
				// remove it
				undirectedGraph[currNodeHash][currNodeIndex].nodeAndEdges.second.remove(j);
				break;
			}
		}
	}
	// and then we remove it
	undirectedGraph[hashIndex].remove(nodeIndex);
	--vertexCount;
}

void UndirectedGraph::addEdge(const String& from, const String& to, int weight) {
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
		// between <from> and <to>
		std::pair<String, int> fromToEdge(to, weight);
		undirectedGraph[fromHashIndex][fromIndex].nodeAndEdges.second.add(fromToEdge);
		// between <to> and <from>
		if (from != to) {
			std::pair<String, int> toFromEdge(from, weight);
			undirectedGraph[toHashIndex][toIndex].nodeAndEdges.second.add(toFromEdge);
		}
		std::cout << "Edge from <" << from << "> to <" << to << "> is created." << std::endl;
	}
	else {
		// If both the nodes and the edge exist then we update the weight
		undirectedGraph[fromHashIndex][fromIndex].nodeAndEdges.second[isIn].second = weight;
		// The graph is undirected so edgeExists(...) will always return a valid index
		undirectedGraph[toHashIndex][toIndex].nodeAndEdges.second[edgeExists(from, toHashIndex, toIndex)].second = weight;
		std::cout << "Edge from <" << from << "> to <" << to << "> is updated with weight " << weight << "." << std::endl;
	}
}

void UndirectedGraph::removeEdge(const String& from, const String& to) {
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
	// If both the nodes and the edge exist then we remove the edge between them
	// between <from> and <to>
	undirectedGraph[fromHashIndex][fromIndex].nodeAndEdges.second.remove(isIn);
	// between <to> and <from>
	undirectedGraph[toHashIndex][toIndex].nodeAndEdges.second.remove(edgeExists(from, toHashIndex, toIndex));
}


void UndirectedGraph::search(const String& from, const String& to, const Algorithm algorithm) const {
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
	if (algorithm == bfs) {
		searchDijkstra(from, to,
			fromHashIndex, fromIndex);
	}
}

void UndirectedGraph::searchDfs(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	if (from == to) {
		std::cout << from << std::endl;
		return;
	}
	// Here we store the visited nodes
	VisitedNodesDfs visited(undirectedGraph.getAllocatedSize());
	visited.makeVisited(from);
	Vector<NodeAndItsEdges> specialStack;
	specialStack.add(undirectedGraph[fromHashIndex][fromIndex]);
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
				int hashIndex = fnvHash(temp.nodeAndEdges.second[i].first) % undirectedGraph.getAllocatedSize();
				int actualIndex = getNodeIndex(temp.nodeAndEdges.second[i].first, hashIndex);
				// and we start dfs with it again
				specialStack.add(undirectedGraph[hashIndex][actualIndex]);
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

void UndirectedGraph::searchBfs(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	if (from == to) {
		std::cout << from << std::endl;
		return;
	}
	// Here we store the visited nodes
	VisitedNodesBfs visited(undirectedGraph.getAllocatedSize());
	std::queue<NodeAndItsEdges> queue;
	queue.push(undirectedGraph[fromHashIndex][fromIndex]);
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
				int hashIndex = fnvHash(temp.nodeAndEdges.second[i].first) % undirectedGraph.getAllocatedSize();
				int actualIndex = getNodeIndex(temp.nodeAndEdges.second[i].first, hashIndex);
				// push it to the queue
				queue.push(undirectedGraph[hashIndex][actualIndex]);
				// and mark it as visited (and make temp its parent)
				visited.makeVisited(undirectedGraph[hashIndex][actualIndex].nodeAndEdges.first,
					temp.nodeAndEdges.first);
			}
		}
		// Finally pop temp from the queue
		queue.pop();
	}
	std::cerr << "No path from <" << from << "> to <" << to << "> exists." << std::endl;
}

void UndirectedGraph::searchDijkstra(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	if (from == to) {
		std::cout << from << std::endl
			<< 0 << std::endl;
		return;
	}
	VisitedNodesDijkstra visited(undirectedGraph.getAllocatedSize());
	visited.makeVisited(from, "NoParent", 0);
	std::priority_queue<NodeAndItsEdges> queue;
	queue.push(undirectedGraph[fromHashIndex][fromIndex]);
	while (!queue.empty()) {
		NodeAndItsEdges temp = queue.top();
		queue.pop();
		// We search through all of temps edges
		for (int i = 0; i < temp.nodeAndEdges.second.getSize(); ++i) {
			// Find the i-th neighbour
			int hashIndex = fnvHash(temp.nodeAndEdges.second[i].first) % undirectedGraph.getAllocatedSize();
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
				queue.push(undirectedGraph[hashIndex][actualIndex]);
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

int UndirectedGraph::edgeExists(const String& to,
	unsigned& fromHashIndex, int& fromIndex) const {
	// For every edge starting from <from>
	int edgeCount = undirectedGraph[fromHashIndex][fromIndex].nodeAndEdges.second.getSize();
	for (int i = 0; i < edgeCount; ++i) {
		// If the edge is found
		// then its index is returned
		if (undirectedGraph[fromHashIndex][fromIndex].nodeAndEdges.second[i].first == to) {
			return i;
		}
	}
	// If the edge is not found
	return -1;
}

void UndirectedGraph::nodesExistCheck(const String& from, const String& to,
	unsigned& fromHashIndex, int& fromIndex,
unsigned& toHashIndex, int& toIndex) const {
	// Hash the <from> nodeID
	fromHashIndex = fnvHash(from) % undirectedGraph.getAllocatedSize();
	// Search for it in the graph
	fromIndex = getNodeIndex(from, fromHashIndex);
	// If <from> doesn't exist in the graph
	// then we say so and do nothing more
	if (fromIndex == -1) {
		String except = "A node with ID <";
		throw  std::exception(except + from + "> doesn't exist in the graph.");
	}
	// We do the same for <to>
	toHashIndex = fnvHash(to) % undirectedGraph.getAllocatedSize();
	toIndex = getNodeIndex(to, toHashIndex);
	if (toIndex == -1) {
		String except = "A node with ID <";
		throw  std::exception(except + to + "> doesn't exist in the graph.");
	}
}

void UndirectedGraph::saveToFile() const {
	// If the file doesn't exist
	// we create it
	String fileName = "UndirectedGraphs\\";
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
		for (int i = 0; i < undirectedGraph.getAllocatedSize(); ++i) {
			// For all nodes mapped to the current index
			for (int j = 0; j < undirectedGraph[i].getSize(); ++j) {
				// Write the node ID
				ofile << '\n' << undirectedGraph[i][j].nodeAndEdges.first;
				if (!ofile.good()) {
					String except = "Couldn't write to file <";
					throw std::exception(except + GraphID + ".txt>.");
				}
				// For all edges that start from the current node
				for (int k = 0; k < undirectedGraph[i][j].nodeAndEdges.second.getSize(); ++k) {
					// Write the edge's ID
					ofile << ' ' << undirectedGraph[i][j].nodeAndEdges.second[k].first;
					if (!ofile.good()) {
						String except = "Couldn't write to file <";
						throw std::exception(except + GraphID + ".txt>.");
					}
					// And the edge's weight
					ofile << ' ' << undirectedGraph[i][j].nodeAndEdges.second[k].second;
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
void UndirectedGraph::setIDfromFileName(const String& fileName, int indexBeforeDotTXT) {
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

void UndirectedGraph::readFromFile(const String& fName) {
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
			undirectedGraph.resize(31);
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
					undirectedGraph.resize(31);
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
		undirectedGraph.resize(vertexCountInFile * 2 + 1);
		// Initialize all nodes and their edges
		for (unsigned i = 0; i < vertexCountInFile; ++i) {
			NodeAndItsEdges node;
			node.readLine(ifile);
			addNodeAndItsEdges(node);
		}
	}
	catch (std::exception&) {
		nullifyVariables();
		throw;
	}
}

void UndirectedGraph::addNodeAndItsEdges(const NodeAndItsEdges& newNode) {
	// Hash it and mod it
	unsigned hashIndex = fnvHash(newNode.nodeAndEdges.first) % undirectedGraph.getAllocatedSize();
	// Insert into graph
	unsigned otherNodesAtIndex = undirectedGraph[hashIndex].getSize();
	for (unsigned i = 0; i < otherNodesAtIndex; ++i) {
		// If there is a node in the graph with the same ID as the new one
		// then we do nothing
		if (undirectedGraph[hashIndex][i].nodeAndEdges.first == newNode.nodeAndEdges.first) {
			throw std::exception("There already exists a node with the same ID in the graph.");
		}
	}
	// If the hash table is filled to at around 75% capacity
	if (vertexCount++ >= 3 * (undirectedGraph.getAllocatedSize() / 4)) {
		// Resize the hash table and rehash all elements
		rehash();
		// Make a new hash because the hashmap has been resized
		unsigned newHashIndex = fnvHash(newNode.nodeAndEdges.first) % undirectedGraph.getAllocatedSize();
		// And insert the new node into the graph
		undirectedGraph[newHashIndex].add(newNode);
		return;
	}
	// If the hash table is not filled to around 75% capacity
	// Insert the node at the old hash index
	undirectedGraph[hashIndex].add(newNode);
}

void UndirectedGraph::rehash() {
	// Make a buffer hash map with twice the size of the the previous one + 1
	int newAllocSize = 2 * undirectedGraph.getAllocatedSize() + 1;
	Vector<Vector<NodeAndItsEdges>> buff(newAllocSize);
	// And copy all of the nodes and edges from the last hash map into it
	// For all of the previous hash map's indexes
	for (int i = 0; i < undirectedGraph.getAllocatedSize(); ++i) {
		// For all nodes hashed to those indexes
		for (int j = 0; j < undirectedGraph[i].getSize(); ++j) {
			// Add the current node and its edges to the buffer
			// Hash the nodeID
			unsigned hashIndex = fnvHash(undirectedGraph[i][j].nodeAndEdges.first) % newAllocSize;
			buff[hashIndex].add(undirectedGraph[i][j]);
		}
	}
	// Finally we make undirectedGraph equal the buffer
	undirectedGraph.resize(newAllocSize);
	for (int j = 0; j < newAllocSize; ++j) {
		undirectedGraph[j] = buff[j];
	}
}

void UndirectedGraph::NodeAndItsEdges::readLine(std::ifstream& ifile) {
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

void UndirectedGraph::createFile(const String& fileName) const {
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
			throw std::exception(except + fileName + ">.");
		}
		ofile.close();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		removeFile(fileName);
	}
}

void UndirectedGraph::removeFile(const String& fileName) const {
	if (remove(fileName) != 0) {
		String except = "Couldn't remove file <";
		throw std::exception(except + fileName + ">."); // ;(
	}
}

bool UndirectedGraph::fileExists(const String& fileName) const {
	bool fileOpenedSuccessfully;
	std::ifstream ifile(fileName);
	fileOpenedSuccessfully = ifile.good();
	ifile.close();
	return fileOpenedSuccessfully;
}

bool UndirectedGraph::endsInDotTXT(const String& str) const {
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

void UndirectedGraph::nullifyVariables() {
	vertexCount = 0;
	GraphID = "UnknownID";
	undirectedGraph.clean();
}

void UndirectedGraph::cleanGraph() {
	undirectedGraph.clean();
}

void UndirectedGraph::refreshGraph() {
	String fileName = "UndirectedGraphs\\";
	readFromFile(fileName + GraphID + ".txt");
}

int UndirectedGraph::getNodeIndex(const String& nodeID, unsigned hashCode) const {
	int nodeIndex = -1;
	for (int i = 0; i < undirectedGraph[hashCode].getSize(); ++i) {
		if (undirectedGraph[hashCode][i].nodeAndEdges.first == nodeID) {
			nodeIndex = i;
			break;
		}
	}
	// If we return -1 then we haven't found the node in the graph
	return nodeIndex;
}

void UndirectedGraph::print() const {
	// For all of our hashMap's indexes
	for (int i = 0; i < undirectedGraph.getAllocatedSize(); ++i) {
		// For all nodes hashed to those indexes
		for (int j = 0; j < undirectedGraph[i].getSize(); ++j) {
			// Print their ids
			std::cout << undirectedGraph[i][j].nodeAndEdges.first;
			int edgeCount = undirectedGraph[i][j].nodeAndEdges.second.getSize();
			// and their edges
			for (int k = 0; k < edgeCount; ++k) {
				std::cout << ' ' << undirectedGraph[i][j].nodeAndEdges.second[k].first
					<< ' ' << undirectedGraph[i][j].nodeAndEdges.second[k].second;
			}
			std::cout << std::endl;
		}
	}
}