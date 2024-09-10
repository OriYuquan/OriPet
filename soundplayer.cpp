#include "soundplayer.h"

#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

SoundPlayer::SoundPlayer(QObject* parent) : QObject(parent)
{
    mediaPlayer   = new QMediaPlayer(this);
    actionPlaying = None;
    mediaPlayer->setVolume(20);  // Volume is set between 0 and 100
}

void SoundPlayer::loadAction(Action action)
{
    actionPlaying = action;
    if (SoundMap[action].totalFrameNumber != 0)
    {
        frequency =
            ActionsMap[actionPlaying].totalFrameNumber / SoundMap[actionPlaying].frequency + 1;
        quint32 seed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
        QRandomGenerator generator(seed);
        int randomValue = generator.bounded(1, SoundMap[actionPlaying].totalFrameNumber + 1);
        curPath         = SoundMap[actionPlaying].path + QString::number(randomValue) + ".wav";
        // qDebug() << curPath;

        mediaPlayer->setMedia(QUrl::fromLocalFile(curPath));
        mediaPlayer->setVolume(20);  // Volume is set between 0 and 100
    }
}

void SoundPlayer::soundPlay(int curFrame)
{
    if (SoundMap[actionPlaying].totalFrameNumber != 0 && curFrame % frequency == 1)
    {
        // qDebug() << actionPlaying;
        if (mediaPlayer->state() == QMediaPlayer::PlayingState)
        {
            mediaPlayer->stop();
        }
        mediaPlayer->play();
    }
}
