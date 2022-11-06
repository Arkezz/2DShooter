#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	//Start background music
	QMediaPlayer* music = new QMediaPlayer();
	QAudioOutput* audioOutput = new QAudioOutput();
    music->setLoops(-1);
	music->setAudioOutput(audioOutput);
	music->setSource(QUrl("qrc:/sounds/bgm"));
	audioOutput->setVolume(50);
	music->play();

	MainWindow w;

	w.setSize();
	w.show();

	return a.exec();
}
