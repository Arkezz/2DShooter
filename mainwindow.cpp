#include "mainwindow.h"

//Pixel size of each block in the grid
const int blockLen = 32;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//Create the scene object and set the scene to the view
	createScene();
	setCentralWidget(view);
	setWindowTitle("2D Shooter");

	//Connections

	//Call the drawScene function
	drawScene();
}

MainWindow::~MainWindow()
{
	delete scene;
	delete view;
}

//Function that adds tiles to the scene.
void MainWindow::addTile(int x, int y, QString tileName) {
	QGraphicsPixmapItem* newItem = new QGraphicsPixmapItem;
	//Set pixmap from .qrc file and make sure it is scaled
	newItem->setPixmap(QPixmap(":/tiles/" + tileName).scaled(blockLen, blockLen));
	newItem->setPos(x * blockLen, y * blockLen);
	scene->addItem(newItem);
}

//Function that draws the scene
void MainWindow::drawScene() {
	scene->clear();

	//Read the text file level 1 from .qrc
	QFile file(":/maps/level1");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	//Draw the tilemap according to the file contents 1 is grass 2 is wall
	QTextStream in(&file);
	int y = 0;
	while (!in.atEnd()) {
		QString line = in.readLine();
		for (int x = 0; x < line.length(); x++) {
			if (line[x] == '1') {
                addTile(x, y, "grass");
			}
			else if (line[x] == '2') {
                addTile(x, y, "wall");
			}
            else {
			}
		}
		y++;
	}
}

//Creation and setting of scene and view
void MainWindow::createScene() {
	scene = new QGraphicsScene;
	view = new SceneView(this);
	view->setScene(scene);
}

//Size changes
void MainWindow::setSize() {
    this->setFixedSize(960, 720);
	int x = ((screenGeometry.width() - this->width()) / 2);
	int y = ((screenGeometry.height() - this->height()) / 2);
    this->move(x, y);
}
