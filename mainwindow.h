#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QScreen>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "sceneview.h"
#include "player.h"

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
    void addTile(int x, int y, QString blockName);
    //Function that draws the scene
    void drawScene();
    //SetSize
    void setSize();

private:
    QGraphicsScene* scene;
    SceneView* view;
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    void createScene();
    Player player;
};
#endif // MAINWINDOW_H
