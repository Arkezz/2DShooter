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
    //Function that plays the sound depending on the input
    void playSound(QString soundName);

public slots:
    //Mute function
    void mute();
    //Volume changer
    void changeVolume(int volume);

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
