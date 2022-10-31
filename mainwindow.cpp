#include "mainwindow.h"

//Pixel size of each block in the grid
const int blockLen = 48;

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
    //Draw the borders of the scene (top, bottom, left, right)
    for (int i = 0; i < 800; i += blockLen) {
        addBlockItem(i, 0, "grass");
        addBlockItem(i, 600 - blockLen, "grass");
    }
    for (int i = 0; i < 600; i += blockLen) {
        addBlockItem(0, i, "grass");
        addBlockItem(800 - blockLen, i, "grass");
    }
    //Create a maze like structure in the middle of the scene
    for (int i = 0; i < 600; i += blockLen) {
        for (int j = 0; j < 800; j += blockLen) {
            if (i == 0 || i == 600 - blockLen || j == 0 || j == 800 - blockLen) {
                continue;
            }
            if (i % 2 == 0 && j % 2 == 0) {
                addBlockItem(j, i, "grass");
            }
        }
    }
}

void MainWindow::createScene() {
    scene = new QGraphicsScene;
    view = new SceneView(this);
    view->setScene(scene);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
}

void MainWindow::setSize(){
    scene->setSceneRect(0, 0, 800, 600);
    view->setMinimumSize(400, 400);
}
