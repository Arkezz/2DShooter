#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "sceneview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	//Function that adds a block item to the scene. Each block is a 16 by 16 png in assets folder
	void addBlockItem(int x, int y, QString blockName);
	//Function that draws the scene
	void drawScene();
    //SetSize
    void setSize();
private:
	QGraphicsScene* scene;
	QGraphicsPixmapItem* player;
	SceneView* view;
    void createScene();
};
#endif // MAINWINDOW_H
