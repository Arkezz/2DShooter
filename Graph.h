#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <QVector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <iterator>

extern int grid[15][20];

//Node class x,y, and bool walkable
class Node {
public:
	int x;
	int y;
	bool walkable;
	Node* parent;
	int gCost;
	int hCost;
	int fCost;

	Node(int x, int y, bool walkable) {
		this->x = x;
		this->y = y;
		this->walkable = walkable;
	}

	Node(int x, int y, bool walkable, Node* parent) {
		this->x = x;
		this->y = y;
		this->walkable = walkable;
		this->parent = parent;
	}
};

class Graph {
private:
	const int width = 20;
	const int height = 15;
public:
	//Create the graph according to the map
	Graph() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (grid[j][i] < 15) {
					nodes[i][j] = new Node(i, j, true);
				}
				else {
					nodes[i][j] = new Node(i, j, false);
				}
			}
		}
	}
	Node* nodes[20][15];

	void createGraph() {
		//qdebug every node in a grid style to see if it is walkable or not
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				std::cout << nodes[i][j]->walkable << " ";
			}
			std::cout << std::endl;
		}
	}

	//getNode returns the node at the x,y
	Node* getNode(int x, int y) {
		// Check if the x and y values are within the bounds of the nodes array
		if (x < 0 || x >= width || y < 0 || y >= height) {
			// If not, return nullptr
			return nullptr;
		}

		return nodes[x][y];
	}

	//getNeighbors returns a vector of the neighbors of the node

	//getDistance returns the distance between two nodes
	int getDistance(Node* nodeA, Node* nodeB) {
		int dstX = abs(nodeA->x - nodeB->x);
		int dstY = abs(nodeA->y - nodeB->y);
		if (dstX > dstY) {
			return 14 * dstY + 10 * (dstX - dstY);
		}
		return 14 * dstX + 10 * (dstY - dstX);
	}

	int manhattanDistance(Node* a, Node* b) {
		return abs(a->x - b->x) + abs(a->y - b->y);
	}

	//getNeighbors returns a vector of the neighbors of the node
	QVector<Node*> getNeighbors(Node* node) {
		QVector<Node*> neighbors;
		int x = node->x;
		int y = node->y;

		// Check all 8 adjacent nodes and add them to the list of neighbors if they are walkable
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) continue; // Skip the current node
				Node* neighbor = getNode(x + i, y + j);
				if (neighbor && neighbor->walkable) {
					neighbors.push_back(neighbor);
				}
			}
		}

		return neighbors;
	}

	//Find shortest path from start to end returns a vector of nodes use Djikstra
	QVector<Node*> findPath(Node* start, Node* end) {
		// Store the original walkable value of the starting and ending nodes
		bool startNodeWalkable = start->walkable;
		bool endNodeWalkable = end->walkable;

		// Temporarily set the starting and ending nodes to be walkable
		start->walkable = true;
		end->walkable = true;

		//THe path consists of all the nodes the enemy would need to go through he can only go through walkable nodes
		QVector<Node*> path;
		//The open set is the set of nodes that have not been checked yet
		QVector<Node*> openSet;
		//The closed set is the set of nodes that have been checked
		QVector<Node*> closedSet;
		//Add the start node to the open set
		openSet.push_back(start);
		//While the open set is not empty
		while (!openSet.empty()) {
			//Set the current node to the first node in the open set
			Node* currentNode = openSet[0];
			//For each node in the open set
			for (int i = 1; i < openSet.size(); i++) {
				//If the fCost of the current node is greater than the fCost of the node in the open set
				if (openSet[i]->fCost < currentNode->fCost || (openSet[i]->fCost == currentNode->fCost && openSet[i]->hCost < currentNode->hCost)) {
					//Set the current node to the node in the open set
					currentNode = openSet[i];
				}
			}
			//Remove the current node from the open set
			openSet.removeOne(currentNode);
			//Add the current node to the closed set
			closedSet.push_back(currentNode);
			//If the current node is the end node
			if (currentNode == end) {
				//Create a temporary node
				Node* temp = currentNode;
				//While the temporary node is not the start node
				while (temp != start) {
					//Add the temporary node to the path
					path.push_back(temp);
					//Set the temporary node to the parent of the temporary node
					temp = temp->parent;
				}
				//Reverse the path
				std::reverse(path.begin(), path.end());
				//Return the path
				return path;
			}
			//Get the neighbors of the current node
			QVector<Node*> neighbors = getNeighbors(currentNode);
			//For each neighbor of the current node
			for (int i = 0; i < neighbors.size(); i++) {
				//If the neighbor is not walkable or the neighbor is in the closed set
				if (!neighbors[i]->walkable || closedSet.contains(neighbors[i])) {
					//Continue to the next neighbor
					continue;
				}
				//Get the distance from the current node to the neighbor
				int newMovementCostToNeighbor = currentNode->gCost + getDistance(currentNode, neighbors[i]);
				//If the new movement cost to the neighbor is less than the gCost of the neighbor or the neighbor is not in the open set
				if (newMovementCostToNeighbor < neighbors[i]->gCost || !openSet.contains(neighbors[i])) {
					//Set the gCost of the neighbor to the new movement cost to the neighbor
					neighbors[i]->gCost = newMovementCostToNeighbor;
					//Set the hCost of the neighbor to the distance from the neighbor to the end node
					neighbors[i]->hCost = getDistance(neighbors[i], end);
					//Set the parent of the neighbor to the current node
					neighbors[i]->parent = currentNode;
					//If the neighbor is not in the open set
					if (!openSet.contains(neighbors[i])) {
						//Add the neighbor to the open set
						openSet.push_back(neighbors[i]);
					}
				}
			}
		}
		//Return an empty path
		start->walkable = startNodeWalkable;
		end->walkable = endNodeWalkable;
		return path;
	}

	QVector<Node*> findPath2(Node* start, Node* end) {
		// Create an empty list to store the path
		QVector<Node*> path;

		// Create an empty set to store the visited nodes
		std::unordered_set<Node*> visited;

		// Create a priority queue to store the nodes to be visited
		// The nodes will be sorted by their fCost (the total cost of traveling to the node)
		auto cmp = [](Node* a, Node* b) { return a->fCost > b->fCost; };
		std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> frontier(cmp);

		// Set the starting node's gCost (the cost of traveling to the node) to 0
		start->gCost = 0;

		// Set the starting node's hCost (the estimated cost of traveling from the node to the end) to the manhattan distance between the start and end nodes
		start->hCost = manhattanDistance(start, end);

		// Set the starting node's fCost (the total cost of traveling to the node) to its gCost and hCost
		start->fCost = start->gCost + start->hCost;

		// Add the starting node to the frontier
		frontier.push(start);

		// Loop until the frontier is empty
		while (!frontier.empty()) {
			// Get the node with the lowest fCost from the frontier
			Node* current = frontier.top();
			frontier.pop();

			// If the current node is the end node, we have found a path
			if (current == end) {
				// Trace the path back from the end node to the start node
				while (current != start) {
					// Add the current node to the path
					path.push_back(current);

					// Set the current node to its parent
					current = current->parent;
				}

				// Add the start node to the path
				path.push_back(start);

				// Reverse the path to get the correct order
				std::reverse(path.begin(), path.end());

				// Return the path
				return path;
			}

			// Add the current node to the visited set
			visited.insert(current);

			// Get the current node's neighbors
			QVector<Node*> neighbors = getNeighbors(current);

			std::vector<Node*> nodes;

			// Loop through the current node's neighbors
			for (Node* neighbor : neighbors) {
				// Skip the neighbor if it is not walkable or if it has already been visited
				if (!neighbor->walkable || visited.count(neighbor) > 0) continue;

				// Calculate the cost of traveling to the neighbor
				int cost = current->gCost + getDistance(current, neighbor);

				// If the neighbor has not been visited, or if the cost of traveling to the neighbor is lower than its current gCost, update the neighbor's gCost, hCost, and fCost, and set its parent to the current node
				if (std::find(nodes.begin(), nodes.end(), neighbor) == nodes.end() || cost < neighbor->gCost) {
					neighbor->gCost = cost;
					neighbor->hCost = manhattanDistance(neighbor, end);
					neighbor->fCost = neighbor->gCost + neighbor->hCost;
					neighbor->parent = current;

					// Add the neighbor to the frontier if it has not been visited yet
					if (std::find(nodes.begin(), nodes.end(), neighbor) == nodes.end()) {
						frontier.push(neighbor);
						nodes.push_back(neighbor);
					}
				}
			}
		}
	}
};

#endif // GRAPH_H
