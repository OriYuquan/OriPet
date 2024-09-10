#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMediaPlayer>
#include <QObject>

#include "actionsdetail.h"

class SoundPlayer : public QObject
{
    Q_OBJECT
   public:
    explicit SoundPlayer(QObject* parent = nullptr);
    void soundPlay(int curFrame);

   private:
    int           frequency;
    QString       curPath;
    Action        actionPlaying;
    QMediaPlayer* mediaPlayer;

   signals:

   public slots:
    void loadAction(Action action);
};

#endif  // SOUNDPLAYER_H
