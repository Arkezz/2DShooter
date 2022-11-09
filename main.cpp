#include "mainwindow.h"
#include <QApplication>
#include "soundmanager.h"
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	//Start background music
	soundManager* sound = new soundManager();
    sound->playSound("bgm");

	MainWindow w;

	w.setSize();
	w.show();

	return a.exec();
}
