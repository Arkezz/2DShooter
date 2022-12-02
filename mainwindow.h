#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDialog>
#include <QSlider>
#include <QApplication>
#include <QScreen>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QTimer>
#include "sceneview.h"
#include "player.h"
#include "soundmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
public:
	//Create the graph according to the map
	Graph(int map[15][20]) {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (map[i][j] < 15) {
					nodes[i][j] = new Node(i, j, true);
				}
				else {
					nodes[i][j] = new Node(i, j, false);
				}
			}
		}
	}
	Node* nodes[20][20];

	void createGraph() {
		//Qdebug each row of the graph
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				qDebug() << nodes[i][j]->walkable;
			}
			qDebug() << "\n";
		}
	}

	//getNode returns the node at the x,y
	Node* getNode(int x, int y) {
		return nodes[x][y];
	}

	//getNeighbors returns a vector of the neighbors of the node
	QVector<Node*> getNeighbors(Node* node) {
		QVector<Node*> neighbors;
		int x = node->x;
		int y = node->y;
		if (x > 0) {
			neighbors.push_back(nodes[x - 1][y]);
		}
		if (x < 19) {
			neighbors.push_back(nodes[x + 1][y]);
		}
		if (y > 0) {
			neighbors.push_back(nodes[x][y - 1]);
		}
		if (y < 19) {
			neighbors.push_back(nodes[x][y + 1]);
		}
		return neighbors;
	}

	//getDistance returns the distance between two nodes
	int getDistance(Node* nodeA, Node* nodeB) {
		int dstX = abs(nodeA->x - nodeB->x);
		int dstY = abs(nodeA->y - nodeB->y);
		if (dstX > dstY) {
			return 14 * dstY + 10 * (dstX - dstY);
		}
		return 14 * dstX + 10 * (dstY - dstX);
	}

	//Find shortest path from start to end returns a vector of nodes use Djikstra
	QVector<Node*> findPath(Node* start, Node* end) {
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
		return path;
	}
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void drawScene();
	void setSize();

signals:
	void enemyAttack();

public slots:
	void drawUI();
	void restart();
	void settings();
	void collisionHandler();
	void enemyCollisionHandler();
	void fullScreen();
	void pathFinding(Enemy& enemy);

private:
	//Only allow one instance of the settings
	QDialog* settingsWindow = nullptr;
	QGraphicsScene* scene;
	SceneView* view;
	QRect screenGeometry = QApplication::primaryScreen()->geometry();
	void gameOver();
	void win();
	void createScene();
	void addTile(int x, int y, QString blockName);
	Player player;
	Enemy enemies[2];
	QVector<QGraphicsPixmapItem*> hearts;
	Collectibles* object;
	soundManager music;
	QVector<QGraphicsEllipseItem*> footsteps;
	QTimer* invincTimer;
	QTimer* enemyTimer;
	QTimer* tempTimer = new QTimer;
	QGraphicsTextItem* ammo;
	QGraphicsTextItem* statusText;
	Graph* graph;
	int level = 1;
};

#endif // MAINWINDOW_H
