﻿#include "behavior.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <cmath>

using std::abs;
using std::max;
using std::min;

Behavior::Behavior(QWidget* parent) : QWidget(parent)
{  // 参量初始化
    LeftEdge   = SCREENWIDTHFIX - ORIWIDTH * 0;
    RightEdge  = SCREENWIDTHFIX + SCREENWIDTH - ORIWIDTH * 1;
    TopEdge    = SCREENHEIGHTFIX - ORIWIDTH * 0;
    BottomEdge = SCREENHEIGHTFIX + SCREENHEIGHT - ORIHEIGHT * 1;

    leftKey = rightKey = upKey = downKey = jumpKey = featherKey = dashKey = mouseLeftKey = false;
    mouseLeftKey                                                                         = false;
    jumpChance                                                                           = 2;
    controlTime                                                                          = 0;

    mousex = mousey = 0;

    startTimer(100000);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    hour                      = currentDateTime.time().hour();
}

double Behavior::generalPossiblity(Action act)
{
    if (ActionsProbability[actionBehavior][act] < 0)
    {
        if (act == RunSlowlyLeft || act == RunFastLeft)
        {
            return 10.0 * (double) (x - LeftEdge) / (double) (RightEdge - LeftEdge);
        }
        else if (act == RunSlowlyRight || act == RunFastRight)
        {
            return 10.0 * (double) (RightEdge - LeftEdge - (x - LeftEdge)) /
                   (double) (RightEdge - LeftEdge);
        }
        else if (act == ClimbUpLeft || act == ClimbUpRight)
        {
            return 12.0 * (double) (y - TopEdge) / (double) (BottomEdge - TopEdge);
        }
        else if (act == ClimbDownLeft || act == ClimbDownRight)
        {
            if (x == LeftEdge || x == RightEdge)
                return 12.0 * (double) (BottomEdge - TopEdge - (y - TopEdge)) /
                       (double) (BottomEdge - TopEdge);
            else
                return 0;
        }
        else if (act == DoubleJumptoFallLeft)
        {
            return (vx == 0) ? 1 : 0;
        }
        else if (act == DoubleJumptoFallRight)
        {
            return (vx == 0) ? 1 : 0;
        }
        else if (act == DoubleJumptoMovingFallLeft)
        {
            return (vx < 0) ? 1 : 0;
        }
        else if (act == DoubleJumptoMovingFallRight)
        {
            return (vx > 0) ? 1 : 0;
        }
        else if (act == TopClimbLeft)
        {
            if (y == TopEdge)
                return 10.0 * (double) (x - LeftEdge) / (double) (RightEdge - LeftEdge);
            else
                return 0;
        }
        else if (act == TopClimbRight)
        {
            if (y == TopEdge)
                return 10.0 * (double) (RightEdge - LeftEdge - (x - LeftEdge)) /
                       (double) (RightEdge - LeftEdge);
            else
                return 0;
        }
        else if (act == DoubleJumpLeftUp || act == DoubleJumpRightUp)
        {
            return (jumpChance > 0) ? 2 : 0;
        }

        else if (act == FeatherLeft || act == FeatherRight)
        {
            return (abs(vx) < 10) ? abs(vy) / 12 + 1 : 0;
        }
        else if (act == DoubleJumptoMovingFallLeft || act == DoubleJumptoMovingFallLeft)
        {
            return (abs(vx) >= 10) ? abs(vy) / 12 + 1 : 0;
        }

        return 0;
    }
    else
    {
        return ActionsProbability[actionBehavior][act];
    }
}

Action Behavior::NextActions(Action currentAction)
{
    // 获取当前时间的时间戳作为种子
    quint32          seed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
    QRandomGenerator generator(seed);

    const QMap<Action, double>& nextActions = ActionsProbability[currentAction];

    // 计算权重总和
    double               totalWeight = 0;
    QMap<Action, double> values;
    for (auto it = nextActions.cbegin(); it != nextActions.cend(); ++it)
    {
        if (ActionsLeastTimes[it.key()] == 0)
        {
            values[it.key()] = generalPossiblity(it.key());
            totalWeight += values[it.key()];
        }
    }

    // 生成随机数
    double randomValue = generator.bounded(totalWeight);
    // 根据随机数选择下一个动作
    double cumulativeWeight = 0;
    for (auto it = values.cbegin(); it != values.cend(); ++it)
    {
        cumulativeWeight += it.value();
        if (randomValue < cumulativeWeight)
        {
            ActionsLeastTimes[it.key()] = ActionsMap[it.key()].leastTime;
            if (it.key() == DoubleJumpLeftUp || it.key() == DoubleJumpRightUp)
                jumpChance--;
            // qDebug() << ActionsMap[it.key()].leastTime;
            return it.key();
        }
    }
    ActionsLeastTimes[currentAction] = ActionsMap[currentAction].leastTime;
    return currentAction;
}

// 状态机更新
void Behavior::actionUpdate(int curFrame, long long time)
{
    bool mirror  = false;
    bool restart = false;

    vx = ActionsDX(actionBehavior, x, y, vx, vy, curFrame, time, mousex, mousey);
    vy = ActionsDY(actionBehavior, x, y, vx, vy, curFrame, time, mousex, mousey);
    x += vx;
    y += vy;

    // 边界位置判断
    x = max(x, LeftEdge);
    x = min(x, RightEdge);
    y = max(y, TopEdge);
    y = min(y, BottomEdge);

    Action pre = actionBehavior;

    // 检查是否存在控制
    bool control =
        leftKey || rightKey || upKey || downKey || jumpKey || featherKey || dashKey || mouseLeftKey;
    if (control)
    {
        controlTime = CONTROLTIME;
    }
    else
    {
        controlTime = (controlTime == 0) ? 0 : controlTime - 1;
    }

    quint32          seed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
    QRandomGenerator generator(seed);
    double           randomValue = generator.bounded(1.0);
    // 检查是否可以进行状态转移
    // 如果操控已冷却，状态机转移
    if (controlTime == 0 && curFrame == ActionsMap[actionBehavior].totalFrameNumber &&
        ActionsMap[actionBehavior].keepPossiblity < randomValue)
    {
        actionBehavior = NextActions(actionBehavior);
    }
    else if (controlTime != 0)
    {
        // 播放完转移
        if ((curFrame == ActionsMap[actionBehavior].totalFrameNumber ||
             ActionsMap[actionBehavior].completelyPlay == false))
        {
            if (actionBehavior == DoubleJumpLeftDown || actionBehavior == DoubleJumpRightDown)
            {
                actionBehavior =
                    (abs(vx) < 10)
                        ? ((ActionsMap[actionBehavior].transform) ? DoubleJumptoFallLeft
                                                                  : DoubleJumptoFallRight)
                        : ((ActionsMap[actionBehavior].transform) ? DoubleJumptoMovingFallLeft
                                                                  : DoubleJumptoMovingFallRight);
            }
            else
            {
                actionBehavior = ActionsColdTrans[actionBehavior];
            }
        }
        // 未播放完转移
        else if (actionBehavior == DoubleJumpLeftUp || actionBehavior == DoubleJumpRightUp ||
                 actionBehavior == DoubleJumpLeftDown || actionBehavior == DoubleJumpRightDown)
        {
            if (vx > 0)
                vx--;
            else if (vx < 0)
                vx++;
        }
        else if (pre == FallLeft || pre == FallRight || pre == MovingFallLeft ||
                 pre == MovingFallRight)
        {
            if (vx > 0)
                vx--;
            else if (vx < 0)
                vx++;
            if (abs(vx) < 10)
                actionBehavior = int(actionBehavior) % 2 == 0 ? FallLeft : FallRight;
        }
        else if (pre == FeatherLeft || pre == FeatherRight || pre == MovingFeatherLeft ||
                 pre == MovingFeatherRight)
        {
            if (abs(vx) < 10)
                actionBehavior = int(actionBehavior) % 2 == 0 ? FeatherLeft : FeatherRight;
        }
    }

    // 按键控制
    if (ActionsMap[actionBehavior].control)
    {
        if (y == BottomEdge)
        {
            if (leftKey)
                actionBehavior = RunFastLeft;
            if (rightKey)
                actionBehavior = RunFastRight;
            if (jumpKey)
            {
                if (pre == RunFastLeft || pre == RunFastRight)
                {
                    actionBehavior = ActionsMap[actionBehavior].transform
                                         ? (randomValue > 0.5 ? RunJump1LeftUp : RunJump2LeftUp)
                                         : (randomValue > 0.5 ? RunJump1RightUp : RunJump2RightUp);
                }
                else
                {
                    actionBehavior = ActionsMap[actionBehavior].transform
                                         ? (randomValue > 0.5 ? Jump1LeftUp : Jump2LeftUp)
                                         : (randomValue > 0.5 ? Jump1RightUp : Jump2RightUp);
                }
                jumpKey = false;
            }
            if (dashKey)
            {
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? DashBeginLeft : DashBeginRight;
                dashKey = false;
            }
        }
        if (y == TopEdge)
        {
            if (leftKey)
            {
                actionBehavior = TopClimbLeft;
            }
            if (rightKey)
            {
                actionBehavior = TopClimbRight;
            }
            if (jumpKey)
            {
                if (pre == TopStayLeft || pre == TopStayRight)
                {
                    actionBehavior = int(actionBehavior) % 2 == 0 ? FallLeft : FallRight;
                }
                else if (pre == TopClimbLeft || pre == TopClimbRight)
                {
                    actionBehavior =
                        int(actionBehavior) % 2 == 0 ? MovingFallLeft : MovingFallRight;
                }
                jumpKey = false;
            }
        }
        if (x == LeftEdge)
        {
            if (upKey)
                actionBehavior = ClimbUpLeft;
            if (downKey)
                actionBehavior = ClimbDownLeft;
            if (jumpKey)
            {
                if (rightKey)
                    actionBehavior = randomValue > 0.5 ? WallLongJump1Left : WallLongJump2Left;
                else if (leftKey)
                    actionBehavior = WallJumpClimbLeft;
                else
                    actionBehavior = randomValue > 0.5 ? WallJump1Left : WallJump2Left;
                jumpKey = false;
            }
        }
        if (x == RightEdge)
        {
            if (upKey)
                actionBehavior = ClimbUpRight;
            if (downKey)
                actionBehavior = ClimbDownRight;
            if (jumpKey)
            {
                if (leftKey)
                    actionBehavior = randomValue > 0.5 ? WallLongJump1Right : WallLongJump2Right;
                else if (rightKey)
                    actionBehavior = WallJumpClimbRight;
                else
                    actionBehavior = randomValue > 0.5 ? WallJump1Right : WallJump2Right;
                jumpKey = false;
            }
        }
        if (x > LeftEdge && x < RightEdge && y < BottomEdge && y > TopEdge)
        {
            if (jumpKey)
            {
                if (jumpChance >= 1)
                {
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? DoubleJumpLeftUp : DoubleJumpRightUp;
                    restart = true;
                    jumpChance--;
                }
                jumpKey = false;
            }
            else if (featherKey)
            {
                if (pre == FeatherAfterMouseLeft)
                    pre = FeatherLeft;
                if (pre == FeatherAfterMouseRight)
                    pre = FeatherRight;
                actionBehavior = int(actionBehavior) % 2 == 0 ? FeatherLeft : FeatherRight;
                if (leftKey)
                {
                    vx             = -18;
                    actionBehavior = MovingFeatherLeft;
                }
                if (rightKey)
                {
                    vx             = 18;
                    actionBehavior = MovingFeatherRight;
                }
            }
            else
            {
                if (leftKey)
                {
                    vx = -18;
                    if (actionBehavior == DoubleJumpRightUp)
                    {
                        actionBehavior = DoubleJumpLeftUp;
                        mirror         = true;
                    }
                    else if (actionBehavior == DoubleJumpRightDown)
                    {
                        actionBehavior = DoubleJumpLeftDown;
                        mirror         = true;
                    }
                    else if (actionBehavior == FallLeft || actionBehavior == FallRight)
                    {
                        actionBehavior = MovingFallLeft;
                    }
                    else if (actionBehavior == MovingFallRight)
                    {
                        actionBehavior = MovingFallLeft;
                        mirror         = true;
                    }
                    else if (actionBehavior == RunJump1RightUp ||
                             actionBehavior == RunJump1RightDown ||
                             actionBehavior == RunJump2RightUp ||
                             actionBehavior == RunJump2RightDown)
                    {
                        actionBehavior = ActionsMirror[actionBehavior];
                        mirror         = true;
                    }
                    else if (actionBehavior == FeatherAfterMouseLeft ||
                             actionBehavior == FeatherAfterMouseRight)
                    {
                        actionBehavior = MovingFallLeft;
                    }
                }
                if (rightKey)
                {
                    vx = 18;
                    if (actionBehavior == DoubleJumpLeftUp)
                    {
                        actionBehavior = DoubleJumpRightUp;
                        mirror         = true;
                    }
                    else if (actionBehavior == DoubleJumpLeftDown)
                    {
                        actionBehavior = DoubleJumpRightDown;
                        mirror         = true;
                    }
                    else if (actionBehavior == FallLeft || actionBehavior == FallRight)
                    {
                        actionBehavior = MovingFallRight;
                    }
                    else if (actionBehavior == MovingFallLeft)
                    {
                        actionBehavior = MovingFallRight;
                        mirror         = true;
                    }
                    else if (actionBehavior == RunJump1LeftUp ||
                             actionBehavior == RunJump1LeftDown ||
                             actionBehavior == RunJump2LeftUp || actionBehavior == RunJump2LeftDown)
                    {
                        actionBehavior = ActionsMirror[actionBehavior];
                        mirror         = true;
                    }
                    else if (actionBehavior == FeatherAfterMouseLeft ||
                             actionBehavior == FeatherAfterMouseRight)
                    {
                        actionBehavior = MovingFallRight;
                    }
                }
            }
        }

        if (mouseLeftKey && mousex > LeftEdge && mousex < RightEdge && mousey < BottomEdge &&
            mousey > TopEdge)
        {
            actionBehavior = int(actionBehavior) % 2 == 0 ? MouseHoldLeft : MouseHoldRight;
        }
    }

    int vxCheck = pre == actionBehavior
                      ? vx
                      : ActionsDX(actionBehavior, x, y, vx, vy, 1, time, mousex, mousey);
    int vyCheck = pre == actionBehavior
                      ? vy
                      : ActionsDY(actionBehavior, x, y, vx, vy, 1, time, mousex, mousey);

    // 边界判断
    if (x == LeftEdge && vxCheck < 0)
    {
        mouseLeftKey = false;
        if (y == BottomEdge)
        {
            actionBehavior = AgainstWallLeft;
        }
        else if (y == TopEdge)
        {
            actionBehavior = TopStayLeft;
        }
        else
        {
            emit SoundPlayerPathPlay("Sound/wallEnter/seinWallSlideEnterGrass", 5);
            actionBehavior = WallStayLeft;
        }
    }
    else if (x == RightEdge && vxCheck > 0)
    {
        mouseLeftKey = false;
        if (y == BottomEdge)
        {
            actionBehavior = AgainstWallRight;
        }
        else if (y == TopEdge)
        {
            actionBehavior = TopStayRight;
        }
        else
        {
            emit SoundPlayerPathPlay("Sound/wallEnter/seinWallSlideEnterGrass", 5);
            actionBehavior = WallStayRight;
        }
    }
    else if (y == TopEdge && vyCheck < 0)
    {
        mouseLeftKey = false;
        if (x == LeftEdge)
        {
            actionBehavior = WallStayLeft;
        }
        else if (x == RightEdge)
        {
            actionBehavior = WallStayRight;
        }
        else
        {
            emit SoundPlayerPathPlay("Sound/wallEnter/seinWallSlideEnterGrass", 5);
            actionBehavior = int(actionBehavior) % 2 == 0 ? TopStayLeft : TopStayRight;
        }
    }
    else if (y == BottomEdge && vyCheck > 0)
    {
        mouseLeftKey = false;
        if (x == LeftEdge)
        {
            actionBehavior = AgainstWallLeft;
        }
        else if (x == RightEdge)
        {
            actionBehavior = AgainstWallRight;
        }
        else
        {
            actionBehavior = ActionsMap[actionBehavior].transform ? LandStandLeft : LandStandRight;
        }
    }

    // 跳跃次数更新
    if (x == LeftEdge || x == RightEdge || y == BottomEdge || y == TopEdge)
    {
        jumpChance = 2;
    }

    // 动作冷却更新
    for (int i = 0; i < int(None); i++)
    {
        if (ActionsLeastTimes[Action(i)] != 0)
        {
            ActionsLeastTimes[Action(i)]--;
        }
        // 冷却时间调试输出
        // qDebug() << i << ActionsLeastTimes[Action(i)];
    }
    qDebug() << x << " " << y << " " << vxCheck << " " << vyCheck << " " << curFrame << " "
             << actionBehavior << " " << jumpChance << " " << mousex << " " << mousey << " "
             << mouseLeftKey << " " << controlTime;

    if (actionBehavior == pre)
    {
        if (restart)
            emit PlayerLoadNewActionSignal(actionBehavior);
        else
            emit PlayerNextPixSignal();
    }
    else
    {
        if (mirror && actionBehavior == ActionsMirror[pre])
        {
            emit PlayerMirrorSignal();
            emit PlayerNextPixSignal();
        }
        else
        {
            emit PlayerLoadNewActionSignal(actionBehavior);
            emit SoundPlayerLoadNewActionSignal(actionBehavior);
        }
    }
}

void Behavior::loadAction(Action act, int _x, int _y)
{
    actionBehavior = act;
    x              = _x;
    y              = _y;
    vx = vy = 0;
}

int Behavior::getX() const
{
    return int(x);
}

int Behavior::getY() const
{
    return int(y);
}

void Behavior::setMousePos(int x, int y)
{
    mousex = x + LeftEdge - 32;
    mousey = y + TopEdge + 30;
}

Action Behavior::getAction() const
{
    return actionBehavior;
}

void Behavior::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left && !event->isAutoRepeat())
    {
        leftKey  = true;
        rightKey = false;
    }
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat())
    {
        rightKey = true;
        leftKey  = false;
    }
    if (event->key() == Qt::Key_Up && !event->isAutoRepeat())
    {
        upKey   = true;
        downKey = false;
    }
    if (event->key() == Qt::Key_Down && !event->isAutoRepeat())
    {
        downKey = true;
        upKey   = false;
    }
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
    {
        jumpKey    = true;
        featherKey = false;
    }
    if (event->key() == Qt::Key_Z && !event->isAutoRepeat())
    {
        featherKey = true;
    }
    if (event->key() == Qt::Key_X && !event->isAutoRepeat())
    {
        dashKey = true;
    }
}

void Behavior::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left && !event->isAutoRepeat())
    {
        leftKey = false;
    }
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat())
    {
        rightKey = false;
    }
    if (event->key() == Qt::Key_Up && !event->isAutoRepeat())
    {
        upKey = false;
    }
    if (event->key() == Qt::Key_Down && !event->isAutoRepeat())
    {
        downKey = false;
    }
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
    {
        jumpKey = false;
    }
    if (event->key() == Qt::Key_Z && !event->isAutoRepeat())
    {
        featherKey = false;
    }
    if (event->key() == Qt::Key_X && !event->isAutoRepeat())
    {
        featherKey = false;
    }
}

void Behavior::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        mouseLeftKey = true;
}
void Behavior::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        mouseLeftKey = false;
}

void Behavior::timerEvent(QTimerEvent* event)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    hour                      = currentDateTime.time().hour();
}
