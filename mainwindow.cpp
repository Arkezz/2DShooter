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

	//Grass tile from .qrc file
	QGraphicsPixmapItem* item = scene->addPixmap(QPixmap(":/assets/grass.png"));
	item->setPos(0, 0);
}

MainWindow::~MainWindow()
{
	delete scene;
	delete view;
}

void MainWindow::addBlockItem(int x, int y, QString blockName)
{
	//Create a pixmap item and add it to the scene
	QGraphicsPixmapItem* block = scene->addPixmap(QPixmap(blockName));
	block->setPos(x, y);
}