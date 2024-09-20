#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <QDateTime>
#include <QWidget>

#include "actionsdetail.h"

class Behavior : public QWidget
{
    Q_OBJECT

   public:
    explicit Behavior(QWidget* parent = nullptr);
    void loadAction(Action act, int _x, int _y);
    int  getX() const;
    int  getY() const;
    void setMousePos(int x, int y);

    Action getAction() const;

    // 状态更新函数
    void actionUpdate(int curFrame, long long time);

   private:
    // 边界判断常量
    int LeftEdge, RightEdge, BottomEdge, TopEdge;
    // 桌宠坐标与速度
    int x, y;
    int vx, vy;
    // 鼠标坐标
    int mousex, mousey;
    // 检查方向键是否在按下的状态
    bool leftKey, rightKey, upKey, downKey, jumpKey, featherKey, dashKey, bashKey;
    // 检查鼠标左键是否在按下的状态
    bool mouseLeftKey;
    // 状态机的当前状态
    Action actionBehavior;
    // 控制冷却时间
    int controlTime, freeTime;
    // 二段跳剩余次数
    int jumpChance;
    // 冲刺剩余次数
    int dashChance;
    // 系统时间
    int hour;
    // 是否接受动作限制
    bool limitable;

    // 动作转移函数
    Action NextActions(Action Action);
    // 通用概率函数
    double generalPossiblity(Action);
    // 操作函数
    void inputControl(Action& pre, bool& mirror, bool& restart, double randomValue, int curFrame);
    // 是否在猛击充能
    bool isBashCharging(Action Action);

   signals:
    void PlayerLoadNewActionSignal(Action);
    void PlayerNextPixSignal();
    void PlayerMirrorSignal();
    void PlayerKeepSignal(Action);
    void SoundPlayerLoadNewActionSignal(Action);
    void SoundPlayerPathPlay(QString, int);

   public:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    // void mouseDoubleClickEvent(QMouseEvent* event);

   protected:
    void timerEvent(QTimerEvent* event);

   public slots:
    void setLimitable(bool limit);
};

#endif  // BEHAVIOR_H
