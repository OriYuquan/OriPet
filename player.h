#ifndef PLAYER_H
#define PLAYER_H

#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

#include "actionsdetail.h"

class Player : public QWidget
{
    Q_OBJECT
   public:
    explicit Player(QWidget* parent = nullptr);
    bool loadAction(Action action, int cur = 1);
    void pixUpdate();

    int       curFrame;
    long long timePlayed;

   private:
    Action  actionPlaying;
    QPixmap pix;

   protected:
    void paintEvent(QPaintEvent* event);

   signals:

   public slots:
};

#endif  // PLAYER_H
