#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	scene = new QGraphicsScene;
	view = new SceneView(this);
	view->setScene(scene);
	view->show();

	//Create a scene
	scene->setSceneRect(0, 0, 800, 600);
	view->setMinimumSize(400, 400);
	setCentralWidget(view);
	//Modify the title
	setWindowTitle("2D Shooter");

	//add a grass block to the scene
	addBlockItem(0, 0, "grass");
    drawScene();
}

MainWindow::~MainWindow()
{
	delete scene;
	delete view;
}

//Function that adds blocks to the scene. Block
void MainWindow::addBlockItem(int x, int y, QString blockName) {
	QGraphicsPixmapItem* newItem = new QGraphicsPixmapItem;
	newItem->setPixmap(QPixmap(":/images/" + blockName + ".png"));
	newItem->setPos(x, y);
	scene->addItem(newItem);
}

//Function that draws the scene
void MainWindow::drawScene() {
	scene->clear();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			addBlockItem(i * 32, j * 32, "grass");
		}
	}
}
