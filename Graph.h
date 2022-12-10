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

	// QVector is a container class that provides similar functionality to std::vector
	QVector<Node*> constructPath(Node* end) {
		// Create a vector to store the path
		QVector<Node*> path;

		// Create a pointer to the current node
		Node* current = end;

		// Add the current node to the path
		path.push_back(current);

		// Follow the chain of parent nodes back to the start node
		// and add each node to the path in reverse order
		while (current->parent != nullptr) {
			current = current->parent;
			path.push_back(current);
		}

		// Reverse the path to put it in the correct order
		// and return it
		std::reverse(path.begin(), path.end());
		return path;
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
		// Create a set to store visited nodes
		std::unordered_set<Node*> visited;

		// Create a priority queue to store the nodes to be visited,
		// ordered by their fCost (estimated total cost from start to end)
		auto cmp = [](Node* left, Node* right) {
			return left->fCost > right->fCost;
		};
		std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> toVisit(cmp);

		// Set the start node's gCost (actual cost from start) to 0
		// and its hCost (estimated cost to end) to the Manhattan distance
		// between the start and end nodes
		start->gCost = 0;
		start->hCost = manhattanDistance(start, end);
		start->fCost = start->gCost + start->hCost;

		// Add the start node to the queue
		toVisit.push(start);

		int maxVisitedNodes = 1000; // or some other suitable threshold
		int numVisitedNodes = 0;
		// Keep searching until the queue is empty
		while (!toVisit.empty() && numVisitedNodes < maxVisitedNodes) {
			// Get the node with the lowest fCost from the queue
			Node* current = toVisit.top();
			toVisit.pop();

			// If we have reached the end node, construct and return the path
			if (current == end) {
				return constructPath(current);
			}

			// Mark the current node as visited
			visited.insert(current);
			numVisitedNodes++;

			// Get the current node's neighbors
			QVector<Node*> neighbors = getNeighbors(current);
			for (Node* neighbor : neighbors) {
				// Skip visited nodes
				if (visited.count(neighbor) > 0) {
					continue;
				}

				// Calculate the neighbor's gCost, hCost, and fCost
				int gCost = current->gCost + 1; // assume a cost of 1 to move to any neighbor
				int hCost = manhattanDistance(neighbor, end);
				int fCost = gCost + hCost;

				// If the neighbor's fCost is lower than its current fCost,
				// or if it is not in the queue, update its parent, gCost, hCost, and fCost
				if (fCost < neighbor->fCost || visited.count(neighbor) == 0) {
					neighbor->parent = current;
					neighbor->gCost = gCost;
					neighbor->hCost = hCost;
					neighbor->fCost = fCost;

					// Add the neighbor to the queue if it is not already in it
					if (visited.count(neighbor) == 0) {
						toVisit.push(neighbor);
					}
				}
			}
		}

		// If we reach here, no path was found
		return QVector<Node*>();
	}
};

#endif // GRAPH_H
