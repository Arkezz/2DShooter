#include "mainwindow.h"

//Pixel size of each Tile in the grid
const int tileLen = 32;

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
	newItem->setPixmap(QPixmap(":/tiles/" + tileName).scaled(tileLen, tileLen));
	newItem->setPos(x * tileLen, y * tileLen);
	scene->addItem(newItem);
}

//Function that draws the scene
void MainWindow::drawScene() {
	scene->clear();

	//Read the tmx file then go line by line in the csv
	QFile file(":/maps/map1");
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
	QString line = in.readLine();
	while (!line.isNull()) {
		if (line.contains("<data encoding=\"csv\">")) {
			line = in.readLine();
			int y = 0;
			while (!line.isNull()) {
				QStringList list = line.split(",");
				for (int x = 0; x < list.size(); x++) {
					//If the tile is not empty, add it to the scene
					if (list[x] == "0") {
						addTile(x, y, "grass");
					}
					else if (list[x] == "1") {
						addTile(x, y, "wall");
					}
				}
				y++;
				line = in.readLine();
			}
		}
		line = in.readLine();
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
