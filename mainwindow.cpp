#include "mainwindow.h"

//Pixel size of each Tile in the grid
const int tileLen = 32;
//Data structure that stores the location of each tile in the grid
//0 = empty, 1 = wall, 2 = player, 3 = enemy
int grid[15][20];

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//Create the scene object and set the scene to the view
	createScene();
	setCentralWidget(view);
	setWindowTitle("2D Shooter");

	//Connections
	//Connect a timer to the idleanimation of player

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
					grid[y][x] = list[x].toInt();
				}
				y++;
				line = in.readLine();
			}
		}
		line = in.readLine();
	}

	// Draw the scene according to the grid
	for (int y = 0; y < 15; y++) {
		for (int x = 0; x < 20; x++) {
			if (grid[y][x] == 1) {
				addTile(x, y, "wall");
			}
			else if (grid[y][x] == 0) {
				addTile(x, y, "grass");
			}
			else if (grid[y][x] == 3) {
				addTile(x, y, "enemy.png");
			}
		}
	}
	//Add player to the middle of the scene
	player.setPos(80, 80);
	player.setFocus();
	scene->addItem(&player);
	//Add enemy to the middle of the scene
	enemies[1].setPos(400, 80);
	scene->addItem(&enemies[1]);
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
