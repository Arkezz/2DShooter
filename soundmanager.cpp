#include "soundmanager.h"

soundManager::soundManager()
{
	audioOutput = new QAudioOutput();

	footSteps = new QSoundEffect();
	footSteps->setSource(QUrl("qrc:/sounds/footstepSound"));
	footSteps->setLoopCount(QSoundEffect::Infinite);
	footSteps->setVolume(0.5);

	hurtSound = new QSoundEffect();
	hurtSound->setSource(QUrl("qrc:/sounds/hurtSound"));

	musicPlayer = new QMediaPlayer();
	musicPlayer->setSource(QUrl("qrc:/sounds/bgm"));
	musicPlayer->setAudioOutput(audioOutput);
	musicPlayer->setLoops(-1);
	audioOutput->setVolume(50);
}

void soundManager::playSound(QString soundName)
{
	if (soundName == "footsteps") {
		footSteps->play();
	}
	else if (soundName == "hurt") {
		hurtSound->play();
	}
	else if (soundName == "bgm") {
		musicPlayer->play();
	}
}

void soundManager::mute()
{
    qDebug() << "muted";
    audioOutput->setVolume(0);
}

void soundManager::changeVolume(int volume)
{
	audioOutput->setVolume(volume);
}
