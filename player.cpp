#include "player.h"

#include <QDebug>
#include <QTransform>

Player::Player(QWidget* parent) : QWidget(parent)
{
    startTimer(RATE);
    resize(PNGWIDTH, PNGHEIGHT);
    actionPlaying = None;
}

void Player::loadAction(Action action)
{
    curFrame      = 1;
    timePlayed    = 0;
    actionPlaying = action;
}

void Player::paintEvent(QPaintEvent* event)
{
    QString curPath = ActionsMap[actionPlaying].path + QString::number(curFrame) + ".png";
    pix.load(
        curPath, nullptr, Qt::AvoidDither | Qt::ThresholdAlphaDither | Qt::ThresholdAlphaDither);

    // 是否镜像反转
    if (ActionsMap[actionPlaying].transform)
    {
        QTransform transform;
        pix = pix.transformed(transform.scale(-1, 1));
    }
    setMask(QBitmap(pix.mask()));

    QPainter painter(this);
    painter.drawPixmap(0, 0, pix);
}

void Player::pixUpdate()
{
    curFrame = (curFrame == ActionsMap[actionPlaying].totalFrameNumber) ? 1 : curFrame + 1;
    timePlayed++;
}

void Player::mirrorAction()
{
    actionPlaying = ActionsMirror[actionPlaying];
}
