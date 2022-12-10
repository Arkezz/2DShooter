#include "soundmanager.h"

soundManager::soundManager()
{
	audioOutput = new QAudioOutput();

    footSteps = new QSoundEffect();
    footSteps->setSource(QUrl("qrc:/sounds/footstepSound"));
    //Only play as long as the player is moving
    footSteps->setLoopCount(1);
    footSteps->setVolume(1);

    hurtSound = new QSoundEffect();
    hurtSound->setSource(QUrl("qrc:/sounds/hurtSound"));

    gunShot = new QSoundEffect();
    gunShot->setSource(QUrl("qrc:/sounds/gunShot"));
    gunShot->setVolume(50);

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
    else if (soundName == "gunShot") {
        gunShot->play();
    }
}

bool soundManager::isPlaying(QString soundName)
{
	if (soundName == "footsteps") {
		return footSteps->isPlaying();
	}
	else if (soundName == "hurt") {
		return hurtSound->isPlaying();
    } else {
        return 0;
    }
}

void soundManager::stopSound(QString soundName)
{
	if (soundName == "footsteps") {
		footSteps->stop();
	}
	else if (soundName == "hurt") {
		hurtSound->stop();
	}
	else if (soundName == "bgm") {
		musicPlayer->stop();
	}
}

void soundManager::mute()
{
	audioOutput->setVolume(0);
}

void soundManager::changeVolume(int volume)
{
	audioOutput->setVolume(volume);
}
