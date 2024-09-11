#include "actionsdetail.h"

#include <QGuiApplication>
#include <QScreen>

using std::pair;

// 初始化映射关系
QMap<Action, ActionsDetail>        ActionsMap;
QMap<Action, SoundsDetail>         SoundMap;
QMap<Action, QMap<Action, double>> ActionsProbability;
QMap<Action, int>                  ActionsLeastTimes;
QMap<Action, Action>               ActionsMirror;
QMap<Action, Action>               ActionsColdTrans;

int ORIWIDTH = 130, ORIHEIGHT = 200;
int SCREENWIDTHFIX = -538, SCREENHEIGHTFIX = -225;
int SCREENWIDTH = 2560, SCREENHEIGHT = 1600;
int PNGWIDTH = 1152, PNGHEIGHT = 648;

void ActionsDetailLoad()
{
    // 获取主屏幕
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        // 获取屏幕分辨率
        SCREENWIDTH  = screen->size().width() + 50;
        SCREENHEIGHT = screen->size().height() + 25;
    }

    // 路径、图片数量、冷却时间、重复概率、能否被控制。控制时是否需要完整播放后转移
    ActionsMap[StandFacingLeft]  = ActionsDetail("Source/Stand/stand_", 41, true, 0, 0.3);
    ActionsMap[StandFacingRight] = ActionsDetail("Source/Stand/stand_", 41, false, 0, 0.3);
    ActionsMap[RunSlowlyLeft]    = ActionsDetail("Source/RunSlowly/ori25-", 21, true, 200, 0.8);
    ActionsMap[RunSlowlyRight]   = ActionsDetail("Source/RunSlowly/ori25-", 21, false, 200, 0.8);
    ActionsMap[Jump1Left]        = ActionsDetail("Source/Jump1/JumpIdle1_", 31, true, 100, 0.0);
    ActionsMap[Jump1Right]       = ActionsDetail("Source/Jump1/JumpIdle1_", 31, false, 100, 0.0);
    ActionsMap[Jump2Left]        = ActionsDetail("Source/Jump2/ori30-", 31, true, 100, 0.0);
    ActionsMap[Jump2Right]       = ActionsDetail("Source/Jump2/ori30-", 31, false, 100, 0.0);
    ActionsMap[LandStandLeft]    = ActionsDetail("Source/LandStand/ori12-", 41, true, 0, 0.0);
    ActionsMap[LandStandRight]   = ActionsDetail("Source/LandStand/ori12-", 41, false, 0, 0.0);
    ActionsMap[AgainstWallLeft]  = ActionsDetail("Source/AgainstWall/ori31-", 30, true, 0, 0.5);
    ActionsMap[AgainstWallRight] = ActionsDetail("Source/AgainstWall/ori31-", 30, false, 0, 0.5);
    ActionsMap[ClimbUpLeft] =
        ActionsDetail("Source/ClimbUp/ClimbUp_", 22, true, 100, 0.9, true, false);
    ActionsMap[ClimbUpRight] =
        ActionsDetail("Source/ClimbUp/ClimbUp_", 22, false, 100, 0.9, true, false);
    ActionsMap[WallStayLeft]  = ActionsDetail("Source/WallStay/ori18-", 34, true, 0, 0.3);
    ActionsMap[WallStayRight] = ActionsDetail("Source/WallStay/ori18-", 34, false, 0, 0.3);
    ActionsMap[ClimbDownLeft] =
        ActionsDetail("Source/ClimbDown/ori43-", 13, true, 100, 0.7, true, false);
    ActionsMap[ClimbDownRight] =
        ActionsDetail("Source/ClimbDown/ori43-", 13, false, 100, 0.7, true, false);
    ActionsMap[RunFastLeft] = ActionsDetail("Source/RunFast/ori32-", 56, true, 0, 0.2, true, false);
    ActionsMap[RunFastRight] =
        ActionsDetail("Source/RunFast/ori32-", 56, false, 0, 0.2, true, false);
    ActionsMap[RuntoStandLeft]  = ActionsDetail("Source/RuntoStand/runToIdle_", 37, true, 0, 0.0);
    ActionsMap[RuntoStandRight] = ActionsDetail("Source/RuntoStand/runToIdle_", 37, false, 0, 0.0);
    ActionsMap[RunJump1Left]    = ActionsDetail("Source/RunJump1/jump1_", 31, true, 0, 0.0);
    ActionsMap[RunJump1Right]   = ActionsDetail("Source/RunJump1/jump1_", 31, false, 0, 0.0);
    ActionsMap[RunJump2Left]    = ActionsDetail("Source/RunJump2/ori28-", 31, true, 0, 0.0);
    ActionsMap[RunJump2Right]   = ActionsDetail("Source/RunJump2/ori28-", 31, false, 0, 0.0);
    ActionsMap[DoubleJumpLeft]  = ActionsDetail("Source/DoubleJump/doubleJump_", 29, true, 0, 0.0);
    ActionsMap[DoubleJumpRight] = ActionsDetail("Source/DoubleJump/doubleJump_", 29, false, 0, 0.0);
    ActionsMap[DoubleJumptoFallLeft] =
        ActionsDetail("Source/DoubleJumptofFall/doubleJumpToFallIdle_", 5, true, 0, 0.0);
    ActionsMap[DoubleJumptoFallRight] =
        ActionsDetail("Source/DoubleJumptofFall/doubleJumpToFallIdle_", 5, false, 0, 0.0);
    ActionsMap[DoubleJumptoMovingFallLeft] =
        ActionsDetail("Source/DoubleJumptoMovinfFall/doubleJumpToFall_", 5, true, 0, 0.0);
    ActionsMap[DoubleJumptoMovingFallRight] =
        ActionsDetail("Source/DoubleJumptoMovinfFall/doubleJumpToFall_", 5, false, 0, 0.0);
    ActionsMap[FallLeft]          = ActionsDetail("Source/Fall/FallIdle_", 13, true, 0, 1.0);
    ActionsMap[FallRight]         = ActionsDetail("Source/Fall/FallIdle_", 13, false, 0, 1.0);
    ActionsMap[MovingFallLeft]    = ActionsDetail("Source/MovingFall/Fall_", 35, true, 0, 1.0);
    ActionsMap[MovingFallRight]   = ActionsDetail("Source/MovingFall/Fall_", 35, false, 0, 1.0);
    ActionsMap[LandRunFastLeft]   = ActionsDetail("Source/LandRunFast/ori45-", 7, true, 0, 0.0);
    ActionsMap[LandRunFastRight]  = ActionsDetail("Source/LandRunFast/ori45-", 7, false, 0, 0.0);
    ActionsMap[WallJump1Left]     = ActionsDetail("Source/WallJump1/WallJump1_", 26, true, 0, 0.0);
    ActionsMap[WallJump1Right]    = ActionsDetail("Source/WallJump1/WallJump1_", 26, false, 0, 0.0);
    ActionsMap[WallJump2Left]     = ActionsDetail("Source/WallJump2/ori40-", 30, true, 0, 0.0);
    ActionsMap[WallJump2Right]    = ActionsDetail("Source/WallJump2/ori40-", 30, false, 0, 0.0);
    ActionsMap[WallLongJump1Left] = ActionsDetail("Source/WallLongJump1/ori41-", 30, false, 0, 0.0);
    ActionsMap[WallLongJump1Right] = ActionsDetail("Source/WallLongJump1/ori41-", 30, true, 0, 0.0);
    ActionsMap[WallLongJump2Left] =
        ActionsDetail("Source/WallLongJump2/WallJumpAway2_", 30, false, 0, 0.0);
    ActionsMap[WallLongJump2Right] =
        ActionsDetail("Source/WallLongJump2/WallJumpAway2_", 30, true, 0, 0.0);
    ActionsMap[TopStayLeft]  = ActionsDetail("Source/TopStay/ori49-", 158, true, 0, 0.1);
    ActionsMap[TopStayRight] = ActionsDetail("Source/TopStay/ori49-", 158, false, 0, 0.1);
    ActionsMap[TopClimbLeft] =
        ActionsDetail("Source/TopClimb/ori50-", 22, true, 0, 0.5, true, false);
    ActionsMap[TopClimbRight] =
        ActionsDetail("Source/TopClimb/ori50-", 22, false, 0, 0.5, true, false);

    // 操作冷却时的转移映射
    ActionsColdTrans[StandFacingLeft]             = StandFacingLeft;
    ActionsColdTrans[StandFacingRight]            = StandFacingRight;
    ActionsColdTrans[RunSlowlyLeft]               = StandFacingLeft;
    ActionsColdTrans[RunSlowlyRight]              = StandFacingRight;
    ActionsColdTrans[Jump1Left]                   = LandStandLeft;
    ActionsColdTrans[Jump1Right]                  = LandStandRight;
    ActionsColdTrans[Jump2Left]                   = LandStandLeft;
    ActionsColdTrans[Jump2Right]                  = LandStandRight;
    ActionsColdTrans[LandStandLeft]               = StandFacingLeft;
    ActionsColdTrans[LandStandRight]              = StandFacingRight;
    ActionsColdTrans[AgainstWallLeft]             = AgainstWallLeft;
    ActionsColdTrans[AgainstWallRight]            = AgainstWallRight;
    ActionsColdTrans[ClimbUpLeft]                 = WallStayLeft;
    ActionsColdTrans[ClimbUpRight]                = WallStayRight;
    ActionsColdTrans[WallStayLeft]                = WallStayLeft;
    ActionsColdTrans[WallStayRight]               = WallStayRight;
    ActionsColdTrans[ClimbDownLeft]               = WallStayLeft;
    ActionsColdTrans[ClimbDownRight]              = WallStayRight;
    ActionsColdTrans[RunFastLeft]                 = RuntoStandLeft;
    ActionsColdTrans[RunFastRight]                = RuntoStandRight;
    ActionsColdTrans[RuntoStandLeft]              = StandFacingLeft;
    ActionsColdTrans[RuntoStandRight]             = StandFacingRight;
    ActionsColdTrans[RunJump1Left]                = LandStandLeft;
    ActionsColdTrans[RunJump1Right]               = LandStandRight;
    ActionsColdTrans[RunJump2Left]                = LandStandLeft;
    ActionsColdTrans[RunJump2Right]               = LandStandRight;
    ActionsColdTrans[DoubleJumpLeft]              = DoubleJumptoFallLeft;
    ActionsColdTrans[DoubleJumpRight]             = DoubleJumptoFallRight;
    ActionsColdTrans[DoubleJumptoFallLeft]        = FallLeft;
    ActionsColdTrans[DoubleJumptoFallRight]       = FallRight;
    ActionsColdTrans[DoubleJumptoMovingFallLeft]  = MovingFallLeft;
    ActionsColdTrans[DoubleJumptoMovingFallRight] = MovingFallRight;
    ActionsColdTrans[FallLeft]                    = FallLeft;
    ActionsColdTrans[FallRight]                   = FallRight;
    ActionsColdTrans[MovingFallLeft]              = MovingFallLeft;
    ActionsColdTrans[MovingFallRight]             = MovingFallRight;
    ActionsColdTrans[LandRunFastLeft]             = RunFastLeft;
    ActionsColdTrans[LandRunFastRight]            = RunFastRight;
    ActionsColdTrans[WallJump1Left]               = MovingFallLeft;
    ActionsColdTrans[WallJump1Right]              = MovingFallRight;
    ActionsColdTrans[WallJump2Left]               = MovingFallLeft;
    ActionsColdTrans[WallJump2Right]              = MovingFallRight;
    ActionsColdTrans[WallLongJump1Left]           = MovingFallRight;
    ActionsColdTrans[WallLongJump1Right]          = MovingFallLeft;
    ActionsColdTrans[WallLongJump2Left]           = MovingFallRight;
    ActionsColdTrans[WallLongJump2Right]          = MovingFallLeft;
    ActionsColdTrans[TopStayLeft]                 = TopStayLeft;
    ActionsColdTrans[TopStayRight]                = TopStayRight;
    ActionsColdTrans[TopClimbLeft]                = TopStayLeft;
    ActionsColdTrans[TopClimbRight]               = TopStayRight;

    // 状态机的转移函数
    ActionsProbability[StandFacingLeft]  = {{StandFacingRight, 1},
                                            {RunSlowlyLeft, -1},
                                            {RunSlowlyRight, -1},
                                            {Jump1Left, 1},
                                            {Jump2Left, 1},
                                            {RunFastLeft, -1},
                                            {RunFastRight, 1}};
    ActionsProbability[StandFacingRight] = {{StandFacingLeft, 1},
                                            {RunSlowlyLeft, -1},
                                            {RunSlowlyRight, -1},
                                            {Jump1Right, 1},
                                            {Jump2Right, 1},
                                            {RunFastLeft, 1},
                                            {RunFastRight, -1}};
    ActionsProbability[RunSlowlyLeft]    = {{StandFacingLeft, 20},
                                            {StandFacingRight, 1},
                                            {RunSlowlyRight, -1},
                                            {RunFastLeft, -1},
                                            {RunFastRight, 1}};
    ActionsProbability[RunSlowlyRight]   = {{StandFacingLeft, 20},
                                            {StandFacingRight, 2},
                                            {RunSlowlyLeft, -1},
                                            {RunFastLeft, 1},
                                            {RunFastRight, -1}};
    ActionsProbability[Jump1Left]        = {{LandStandLeft, 1}};
    ActionsProbability[Jump1Right]       = {{LandStandRight, 1}};
    ActionsProbability[Jump2Left]        = {{LandStandLeft, 1}};
    ActionsProbability[Jump2Right]       = {{LandStandRight, 1}};
    ActionsProbability[LandStandLeft]    = {{StandFacingLeft, 3},
                                            {StandFacingRight, 1},
                                            {RunSlowlyLeft, -1},
                                            {RunSlowlyRight, -1},
                                            {Jump1Left, 1},
                                            {Jump2Left, 1}};
    ActionsProbability[LandStandRight]   = {{StandFacingRight, 3},
                                            {StandFacingLeft, 1},
                                            {RunSlowlyLeft, -1},
                                            {RunSlowlyRight, -1},
                                            {Jump1Right, 1},
                                            {Jump2Right, 1}};
    ActionsProbability[AgainstWallLeft]  = {
        {StandFacingLeft, 5}, {StandFacingRight, 1}, {RunSlowlyRight, -1}, {ClimbUpLeft, 7}};
    ActionsProbability[AgainstWallRight] = {
        {StandFacingLeft, 1}, {StandFacingLeft, 5}, {RunSlowlyLeft, -1}, {ClimbUpRight, 7}};
    ActionsProbability[ClimbUpLeft]    = {{WallStayLeft, 8},
                                          {ClimbDownLeft, 1},
                                          {WallJump1Left, 1},
                                          {WallJump2Left, 1},
                                          {WallLongJump1Left, 2},
                                          {WallLongJump2Left, 2}};
    ActionsProbability[ClimbUpRight]   = {{WallStayRight, 8},
                                          {ClimbDownRight, 1},
                                          {WallJump1Left, 1},
                                          {WallJump2Left, 1},
                                          {WallLongJump1Left, 2},
                                          {WallLongJump2Left, 2}};
    ActionsProbability[WallStayLeft]   = {{ClimbUpLeft, -1},
                                          {ClimbDownLeft, -1},
                                          {WallJump1Left, 1},
                                          {WallJump2Left, 1},
                                          {WallLongJump1Left, 2},
                                          {WallLongJump2Left, 2},
                                          {TopClimbRight, -1}};
    ActionsProbability[WallStayRight]  = {{ClimbUpRight, -1},
                                          {ClimbDownRight, -1},
                                          {ClimbDownRight, -1},
                                          {WallJump1Right, 1},
                                          {WallJump2Right, 1},
                                          {WallLongJump1Right, 2},
                                          {WallLongJump2Right, 2},
                                          {TopClimbLeft, -1}};
    ActionsProbability[ClimbDownLeft]  = {{WallStayLeft, 8},
                                          {ClimbUpLeft, 1},
                                          {WallJump1Left, 1},
                                          {WallJump2Left, 1},
                                          {WallLongJump1Left, 2},
                                          {WallLongJump2Left, 2}};
    ActionsProbability[ClimbDownRight] = {{WallStayRight, 8},
                                          {ClimbUpRight, 1},
                                          {WallJump1Left, 1},
                                          {WallJump2Left, 1},
                                          {WallLongJump1Left, 2},
                                          {WallLongJump2Left, 2}};
    ActionsProbability[RunFastLeft]    = {
        {RuntoStandLeft, 10}, {RunSlowlyLeft, -1}, {RunJump1Left, 3}, {RunJump2Left, 3}};
    ActionsProbability[RunFastRight] = {
        {RuntoStandRight, 10}, {RunSlowlyRight, -1}, {RunJump1Right, 3}, {RunJump2Right, 3}};
    ActionsProbability[RuntoStandLeft]              = {{StandFacingLeft, 1}};
    ActionsProbability[RuntoStandRight]             = {{StandFacingRight, 1}};
    ActionsProbability[RunJump1Left]                = {{LandStandLeft, 1}, {LandRunFastLeft, 3}};
    ActionsProbability[RunJump1Right]               = {{LandStandRight, 1}, {LandRunFastRight, 3}};
    ActionsProbability[RunJump2Left]                = {{LandStandLeft, 1}, {LandRunFastLeft, 3}};
    ActionsProbability[RunJump2Right]               = {{LandStandRight, 1}, {LandRunFastRight, 3}};
    ActionsProbability[DoubleJumpLeft]              = {{DoubleJumptoFallLeft, -1},
                                                       {DoubleJumptoMovingFallLeft, -1}};
    ActionsProbability[DoubleJumpRight]             = {{DoubleJumptoFallRight, -1},
                                                       {DoubleJumptoMovingFallRight, -1}};
    ActionsProbability[DoubleJumptoFallLeft]        = {{FallLeft, 1}};
    ActionsProbability[DoubleJumptoFallRight]       = {{FallRight, 1}};
    ActionsProbability[DoubleJumptoMovingFallLeft]  = {{MovingFallLeft, 1}};
    ActionsProbability[DoubleJumptoMovingFallRight] = {{MovingFallRight, 1}};
    ActionsProbability[LandRunFastLeft]             = {{RunFastLeft, 1}};
    ActionsProbability[LandRunFastRight]            = {{RunFastRight, 1}};
    ActionsProbability[WallJump1Left]               = {{FallLeft, 1}};
    ActionsProbability[WallJump1Right]              = {{FallRight, 1}};
    ActionsProbability[WallJump2Left]               = {{FallLeft, 1}};
    ActionsProbability[WallJump2Right]              = {{FallRight, 1}};
    ActionsProbability[WallLongJump1Left]           = {{MovingFallRight, 1}};
    ActionsProbability[WallLongJump1Right]          = {{MovingFallLeft, 1}};
    ActionsProbability[WallLongJump2Left]           = {{MovingFallRight, 1}};
    ActionsProbability[WallLongJump2Right]          = {{MovingFallLeft, 1}};
    ActionsProbability[TopStayLeft]                 = {{TopClimbLeft, -1},
                                                       {TopClimbRight, 1},
                                                       {TopStayRight, 3},
                                                       {FallLeft, 5},
                                                       {FallRight, 1},
                                                       {ClimbDownLeft, -1}};
    ActionsProbability[TopStayRight]                = {{TopClimbLeft, 1},
                                                       {TopClimbRight, -1},
                                                       {TopStayLeft, 3},
                                                       {FallLeft, 1},
                                                       {FallRight, 5},
                                                       {ClimbDownRight, -1}};
    ActionsProbability[TopClimbLeft]                = {
        {TopStayLeft, 7}, {TopStayRight, 1}, {TopClimbRight, 1}, {MovingFallLeft, 5}};
    ActionsProbability[TopClimbRight] = {
        {TopStayLeft, 1}, {TopStayRight, 7}, {TopStayLeft, 1}, {MovingFallRight, 5}};

    // 音效加载
    SoundMap[RunSlowlyLeft] = SoundMap[RunSlowlyRight] =
        SoundsDetail("Sound/stepSound/seinFootstepsRock", 5, 2);
    SoundMap[LandStandLeft] = SoundMap[LandStandRight] = SoundMap[LandRunFastLeft] =
        SoundMap[LandRunFastRight] = SoundsDetail("Sound/land/seinLandsStone", 5, 1);
    SoundMap[Jump1Left] = SoundMap[Jump1Right] = SoundMap[Jump2Left] = SoundMap[Jump2Right] =
        SoundMap[RunJump1Left] = SoundMap[RunJump1Right] = SoundMap[RunJump2Left] =
            SoundMap[RunJump2Right] = SoundsDetail("Sound/jump/seinJumpsGrass", 5, 1);
    SoundMap[ClimbUpLeft] = SoundMap[ClimbUpRight] = SoundMap[ClimbDownLeft] =
        SoundMap[ClimbDownRight] = SoundMap[TopClimbLeft] = SoundMap[TopClimbRight] =
            SoundsDetail("Sound/wallClimb/seinWallClimbStepsGrass", 5, 1);
    SoundMap[RunFastLeft] = SoundMap[RunFastRight] =
        SoundsDetail("Sound/stepSound/seinFootstepsRock", 5, 10);
    SoundMap[DoubleJumpLeft] = SoundMap[DoubleJumpRight] =
        SoundsDetail("Sound/doubleJump/seinDoubleJumps", 5, 1);
    SoundMap[WallJump1Left] = SoundMap[WallJump1Right] = SoundMap[WallJump2Left] =
        SoundMap[WallJump2Right] = SoundMap[WallLongJump1Left] = SoundMap[WallLongJump1Right] =
            SoundMap[WallLongJump2Left]                        = SoundMap[WallLongJump2Left] =
                SoundsDetail("Sound/wallJump/seinWallJumps", 5, 1);

    // 镜像动作初始化
    for (int i = 0; i < int(None); i++)
    {
        if (i % 2 == 0)
        {
            ActionsMirror[Action(i)] = Action(i + 1);
        }
        else
        {
            ActionsMirror[Action(i)] = Action(i - 1);
        }
    }
}

// 位移函数
pair<int, int>
ActionsMovement(Action action, int x, int y, int vx, int vy, int curFrame, long long time)
{
    int dx = 0, dy = 0;
    if (action == RunSlowlyLeft || action == RunSlowlyRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 8;
        dy = 0;
    }
    if (action == Jump1Left || action == Jump1Right || action == Jump2Left || action == Jump2Right)
    {
        dx = 0;
        dy = -(ActionsMap[Jump1Left].totalFrameNumber + 1) + curFrame * 2;
    }
    if (action == ClimbUpLeft || action == ClimbUpRight)
    {
        dx = 0;
        dy = -10;
    }
    if (action == ClimbDownLeft || action == ClimbDownRight)
    {
        dx = 0;
        dy = 14;
    }
    if (action == RunFastLeft || action == RunFastRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 18;
        dy = 0;
    }
    if (action == RunJump1Left || action == RunJump1Right || action == RunJump2Left ||
        action == RunJump2Right)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 18;
        dy = -(ActionsMap[action].totalFrameNumber + 1) + curFrame * 2;
    }
    if (action == DoubleJumpLeft || action == DoubleJumpRight)
    {
        dx = vx;
        dy = -(ActionsMap[action].totalFrameNumber + 1) + curFrame * 2;
    }
    if (action == FallLeft || action == FallRight || action == DoubleJumptoFallLeft ||
        action == DoubleJumptoFallRight)
    {
        dx = 0;
        dy = vy + 2;
    }
    if (action == MovingFallLeft || action == MovingFallRight ||
        action == DoubleJumptoMovingFallLeft || action == DoubleJumptoMovingFallRight)
    {
        dx = vx;
        dy = vy + 2;
    }
    if (action == LandRunFastLeft || action == LandRunFastLeft)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 18;
        dy = 0;
    }
    if (action == WallJump1Left || action == WallJump1Right || action == WallJump2Left ||
        action == WallJump2Right)
    {
        if (action == WallJump1Left || action == WallJump2Left)
        {
            dx = (ActionsMap[action].totalFrameNumber - curFrame + 5) / 2;
        }
        else
        {
            dx = -(ActionsMap[action].totalFrameNumber - curFrame + 5) / 2;
        }
        dy = -18 + time * 2;
    }
    if (action == WallLongJump1Left || action == WallLongJump1Right ||
        action == WallLongJump2Left || action == WallLongJump2Right)
    {
        if (action == WallLongJump1Left || action == WallLongJump2Left)
        {
            dx = 22;
        }
        else
        {
            dx = -22;
        }
        dy = -18 + time * 2;
    }
    if (action == TopClimbLeft || action == TopClimbRight)
    {
        dx = int(action) % 2 == 0 ? -10 : 10;
        dy = 0;
    }

    return {dx, dy};
}

int ActionsDX(Action action, int x, int y, int vx, int vy, int curFrame, long long time)
{
    return ActionsMovement(action, x, y, vx, vy, curFrame, time).first;
}
int ActionsDY(Action action, int x, int y, int vx, int vy, int curFrame, long long time)
{
    return ActionsMovement(action, x, y, vx, vy, curFrame, time).second;
}
