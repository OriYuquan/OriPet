#include "soundplayer.h"

#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

SoundPlayer::SoundPlayer(QObject* parent) : QObject(parent)
{
    mediaPlayer.resize(4);
    for (int i = 0; i < mediaPlayer.size(); i++)
        mediaPlayer[i] = new QMediaPlayer(this);
    curPlayer     = 0;
    actionPlaying = None;
    volume        = 20;
}

void SoundPlayer::loadAction(Action action)
{
    actionPlaying = action;
    if (SoundMap[action].totalFrameNumber != 0)
    {
        if (SoundMap[actionPlaying].frequency != 0)
        {
            frequency =
                static_cast<int>(static_cast<double>(ActionsMap[actionPlaying].totalFrameNumber) /
                                     static_cast<double>(SoundMap[actionPlaying].frequency) +
                                 0.5);
            int randomValue = QRandomGenerator::system()->bounded(
                1, SoundMap[actionPlaying].totalFrameNumber + 1);
            curPath = SoundMap[actionPlaying].path + QString::number(randomValue) + ".wav";
            // qDebug() << curPath;
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
        // qDebug() << actionPlaying << " " << curPlayer;
        mediaPlayer[curPlayer]->setMedia(QUrl::fromLocalFile(curPath));
        mediaPlayer[curPlayer]->setVolume(volume);  // Volume is set between 0 and 100
        mediaPlayer[curPlayer]->play();
        curPlayer++;
        curPlayer = (curPlayer == mediaPlayer.size()) ? 0 : curPlayer;
    }
}

void SoundPlayer::loadPathAndPlay(QString path, int num)
{
    quint32          seed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
    QRandomGenerator generator(seed);
    int              randomValue = generator.bounded(1, num + 1);
    curPath                      = path + QString::number(randomValue) + ".wav";
    // qDebug() << actionPlaying << " " << curPlayer;
    mediaPlayer[curPlayer]->setMedia(QUrl::fromLocalFile(curPath));
    mediaPlayer[curPlayer]->setVolume(volume);  // Volume is set between 0 and
    mediaPlayer[curPlayer]->play();
    curPlayer++;
    curPlayer = (curPlayer == mediaPlayer.size()) ? 0 : curPlayer;
}

void SoundPlayer::setVolume(int v)
{
    volume = v;
}

void SoundPlayer::setMuted(bool muted)
{
    for (int i = 0; i < mediaPlayer.size(); i++)
        mediaPlayer[i]->setMuted(muted);
}

int SoundPlayer::getVolume()
{
    return volume;
}
