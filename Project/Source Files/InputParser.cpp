#include "InputParser.h"

bool InputParser::parseAndExecute(const String& input, Graphs& graphs) {
	int wordCount = input.countWords();
	// If the input is empty then we do nothing
	if (!wordCount) {
		return true;
	}
	String firstWord = input.wordNumber(1);
	// If the input starts with "Create"
	if (!strcmp(firstWord, "Create")) {
		if (wordCount == 1) {
			std::cerr << "Unknown command." << std::endl;
			return true;
		}
		String secondWord = input.wordNumber(2);
		// Graph
		if (!strcmp(secondWord, "Graph")) {
			if (wordCount == 2) {
				std::cerr << "Please add a Graph ID." << std::endl;
				return true;
			}
			String GraphID = input.wordNumber(3);
			if (wordCount == 3) {
				graphs.createGraph(GraphID);
				std::cout << "Graph <" << GraphID << "> is created." << std::endl;
				return true;
			}
			if (wordCount == 4) {
				String direction = input.wordNumber(4);
				if (!strcmp(direction, "directed")) {
					graphs.createGraph(GraphID, isDirected);
					std::cout << "Directed graph <" << GraphID << "> is created." << std::endl;
					return true;
				}
				if (!strcmp(direction, "undirected")) {
					graphs.createGraph(GraphID, isUndirected);
					std::cout << "Undirected graph <" << GraphID << "> is created." << std::endl;
					return true;
				}
				std::cerr << "Unknown graph directionality." << std::endl;
				return true;
			}
			std::cerr << "Too many instructions" << std::endl;
			return true;
		}
		// Node
		if (!strcmp(secondWord, "Node")) {
			if (wordCount == 2) {
				std::cerr << "Please add a Node ID." << std::endl;
				return true;
			}
			String NodeID = input.wordNumber(3);
			if (wordCount == 3) {
				graphs.createNode(NodeID);
				std::cout << "Node <" << NodeID << "> is created." << std::endl;
				return true;
			}
			std::cerr << "Too many instructions" << std::endl;
			return true;
		}
		// Edge
		if (!strcmp(secondWord, "Edge")) {
			if (wordCount == 2) {
				std::cerr << "Please add a <from> edge ID and a <to> edge ID." << std::endl;
				return true;
			}
			if (wordCount == 3) {
				std::cerr << "Please add a <to> edge ID." << std::endl;
				return true;
			}
			String from = input.wordNumber(3);
			String to = input.wordNumber(4);
			if (wordCount == 4) {
				graphs.createEdge(from, to);
				return true;
			}
			if (wordCount == 5) {
				// Check if the weight is correct
				String weight = input.wordNumber(5);
				// Look if any of the chars of the weight are not digits
				for (int i = 0; i < weight.getLength(); ++i) {
					if (weight[i] < '0' || weight[i] > '9') {
						std::cerr << "Only digits allowed in weight." << std::endl;
						return true;
					}
				}
				// If the weight is indeed a number
				int numWeight = atoi(weight);
				graphs.createEdge(from, to, numWeight);
				return true;
			}
			std::cerr << "Too many instructions" << std::endl;
			return true;
		}
		std::cerr << "Don't know what to create." << std::endl;
		return true;
	}
	if (!strcmp(firstWord, "Delete")) {
		if (wordCount == 1) {
			std::cerr << "Unknown command." << std::endl;
			return true;
		}
		String secondWord = input.wordNumber(2);
		// Graph
		if (!strcmp(secondWord, "Graph")) {
			if (wordCount == 2) {
				if (graphs.getSize() == 1) {
					String GraphID = graphs.getCurrGraphID();
					graphs.deleteGraph(GraphID);
					std::cout << "Graph <" << GraphID << "> is deleted." << std::endl;
					return true;
				}
				std::cerr << "Please add a Graph ID." << std::endl;
				return true;
			}
			String GraphID = input.wordNumber(3);
			if (wordCount == 3) {
				graphs.deleteGraph(GraphID);
				std::cout << "Graph <" << GraphID << "> is deleted." << std::endl;
				return true;
			}
			std::cerr << "Too many instructions" << std::endl;
			return true;
		}
		// Node
		if (!strcmp(secondWord, "Node")) {
			if (wordCount == 2) {
				std::cerr << "Please add a Node ID." << std::endl;
				return true;
			}
			String NodeID = input.wordNumber(3);
			if (wordCount == 3) {
				graphs.deleteNode(NodeID);
				std::cout << "Node <" << NodeID << "> is deleted." << std::endl;
				return true;
			}
			std::cerr << "Too many instructions" << std::endl;
			return true;
		}
		// Edge
		if (!strcmp(secondWord, "Edge")) {
			if (wordCount == 2) {
				std::cerr << "Please add a <from> edge ID and a <to> edge ID." << std::endl;
				return true;
			}
			if (wordCount == 3) {
				std::cerr << "Please add a <to> edge ID." << std::endl;
				return true;
			}
			String from = input.wordNumber(3);
			String to = input.wordNumber(4);
			if (wordCount == 4) {
				graphs.deleteEdge(from, to);
				std::cout << "Edge from <" << from << "> to <" << to << "> is deleted." << std::endl;
				return true;
			}
			std::cerr << "Too many instructions" << std::endl;
			return true;
		}
		std::cerr << "Don't know what to delete." << std::endl;
		return true;
	}
	if (!strcmp(firstWord, "Search")) {
		if (wordCount == 1) {
			std::cerr << "Please add a <from> edge ID and a <to> edge ID." << std::endl;
			return true;
		}
		if (wordCount == 2) {
			std::cerr << "Please add a <to> edge ID." << std::endl;
			return true;
		}
		String from = input.wordNumber(2);
		String to = input.wordNumber(3);
		if (wordCount == 3) {
			graphs.search(from, to);
			return true;
		}
		if (wordCount == 4) {
			// Check if the algorithm is valid
			String algorithm = input.wordNumber(4);
			Algorithm alg;
			if (!strcmp(algorithm, "dfs")) {
				alg = dfs;
				graphs.search(from, to, alg);
				return true;
			}
			if (!strcmp(algorithm, "bfs")) {
				alg = bfs;
				graphs.search(from, to, alg);
				return true;
			}
			if (!strcmp(algorithm, "dijkstra")) {
				alg = dijkstra;
				graphs.search(from, to, alg);
				return true;
			}
			std::cerr << "Unknown algorithm." << std::endl;
			return true;
		}
		std::cerr << "Too many instructions" << std::endl;
		return true;
	}
	if (!strcmp(firstWord, "Use")) {
		if (wordCount == 1) {
			std::cerr << "Unknown command." << std::endl;
			return true;
		}
		String secondWord = input.wordNumber(2);
		if (wordCount == 2) {
			if (!strcmp(secondWord, "Graph")) {
				std::cerr << "Please add a Graph ID." << std::endl;
				return true;
			}
			else {
				std::cerr << "Don't know what to use." << std::endl;
				return true;
			}
		}
		if (wordCount == 3) {
			String GraphID = input.wordNumber(3);
			graphs.useGraph(GraphID);
			std::cout << "Now using graph <" << GraphID << ">." << std::endl;
			return true;
		}
		std::cerr << "Don't know what to use." << std::endl;
		return true;
	}
	if (wordCount == 1 && !strcmp(firstWord, "Print")) {
		graphs.printUsedGraph();
		return true;
	}
	if (wordCount == 1 && !strcmp(firstWord, "Save")) {
		graphs.saveGraphToFile();
		std::cout << "Graph <" << graphs.getCurrGraphID() << "> saved to file." << std::endl;
		return true;
	}
	if (wordCount == 1 && !strcmp(firstWord, "Quit")) {
		std::cout << "Quitting the program." << std::endl;
		return false;
	}
	std::cerr << "Unknown command." << std::endl;
	return true;
}