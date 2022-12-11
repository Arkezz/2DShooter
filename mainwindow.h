#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDialog>
#include <QSlider>
#include <QApplication>
#include <QScreen>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QTimer>
#include "sceneview.h"
#include "player.h"
#include "soundmanager.h"
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void drawScene();
	void setSize();

signals:
	void enemyAttack();

public slots:
	void drawUI();
	void restart();
	void settings();
	void collisionHandler();
	void enemyCollisionHandler();
	void fullScreen();
	void pathFinding(Enemy& enemy);

private:
	QDialog* settingsWindow;
	QGraphicsScene* scene;
	SceneView* view;
	QRect screenGeometry = QApplication::primaryScreen()->geometry();
	void gameOver();
	void win();
	void createScene();
	void addTile(int x, int y, QString blockName);
	Player player;
	Enemy enemies[2];
	QVector<QGraphicsPixmapItem*> hearts;
	Collectibles* object;
	soundManager music;
	QVector<QGraphicsEllipseItem*> footsteps;
	QTimer* invincTimer;
	QTimer* enemyTimer;
	QTimer* tempTimer;
	QGraphicsTextItem* ammo;
	QGraphicsTextItem* statusText;
	Graph* graph;
	int level = 1;
};

#endif // MAINWINDOW_H
