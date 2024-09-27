#include "behavior.h"

#include <QCursor>
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

    leftKey = rightKey = upKey = downKey = jumpKey = featherKey = dashKey = mouseLeftKey = bashKey =
        false;
    mouseLeftKey = false;
    jumpChance   = 2;
    dashChance   = 1;
    controlTime = freeTime = 0;
    limitable              = false;

    x = y = vx = vy = mousex = mousey = 0;

    startTimer(10000);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    hour                      = currentDateTime.time().hour();
}

double Behavior::generalPossiblity(Action act)
{
    if (ActionsProbability[actionBehavior][act] < 0)
    {
        if (act == RunSlowlyLeft || act == RunFastLeft || act == GetDownWalkLeft || act == WalkLeft)
        {
            return 10.0 * (double) (x - LeftEdge) / (double) (RightEdge - LeftEdge);
        }
        else if (act == RunSlowlyRight || act == RunFastRight || act == GetDownWalkRight ||
                 act == WalkRight)
        {
            return 10.0 * (double) (RightEdge - LeftEdge - (x - LeftEdge)) /
                   (double) (RightEdge - LeftEdge);
        }
        else if (act == RunLeft)
        {
            return 2.0 * (double) (x - LeftEdge) / (double) (RightEdge - LeftEdge);
        }
        else if (act == RunRight)
        {
            return 2.0 * (double) (RightEdge - LeftEdge - (x - LeftEdge)) /
                   (double) (RightEdge - LeftEdge);
        }
        else if (act == ClimbUpLeft || act == ClimbUpRight)
        {
            if ((act == ClimbUpLeft && x == LeftEdge) || (act == ClimbUpRight && x == RightEdge))
                return 8.0 * (double) (y - TopEdge) / (double) (BottomEdge - TopEdge);
        }
        else if (act == ClimbDownLeft || act == ClimbDownRight)
        {
            if ((act == ClimbDownLeft && x == LeftEdge) ||
                (act == ClimbDownRight && x == RightEdge))
                return 8.0 * (double) (BottomEdge - TopEdge - (y - TopEdge)) /
                       (double) (BottomEdge - TopEdge);
            else
                return 0;
        }
        else if (act == DoubleJumptoFallLeft)
        {
            return (abs(vx) < 10) ? 2 : 0;
        }
        else if (act == DoubleJumptoFallRight)
        {
            return (abs(vx) < 10) ? 2 : 0;
        }
        else if (act == DoubleJumptoMovingFallLeft)
        {
            return (abs(vx) >= 10) ? 2 : 0;
        }
        else if (act == DoubleJumptoMovingFallRight)
        {
            return (abs(vx) >= 10) ? 2 : 0;
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
            return (jumpChance > 0 && y > 150) ? 3 : 0;
        }
        else if (act == AirDashLeft || act == AirDashRight)
        {
            return (dashChance > 0 && y > 150) ? 2 : 0;
        }

        else if (act == FeatherLeft || act == FeatherRight)
        {
            return (abs(vx) < 10 && y < (BottomEdge - 30))
                       ? (abs(vy) / 9 + 5 - (jumpChance + dashChance))
                       : 0;
        }
        else if (act == MovingFeatherLeft || act == MovingFeatherRight)
        {
            return (abs(vx) >= 10 && y < (BottomEdge - 30))
                       ? (abs(vy) / 9 + 5 - (jumpChance + dashChance))
                       : 0;
        }

        else if (act == StandtoLookUpListenLeft || act == StandtoLookUpListenRight)
        {
            if ((act == StandtoLookUpListenLeft &&
                 x > ((RightEdge - LeftEdge) * 2 / 3 + LeftEdge)) ||
                (act == StandtoLookUpListenRight && x < ((RightEdge - LeftEdge) / 3 + LeftEdge)))
                return 7;
            else
                return 1;
        }
        else if (act == LookUpLeft || act == LookUpRight)
        {
            if ((act == LookUpLeft && mousex < x && mousex > x - 600 && mousey < 800) ||
                (act == LookUpRight && mousex > x && mousex < x + 600 && mousey < 800))
                return 20;
            else
                return 1;
        }
        else if (act == YawnLeft || act == YawnRight)
        {
            if (freeTime > 24000 && (hour >= 23 || hour < 7))
                return 25;
            else
                return 1;
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
    const QMap<Action, double>& nextActions = ActionsProbability[currentAction];

    // 计算权重总和
    double               totalWeight = 0;
    QMap<Action, double> values;
    for (Action key : nextActions.keys())
    {
        if (ActionsLeastTimes[key] == 0 &&
            (!limitable || ActionLimit.find(key) == ActionLimit.end()))
        {
            values[key] = generalPossiblity(key);
            totalWeight += values[key];
        }
    }

    // 生成随机数
    double randomValue = QRandomGenerator::system()->bounded(totalWeight);
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
            if (it.key() == AirDashLeft || it.key() == AirDashRight)
                dashChance--;
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

    Action pre = actionBehavior;

    // 检查是否存在控制
    bool control = (leftKey || rightKey || upKey || downKey || jumpKey || featherKey || dashKey ||
                    mouseLeftKey || bashKey);
    if (control)
    {
        freeTime    = 0;
        controlTime = CONTROLTIME;
    }
    else
    {
        freeTime    = (freeTime > 1000000) ? freeTime : freeTime + 1;
        controlTime = (controlTime == 0) ? 0 : controlTime - 1;
    }

    vx = ActionsDX(actionBehavior, x, y, vx, vy, curFrame, time, mousex, mousey, controlTime != 0);
    vy = ActionsDY(actionBehavior, x, y, vx, vy, curFrame, time, mousex, mousey, controlTime != 0);
    x += vx;
    y += vy;

    // 边界位置判断
    x = max(x, LeftEdge);
    x = min(x, RightEdge);
    y = max(y, TopEdge);
    y = min(y, BottomEdge);

    double randomValue = QRandomGenerator::system()->bounded(1.0);
    // 检查是否可以进行状态转移
    // 如果操控已冷却，状态机转移
    if (controlTime == 0 &&
        (curFrame == ActionsMap[actionBehavior].totalFrameNumber ||
         ActionsMap[actionBehavior].keepPossiblity < 0.0) &&
        abs(ActionsMap[actionBehavior].keepPossiblity) < randomValue)
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
                 actionBehavior == DoubleJumpLeftDown || actionBehavior == DoubleJumpRightDown ||
                 actionBehavior == Jump1LeftUp || actionBehavior == Jump1RightUp ||
                 actionBehavior == Jump1LeftDown || actionBehavior == Jump1RightDown ||
                 actionBehavior == Jump2LeftUp || actionBehavior == Jump2RightUp ||
                 actionBehavior == Jump2LeftDown || actionBehavior == Jump2RightDown ||
                 actionBehavior == Jump3LeftUp || actionBehavior == Jump3RightUp ||
                 actionBehavior == Jump3LeftDown || actionBehavior == Jump3RightDown)
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
                actionBehavior = ActionsMap[actionBehavior].transform ? FallLeft : FallRight;
        }
        else if (pre == FeatherLeft || pre == FeatherRight || pre == MovingFeatherLeft ||
                 pre == MovingFeatherRight)
        {
            if (abs(vx) < 10)
                actionBehavior = ActionsMap[actionBehavior].transform ? FeatherLeft : FeatherRight;
        }
    }

    // 按键控制
    if (ActionsMap[pre].control)
    {
        inputControl(pre, mirror, restart, randomValue, curFrame);
    }

    int vxCheck =
        pre == actionBehavior
            ? vx
            : ActionsDX(actionBehavior, x, y, vx, vy, 1, time, mousex, mousey, controlTime != 0);
    int vyCheck =
        pre == actionBehavior
            ? vy
            : ActionsDY(actionBehavior, x, y, vx, vy, 1, time, mousex, mousey, controlTime != 0);

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
            actionBehavior = ActionsMap[actionBehavior].transform ? TopStayLeft : TopStayRight;
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
            if (vyCheck > 60 && abs(vx) > 15 && controlTime == 0 && randomValue > 0.5)
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? LandRollLeft : LandRollRight;
        }
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

    // 跳跃次数更新
    if (x == LeftEdge || x == RightEdge || y == BottomEdge || y == TopEdge || isBashCharging(pre))
    {
        jumpChance = 2;
        dashChance = 1;
    }

    debugMessage = "x:" + QString::number(x) + "\ny:" + QString::number(y) +
                   "\nvx:" + QString::number(vxCheck) + "\nvy:" + QString::number(vyCheck) +
                   "\ncurFrame:" + QString::number(curFrame) +
                   "\naction:" + QString::number(actionBehavior) +
                   "\njumpChance:" + QString::number(jumpChance) +
                   "\ndashChance:" + QString::number(dashChance) +
                   "\nmouseKey:" + QString::number(int(mouseLeftKey)) +
                   "\ncontrolTime:" + QString::number(controlTime) +
                   "\nmouseX:" + QString::number(mousex) + "\nmouseY:" + QString::number(mousey);
    qDebug() << debugMessage;

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
        else if (isBashCharging(pre) && isBashCharging(actionBehavior))
        {
            emit PlayerKeepSignal(actionBehavior);
            emit PlayerNextPixSignal();
        }
        else
        {
            emit PlayerLoadNewActionSignal(actionBehavior);
            emit SoundPlayerLoadNewActionSignal(actionBehavior);
        }
    }
}

bool Behavior::isBashCharging(Action action)
{
    return action == BashUpChargeLeft || action == BashUpChargeRight ||
           action == BashHorChargeLeft || action == BashHorChargeRight ||
           action == BashDownChargeLeft || action == BashDownChargeRight ||
           action == BashDiaUpChargeLeft || action == BashDiaUpChargeRight ||
           action == BashDiaDownChargeLeft || action == BashDiaDownChargeRight;
}

void Behavior::inputControl(Action& pre,
                            bool&   mirror,
                            bool&   restart,
                            double  randomValue,
                            int     curFrame)
{
    if (mouseLeftKey && mousex > LeftEdge && mousex < RightEdge && mousey < BottomEdge &&
        mousey > TopEdge)
    {
        actionBehavior = ActionsMap[actionBehavior].transform ? MouseHoldLeft : MouseHoldRight;
        return;
    }

    // 猛击优先判断
    bool side = mousex > (x - 70) && mousex < (x + 110) && mousey > (y - 30) && mousey < (y + 130);
    if (bashKey && (side || isBashCharging(pre)))
    {
        if (pre == BashUpChargeLeft)
        {
            x = originalMouseX + 40;
            y = originalMousey - 130;
        }
        else if (pre == BashUpChargeRight)
        {
            x = originalMouseX - 40;
            y = originalMousey - 130;
        }
        else if (pre == BashHorChargeLeft)
        {
            x = originalMouseX + 25;
            y = originalMousey - 130;
        }
        else if (pre == BashHorChargeRight)
        {
            x = originalMouseX - 25;
            y = originalMousey - 130;
        }
        else if (pre == BashDownChargeLeft)
        {
            x = originalMouseX - 10;
            y = originalMousey - 120;
        }
        else if (pre == BashDownChargeRight)
        {
            x = originalMouseX + 20;
            y = originalMousey - 120;
        }
        else if (pre == BashDiaUpChargeLeft)
        {
            x = originalMouseX + 25;
            y = originalMousey - 120;
        }
        else if (pre == BashDiaUpChargeRight)
        {
            x = originalMouseX - 20;
            y = originalMousey - 120;
        }
        else if (pre == BashDiaDownChargeLeft)
        {
            x = originalMouseX + 0;
            y = originalMousey - 120;
        }
        else if (pre == BashDiaDownChargeRight)
        {
            x = originalMouseX + 10;
            y = originalMousey - 120;
        }

        // 蓄力时间已满
        if (isBashCharging(pre) && curFrame == ActionsMap[BashUpChargeLeft].totalFrameNumber)
        {
            actionBehavior = ActionsColdTrans[pre];
            bashKey        = false;
            return;
        }
        else
        {
            if (!isBashCharging(pre))
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? BashUpChargeLeft : BashUpChargeRight;
            else
                actionBehavior = pre;

            if (upKey && leftKey)
                actionBehavior = BashDiaUpChargeLeft;
            else if (upKey && rightKey)
                actionBehavior = BashDiaUpChargeRight;
            else if (downKey && leftKey)
                actionBehavior = BashDiaDownChargeLeft;
            else if (downKey && rightKey)
                actionBehavior = BashDiaDownChargeRight;
            else if (leftKey)
                actionBehavior = BashHorChargeLeft;
            else if (rightKey)
                actionBehavior = BashHorChargeRight;
            else if (upKey)
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? BashUpChargeLeft : BashUpChargeRight;
            else if (downKey)
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? BashDownChargeLeft : BashDownChargeRight;

            return;
        }
    }
    else if (bashKey)
    {
        bashKey = false;
    }

    if (y == BottomEdge)
    {
        if (upKey)
        {
            actionBehavior = ActionsMap[actionBehavior].transform ? LookUpLeft : LookUpRight;
        }
        if (leftKey)
        {
            actionBehavior = RunFastLeft;
            if ((pre == MovingFallLeft && vy > 60 && abs(vx) > 15) ||
                (pre == LandRollLeft && curFrame != ActionsMap[LandRollLeft].totalFrameNumber))
                actionBehavior = LandRollLeft;
        }
        if (rightKey)
        {
            actionBehavior = RunFastRight;
            if ((pre == MovingFallRight && vy > 60 && abs(vx) > 15) ||
                (pre == LandRollRight && curFrame != ActionsMap[LandRollLeft].totalFrameNumber))
                actionBehavior = LandRollRight;
        }
        if (downKey)
        {
            actionBehavior = ActionsMap[actionBehavior].transform ? GetDownLeft : GetDownRight;
            if (leftKey)
                actionBehavior = GetDownWalkLeft;
            if (rightKey)
                actionBehavior = GetDownWalkRight;
            if (jumpKey)
            {
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? GetDownJumpLeftUp : GetDownJumpRightUp;
                jumpKey = false;
            }
        }
        if (jumpKey)
        {
            if (pre == RunFastLeft || pre == RunFastRight || pre == LandRollLeft ||
                pre == LandRollRight)
            {
                if (randomValue < 0.33)
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? RunJump1LeftUp : RunJump1RightUp;
                else if (randomValue < 0.66)
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? RunJump2LeftUp : RunJump2RightUp;
                else
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? RunJump3LeftUp : RunJump3RightUp;
            }
            else
            {
                if (randomValue < 0.33)
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? Jump1LeftUp : Jump1RightUp;
                else if (randomValue < 0.66)
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? Jump2LeftUp : Jump2RightUp;
                else
                    actionBehavior =
                        ActionsMap[actionBehavior].transform ? Jump3LeftUp : Jump3RightUp;
            }
            jumpKey = false;
        }
        if (dashKey)
        {
            actionBehavior = ActionsMap[actionBehavior].transform ? DashBeginLeft : DashBeginRight;
            dashKey        = false;
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
                actionBehavior = ActionsMap[actionBehavior].transform ? FallLeft : FallRight;
            }
            else if (pre == TopClimbLeft || pre == TopClimbRight)
            {
                actionBehavior =
                    ActionsMap[actionBehavior].transform ? MovingFallLeft : MovingFallRight;
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
        if (dashKey && rightKey)
        {
            actionBehavior = AirDashRight;
            dashChance--;
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
        if (dashKey && leftKey)
        {
            actionBehavior = AirDashLeft;
            dashChance--;
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
            jumpKey    = false;
            featherKey = false;
        }
        else if (dashKey)
        {
            // 空中冲刺
            if (dashChance > 0)
            {
                actionBehavior = ActionsMap[actionBehavior].transform ? AirDashLeft : AirDashRight;
                dashChance--;
            }
            dashKey    = false;
            featherKey = false;
        }
        else if (featherKey)
        {
            if (pre == FeatherAfterMouseLeft)
                pre = FeatherLeft;
            if (pre == FeatherAfterMouseRight)
                pre = FeatherRight;
            actionBehavior = ActionsMap[actionBehavior].transform ? FeatherLeft : FeatherRight;
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
                if (actionBehavior == Jump1RightUp || actionBehavior == Jump1RightDown ||
                    actionBehavior == Jump2RightUp || actionBehavior == Jump2RightDown ||
                    actionBehavior == Jump3RightUp || actionBehavior == Jump3RightDown ||
                    actionBehavior == Jump1LeftUp || actionBehavior == Jump1LeftDown ||
                    actionBehavior == Jump2LeftUp || actionBehavior == Jump2LeftDown ||
                    actionBehavior == Jump3LeftUp || actionBehavior == Jump3LeftDown ||
                    actionBehavior == GetDownJumpLeftUp || actionBehavior == GetDownJumpLeftDown ||
                    actionBehavior == GetDownJumpRightUp || actionBehavior == GetDownJumpRightDown)
                    vx = -9;
                else
                    vx = -18;
                if (actionBehavior == GetDownJumpRightUp || actionBehavior == GetDownJumpRightDown)
                {
                    actionBehavior = ActionsMirror[actionBehavior];
                    mirror         = true;
                }
                else if (actionBehavior == Jump1RightUp || actionBehavior == Jump1RightDown ||
                         actionBehavior == Jump2RightUp || actionBehavior == Jump2RightDown ||
                         actionBehavior == Jump3RightUp || actionBehavior == Jump3RightDown)
                {
                    actionBehavior = ActionsMirror[actionBehavior];
                    mirror         = true;
                }
                else if (actionBehavior == DoubleJumpRightUp)
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
                else if (actionBehavior == RunJump1RightUp || actionBehavior == RunJump1RightDown ||
                         actionBehavior == RunJump2RightUp || actionBehavior == RunJump2RightDown ||
                         actionBehavior == RunJump3RightUp || actionBehavior == RunJump3RightDown)
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
                if (actionBehavior == Jump1RightUp || actionBehavior == Jump1RightDown ||
                    actionBehavior == Jump2RightUp || actionBehavior == Jump2RightDown ||
                    actionBehavior == Jump3RightUp || actionBehavior == Jump3RightDown ||
                    actionBehavior == Jump1LeftUp || actionBehavior == Jump1LeftDown ||
                    actionBehavior == Jump2LeftUp || actionBehavior == Jump2LeftDown ||
                    actionBehavior == Jump3LeftUp || actionBehavior == Jump3LeftDown ||
                    actionBehavior == GetDownJumpLeftUp || actionBehavior == GetDownJumpLeftDown ||
                    actionBehavior == GetDownJumpRightUp || actionBehavior == GetDownJumpRightDown)
                    vx = 9;
                else
                    vx = 18;
                if (actionBehavior == GetDownJumpLeftUp || actionBehavior == GetDownJumpLeftDown)
                {
                    actionBehavior = ActionsMirror[actionBehavior];
                    mirror         = true;
                }
                else if (actionBehavior == Jump1LeftUp || actionBehavior == Jump1LeftDown ||
                         actionBehavior == Jump2LeftUp || actionBehavior == Jump2LeftDown ||
                         actionBehavior == Jump3LeftUp || actionBehavior == Jump3LeftDown)
                {
                    actionBehavior = ActionsMirror[actionBehavior];
                    mirror         = true;
                }
                else if (actionBehavior == DoubleJumpLeftUp)
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
                else if (actionBehavior == RunJump1LeftUp || actionBehavior == RunJump1LeftDown ||
                         actionBehavior == RunJump2LeftUp || actionBehavior == RunJump2LeftDown ||
                         actionBehavior == RunJump3LeftUp || actionBehavior == RunJump3LeftDown)
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
}

void Behavior::loadAction(Action act)
{
    if (actionBehavior == act)
        return;
    actionBehavior = act;
    emit PlayerLoadNewActionSignal(actionBehavior);
    emit SoundPlayerLoadNewActionSignal(actionBehavior);
}

void Behavior::loadPos(int _x, int _y)
{
    x = _x;
    y = _y;
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
    mousex = x - 32 + LeftEdge;
    mousey = y + 30 + TopEdge;
}

Action Behavior::getAction() const
{
    return actionBehavior;
}

void Behavior::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_A && !event->isAutoRepeat())
    {
        leftKey  = true;
        rightKey = false;
    }
    if (event->key() == Qt::Key_D && !event->isAutoRepeat())
    {
        rightKey = true;
        leftKey  = false;
    }
    if (event->key() == Qt::Key_W && !event->isAutoRepeat())
    {
        upKey   = true;
        downKey = false;
    }
    if (event->key() == Qt::Key_S && !event->isAutoRepeat())
    {
        downKey = true;
        upKey   = false;
    }
    if (event->key() == Qt::Key_K && !event->isAutoRepeat())
    {
        jumpKey    = true;
        featherKey = false;
    }
    if (event->key() == Qt::Key_I && !event->isAutoRepeat())
    {
        featherKey = true;
    }
    if (event->key() == Qt::Key_L && !event->isAutoRepeat())
    {
        dashKey = true;
    }
    if (event->key() == Qt::Key_J && !event->isAutoRepeat())
    {
        bashKey = true;
    }
}

void Behavior::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_A && !event->isAutoRepeat())
    {
        leftKey = false;
    }
    if (event->key() == Qt::Key_D && !event->isAutoRepeat())
    {
        rightKey = false;
    }
    if (event->key() == Qt::Key_W && !event->isAutoRepeat())
    {
        upKey = false;
    }
    if (event->key() == Qt::Key_S && !event->isAutoRepeat())
    {
        downKey = false;
    }
    if (event->key() == Qt::Key_K && !event->isAutoRepeat())
    {
        jumpKey = false;
    }
    if (event->key() == Qt::Key_I && !event->isAutoRepeat())
    {
        featherKey = false;
    }
    if (event->key() == Qt::Key_L && !event->isAutoRepeat())
    {
        dashKey = false;
    }
    if (event->key() == Qt::Key_J && !event->isAutoRepeat())
    {
        bashKey = false;
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

void Behavior::setLimitable(bool limit)
{
    limitable = limit;
}

void Behavior::timerEvent(QTimerEvent* event)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    hour                      = currentDateTime.time().hour();
}
