#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

class soundManager : public QObject
{
	Q_OBJECT
public:
	soundManager();
	void playSound(QString soundName);
	bool isPlaying(QString soundName);
	void stopSound(QString soundName);

public slots:
	void mute();
	void changeVolume(int volume);

private:
    //Footsteps, gunshots, etc
    QSoundEffect* footSteps;
    QSoundEffect* hurtSound;
    QSoundEffect* gunShot;
    //Background music
	QMediaPlayer* musicPlayer;
	QAudioOutput* audioOutput;
};

#endif // SOUNDMANAGER_H
