#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

class soundManager
{
public:
	soundManager();
	//Function that plays the sound depending on the input
	void playSound(QString soundName);

private:
	//Footsteps, gunshots, etc
	QSoundEffect* footSteps;
	QSoundEffect* hurtSound;
	//Background music
	QMediaPlayer* musicPlayer;
	//Audiooutput
	QAudioOutput* audioOutput;
};

#endif // SOUNDMANAGER_H
