#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMediaPlayer>
#include <QObject>
#include <QVector>

#include "actionsdetail.h"

class SoundPlayer : public QObject
{
    Q_OBJECT
   public:
    explicit SoundPlayer(QObject* parent = nullptr);
    void soundPlay(int curFrame);
    int  getVolume();  // 获取当前音量

   private:
    int                    volume;
    int                    frequency;
    QString                curPath;
    Action                 actionPlaying;
    QVector<QMediaPlayer*> mediaPlayer;
    int                    curPlayer;

   signals:

   public slots:
    void setVolume(int v);
    void loadAction(Action action);
    void loadPathAndPlay(QString path, int num);
    void setMuted(bool muted);
};

#endif  // SOUNDPLAYER_H
