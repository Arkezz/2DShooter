#include "mainwindow.h"

//Pixel size of each block in the grid
const int blockLen = 48;

//Create a tilemap that is used by drawscene to determine what to draw
//The tilemap is a 2D array of integers that represent the type of tile
//that should be drawn at that location
//Tile map should be as big as the screen

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//Create the scene
	createScene();
	setCentralWidget(view);
	setWindowTitle("2D Shooter");

	drawScene();
}

MainWindow::~MainWindow()
{
	delete scene;
	delete view;
}

//Function that adds blocks to the scene.
void MainWindow::addBlockItem(int x, int y, QString blockName) {
	QGraphicsPixmapItem* newItem = new QGraphicsPixmapItem;
	newItem->setPixmap(QPixmap(":/images/" + blockName + ".png"));
	newItem->setPos(x, y);
	scene->addItem(newItem);
}

//Function that draws the scene
void MainWindow::drawScene() {
	scene->clear();
	//add a grass block use screengeomtry
}

//Creation and setting of scene and view
void MainWindow::createScene() {
	scene = new QGraphicsScene;
	view = new SceneView(this);
	view->setScene(scene);
}

//Size changes
void MainWindow::setSize() {
	this->setFixedSize(800, 600);
	int x = ((screenGeometry.width() - this->width()) / 2);
	int y = ((screenGeometry.height() - this->height()) / 2);
	this->move(x, y);
}