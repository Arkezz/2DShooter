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
	//Connect player collision handler to drawUI
	connect(&player, SIGNAL(drawUi()), this, SLOT(drawUI()));
	//Connect settings button to settings function
	connect(&player, SIGNAL(openSettings()), this, SLOT(settings()));
	//Connect drawui to enemy attackhandler
	connect(&player, SIGNAL(drawUi()), &enemies[0], SLOT(attackHandler()));

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
		}
	}
	//Add player to the middle of the scene
	player.setPos(400, 200);
	player.setFocus();
	scene->addItem(&player);
	//Add enemy to the middle of the scene
	enemies[0].setPos(400, 80);
	scene->addItem(&enemies[0]);

	for (int i = 0; i < player.getHealth(); i++) {
		QGraphicsPixmapItem* heart = new QGraphicsPixmapItem;
		heart->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
		heart->setPos(0 + (i * tileLen), 0);
		scene->addItem(heart);
		hearts.push_back(heart);
	}
}

//Function that controls the ui
void MainWindow::drawUI() {
	//Set the heart to empty
	hearts[player.getHealth()]->setPixmap(QPixmap(":/ui/emptyHeart").scaled(tileLen, tileLen));
	if (player.getHealth() == 0) {
		scene->removeItem(&player);
		gameOver();
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

//GameOver function opens a new window that has two buttons (restart and quit)
void MainWindow::gameOver() {
	//Create the window
	QWidget* gameOverWindow = new QWidget;
	gameOverWindow->setWindowTitle("Game Over");
	gameOverWindow->setFixedSize(300, 200);
	gameOverWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	gameOverWindow->setWindowModality(Qt::ApplicationModal);
	gameOverWindow->setStyleSheet("background-color: #000000;");
	//Make the screen half transparent
	QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect;
	effect->setOpacity(0.5);
	gameOverWindow->setGraphicsEffect(effect);

	//Create the layout
	QVBoxLayout* layout = new QVBoxLayout;
	gameOverWindow->setLayout(layout);

	//Create the label
	QLabel* gameOverLabel = new QLabel("Game Over");
	gameOverLabel->setStyleSheet("color: #FFFFFF; font-size: 30px;");
	layout->addWidget(gameOverLabel, 0, Qt::AlignCenter);

	//Create the restart button
	QPushButton* restartButton = new QPushButton("Restart");
	restartButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
	layout->addWidget(restartButton, 0, Qt::AlignCenter);
	connect(restartButton, SIGNAL(clicked()), gameOverWindow, SLOT(close()));
	connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));

	//Create the quit button
	QPushButton* quitButton = new QPushButton("Quit");
	quitButton->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 20px;");
	layout->addWidget(quitButton, 0, Qt::AlignCenter);
	connect(quitButton, SIGNAL(clicked()), gameOverWindow, SLOT(close()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

	//Show the window
	gameOverWindow->show();
}

//Restart function
void MainWindow::restart() {
	//Reset the player
	player.setPos(400, 200);
	player.setHealth(3);
	player.setFocus();
	scene->addItem(&player);
	//Reset the enemy
	enemies[0].setPos(400, 80);
	scene->addItem(&enemies[0]);
	//Reset the hearts
	for (int i = 0; i < hearts.size(); i++) {
		hearts[i]->setPixmap(QPixmap(":/ui/fullHeart").scaled(tileLen, tileLen));
	}
}

//Settings function
void MainWindow::settings() {
	//Create the window
	QWidget* settingsWindow = new QWidget;
	settingsWindow->setWindowTitle("Settings");
	settingsWindow->setFixedSize(300, 200);
	settingsWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	settingsWindow->setWindowModality(Qt::ApplicationModal);
	settingsWindow->setStyleSheet("background-color: #000000;");
	setAttribute(Qt::WA_TranslucentBackground);

	//Make the screen show whats behind it

	//Create the layout
	QVBoxLayout* layout = new QVBoxLayout;
	settingsWindow->setLayout(layout);

	//Make the window closable button is round
	QPushButton* closeButton = new QPushButton;
	closeButton->setStyleSheet("background-color: #000000; border-radius: 15px; border: 1px solid #FFFFFF;");
	closeButton->setFixedSize(30, 30);
	closeButton->setIcon(QIcon(":/ui/closeButton"));
	closeButton->setIconSize(QSize(20, 20));
	layout->addWidget(closeButton, 0, Qt::AlignRight);
	connect(closeButton, SIGNAL(clicked()), settingsWindow, SLOT(close()));

	//Create the label
	QLabel* settingsLabel = new QLabel("Settings");
	settingsLabel->setStyleSheet("color: #FFFFFF; font-size: 30px;");
	layout->addWidget(settingsLabel, 0, Qt::AlignCenter);

	//Show the window
	settingsWindow->show();
}