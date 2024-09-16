#include "soundplayer.h"

#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

SoundPlayer::SoundPlayer(QObject* parent) : QObject(parent)
{
    mediaPlayer   = new QMediaPlayer(this);
    actionPlaying = None;
    volume        = 20;
    mediaPlayer->setVolume(volume);  // Volume is set between 0 and 100
}

void SoundPlayer::loadAction(Action action)
{
    actionPlaying = action;
    if (SoundMap[action].totalFrameNumber != 0)
    {
        if (SoundMap[actionPlaying].frequency != 0)
        {
            frequency =
                ActionsMap[actionPlaying].totalFrameNumber / SoundMap[actionPlaying].frequency + 1;
            quint32 seed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
            QRandomGenerator generator(seed);
            int randomValue = generator.bounded(1, SoundMap[actionPlaying].totalFrameNumber + 1);
            curPath         = SoundMap[actionPlaying].path + QString::number(randomValue) + ".wav";
            // qDebug() << curPath;

            mediaPlayer->setMedia(QUrl::fromLocalFile(curPath));
            mediaPlayer->setVolume(volume);  // Volume is set between 0 and 100
        }
        else
        {
            loadPathAndPlay(SoundMap[actionPlaying].path, SoundMap[actionPlaying].totalFrameNumber);
        }
    }
}

void SoundPlayer::soundPlay(int curFrame)
{
    if (SoundMap[actionPlaying].frequency != 0 && SoundMap[actionPlaying].totalFrameNumber != 0 &&
        curFrame % frequency == 1)
    {
        // qDebug() << actionPlaying;
        if (mediaPlayer->state() == QMediaPlayer::PlayingState)
        {
            mediaPlayer->stop();
        }
        mediaPlayer->play();
    }
}

void SoundPlayer::loadPathAndPlay(QString path, int num)
{
    quint32          seed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
    QRandomGenerator generator(seed);
    int              randomValue = generator.bounded(1, num + 1);
    curPath                      = path + QString::number(randomValue) + ".wav";
    // qDebug() << curPath;

    mediaPlayer->setMedia(QUrl::fromLocalFile(curPath));
    mediaPlayer->setVolume(volume);  // Volume is set between 0 and
    mediaPlayer->play();
}

void SoundPlayer::setVolume(int v)
{
    volume = v;
}
