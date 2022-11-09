#include "mainwindow.h"
#include <QApplication>
#include "soundmanager.h"
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	//Start background music

	MainWindow w;

	w.setSize();
	w.show();

	return a.exec();
}
