#include "behavior.h"

#include <QCursor>
#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <cmath>

using std::max;
using std::min;

Behavior::Behavior(QWidget* parent) : QWidget(parent)
{  // 参量初始化
    LeftEdge   = SCREENWIDTHFIX - ORIWIDTH * 0;
    RightEdge  = SCREENWIDTHFIX + SCREENWIDTH - ORIWIDTH * 1;
    TopEdge    = SCREENHEIGHTFIX - ORIWIDTH * 0;
    BottomEdge = SCREENHEIGHTFIX + SCREENHEIGHT - ORIHEIGHT * 1;

    leftKey = rightKey = upKey = downKey = jumpKey = false;
    jumpChance                                     = 2;
    controlTime                                    = 0;

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
            return 10.0 * (double) (y - TopEdge) / (double) (BottomEdge - TopEdge);
        }
        else if (act == ClimbDownLeft || act == ClimbDownRight)
        {
            return 10.0 * (double) (BottomEdge - TopEdge - (y - TopEdge)) /
                   (double) (BottomEdge - TopEdge);
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
            // qDebug() << ActionsMap[it.key()].leastTime;
            return it.key();
        }
    }
    ActionsLeastTimes[currentAction] = ActionsMap[currentAction].leastTime;
    return currentAction;
}

// 状态机更新
bool Behavior::actionUpdate(int curFrame, long long time)
{
    bool result = false;

    vx = ActionsDX(actionBehavior, x, y, vx, vy, curFrame, time);
    vy = ActionsDY(actionBehavior, x, y, vx, vy, curFrame, time);
    x += vx;
    y += vy;

    Action pre = actionBehavior;

    // 检查是否存在控制
    bool control = leftKey || rightKey || upKey || downKey || jumpKey;
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
    else if (controlTime != 0 && (curFrame == ActionsMap[actionBehavior].totalFrameNumber ||
                                  ActionsMap[actionBehavior].completelyPlay == false))
    {
        if (actionBehavior == DoubleJumpLeft || actionBehavior == DoubleJumpRight ||
            actionBehavior == TripleJumpLeft || actionBehavior == TripleJumpRight)
        {
            actionBehavior =
                (vx == 0) ? ((ActionsMap[actionBehavior].transform) ? DoubleJumptoFallLeft
                                                                    : DoubleJumptoFallRight)
                          : ((ActionsMap[actionBehavior].transform) ? DoubleJumptoMovingFallLeft
                                                                    : DoubleJumptoMovingFallRight);
        }
        else
        {
            actionBehavior = ActionsColdTrans[actionBehavior];
        }
    }

    // 边界位置判断
    x = max(x, LeftEdge);
    x = min(x, RightEdge);
    y = max(y, TopEdge);
    y = min(y, BottomEdge);

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
                                         ? (randomValue > 0.5 ? RunJump1Left : RunJump2Left)
                                         : (randomValue > 0.5 ? RunJump1Right : RunJump2Right);
                }
                else
                {
                    actionBehavior = ActionsMap[actionBehavior].transform
                                         ? (randomValue > 0.5 ? Jump1Left : Jump2Left)
                                         : (randomValue > 0.5 ? Jump1Right : Jump2Right);
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
            }
        }
        if ((pre == Jump1Left || pre == Jump1Right || pre == Jump2Left || pre == Jump2Right ||
             pre == RunJump1Left || pre == RunJump1Right || pre == RunJump2Left ||
             pre == RunJump2Right) &&
            y < BottomEdge)
        {
            if (jumpKey)
            {
                if (jumpChance >= 1)
                {
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? DoubleJumpLeft : DoubleJumpRight;
                    jumpChance--;
                }
                jumpKey = false;
            }
        }
        if ((pre == DoubleJumpLeft || pre == DoubleJumpRight) && y < BottomEdge)
        {
            if (jumpKey)
            {
                if (jumpChance >= 1)
                {
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? TripleJumpLeft : TripleJumpRight;
                    jumpChance--;
                }
                jumpKey = false;
            }
        }
    }

    int vxCheck = pre == actionBehavior ? vx : ActionsDX(actionBehavior, x, y, vx, vy, 1, time);
    int vyCheck = pre == actionBehavior ? vy : ActionsDY(actionBehavior, x, y, vx, vy, 1, time);

    // 边界判断
    if (x == LeftEdge && vxCheck < 0)
    {
        if (y == BottomEdge)
        {
            actionBehavior = AgainstWallLeft;
        }
        else if (y == TopEdge)
        {
        }
        else
        {
            actionBehavior = WallStayLeft;
        }
    }
    else if (x == RightEdge && vxCheck > 0)
    {
        if (y == BottomEdge)
        {
            actionBehavior = AgainstWallRight;
        }
        else if (y == TopEdge)
        {
        }
        else
        {
            actionBehavior = WallStayRight;
        }
    }
    else if (y == TopEdge && vyCheck < 0)
    {
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
        }
    }
    else if (y == BottomEdge && vyCheck > 0)
    {
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

    return result;
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

Action Behavior::getAction() const
{
    return actionBehavior;
}

void Behavior::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
    {
        leftKey  = true;
        rightKey = false;
    }
    if (event->key() == Qt::Key_Right)
    {
        rightKey = true;
        leftKey  = false;
    }
    if (event->key() == Qt::Key_Up)
    {
        upKey   = true;
        downKey = false;
    }
    if (event->key() == Qt::Key_Down)
    {
        downKey = true;
        upKey   = false;
    }
    if (event->key() == Qt::Key_Space)
    {
        jumpKey = true;
    }
}

void Behavior::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
    {
        leftKey = false;
    }
    if (event->key() == Qt::Key_Right)
    {
        rightKey = false;
    }
    if (event->key() == Qt::Key_Up)
    {
        upKey = false;
    }
    if (event->key() == Qt::Key_Down)
    {
        downKey = false;
    }
    if (event->key() == Qt::Key_Space)
    {
        jumpKey = false;
    }
}

void Behavior::timerEvent(QTimerEvent* event)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    hour                      = currentDateTime.time().hour();
}
