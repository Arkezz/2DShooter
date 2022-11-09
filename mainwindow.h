#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QSlider>
#include <QApplication>
#include <QScreen>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>
#include "sceneview.h"
#include "player.h"
#include "soundmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	//Function that draws the scene
	void drawScene();
	//SetSize
	void setSize();

public slots:
	//Function for controlling the UI
	void drawUI();
	//Restart function
	void restart();
	//settings function
	void settings();

private:
	QGraphicsScene* scene;
	SceneView* view;
	QRect screenGeometry = QApplication::primaryScreen()->geometry();
	void gameOver();
	void createScene();
	void addTile(int x, int y, QString blockName);
	Player player;
	Enemy enemies[2];
	QVector<QGraphicsPixmapItem*> hearts;
	soundManager music;
};
#endif // MAINWINDOW_H
