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
        ActionsDetail("Source/ClimbUp/ClimbUp_", 22, true, 300, 0.8, true, false);
    ActionsMap[ClimbUpRight] =
        ActionsDetail("Source/ClimbUp/ClimbUp_", 22, false, 300, 0.8, true, false);
    ActionsMap[WallStayLeft]  = ActionsDetail("Source/WallStay/ori18-", 34, true, 0, 0.3);
    ActionsMap[WallStayRight] = ActionsDetail("Source/WallStay/ori18-", 34, false, 0, 0.3);
    ActionsMap[ClimbDownLeft] =
        ActionsDetail("Source/ClimbDown/ori43-", 13, true, 100, 0.7, true, false);
    ActionsMap[ClimbDownRight] =
        ActionsDetail("Source/ClimbDown/ori43-", 13, false, 100, 0.7, true, false);
    ActionsMap[RunFastLeft] =
        ActionsDetail("Source/RunFast/ori32-", 56, true, 200, 0.2, true, false);
    ActionsMap[RunFastRight] =
        ActionsDetail("Source/RunFast/ori32-", 56, false, 200, 0.2, true, false);
    ActionsMap[RuntoStandLeft]  = ActionsDetail("Source/RuntoStand/runToIdle_", 37, true, 0, 0.0);
    ActionsMap[RuntoStandRight] = ActionsDetail("Source/RuntoStand/runToIdle_", 37, false, 0, 0.0);
    ActionsMap[RunJump1Left]    = ActionsDetail("Source/RunJump1/jump1_", 40, true, 0, 0.0);
    ActionsMap[RunJump1Right]   = ActionsDetail("Source/RunJump1/jump1_", 40, false, 0, 0.0);
    ActionsMap[RunJump2Left]    = ActionsDetail("Source/RunJump2/ori28-", 40, true, 0, 0.0);
    ActionsMap[RunJump2Right]   = ActionsDetail("Source/RunJump2/ori28-", 40, false, 0, 0.0);
    ActionsMap[DoubleJumpLeft]  = ActionsDetail("Source/DoubleJump/doubleJump_", 29, true, 0, 0.0);
    ActionsMap[DoubleJumpRight] = ActionsDetail("Source/DoubleJump/doubleJump_", 29, false, 0, 0.0);
    ActionsMap[TripleJumpLeft]  = ActionsDetail("Source/DoubleJump/doubleJump_", 29, true, 0, 0.0);
    ActionsMap[TripleJumpRight] = ActionsDetail("Source/DoubleJump/doubleJump_", 29, false, 0, 0.0);
    ActionsMap[DoubleJumptoFallLeft] =
        ActionsDetail("Source/DoubleJumptofFall/doubleJumpToFallIdle_", 5, true, 0, 0.0);
    ActionsMap[DoubleJumptoFallRight] =
        ActionsDetail("Source/DoubleJumptofFall/doubleJumpToFallIdle_", 5, false, 0, 0.0);
    ActionsMap[DoubleJumptoMovingFallLeft] =
        ActionsDetail("Source/DoubleJumptoMovinfFall/doubleJumpToFall_", 5, true, 0, 0.0);
    ActionsMap[DoubleJumptoMovingFallRight] =
        ActionsDetail("Source/DoubleJumptoMovinfFall/doubleJumpToFall_", 5, false, 0, 0.0);
    ActionsMap[FallLeft]        = ActionsDetail("Source/Fall/FallIdle_", 13, true, 0, 1.0);
    ActionsMap[FallRight]       = ActionsDetail("Source/Fall/FallIdle_", 13, false, 0, 1.0);
    ActionsMap[MovingFallLeft]  = ActionsDetail("Source/MovingFall/Fall_", 35, true, 0, 1.0);
    ActionsMap[MovingFallRight] = ActionsDetail("Source/MovingFall/Fall_", 35, false, 0, 1.0);

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
    ActionsColdTrans[TripleJumpLeft]              = DoubleJumptoFallLeft;
    ActionsColdTrans[TripleJumpRight]             = DoubleJumptoFallRight;
    ActionsColdTrans[DoubleJumptoFallLeft]        = FallLeft;
    ActionsColdTrans[DoubleJumptoFallRight]       = FallRight;
    ActionsColdTrans[DoubleJumptoMovingFallLeft]  = DoubleJumptoMovingFallLeft;
    ActionsColdTrans[DoubleJumptoMovingFallRight] = DoubleJumptoMovingFallRight;
    ActionsColdTrans[FallLeft]                    = FallLeft;
    ActionsColdTrans[FallRight]                   = FallRight;
    ActionsColdTrans[MovingFallLeft]              = DoubleJumptoMovingFallLeft;
    ActionsColdTrans[MovingFallRight]             = DoubleJumptoMovingFallRight;

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
    ActionsProbability[ClimbUpLeft]    = {{WallStayLeft, 8}, {ClimbDownLeft, -1}};
    ActionsProbability[ClimbUpRight]   = {{WallStayRight, 8}, {ClimbDownRight, -1}};
    ActionsProbability[WallStayLeft]   = {{ClimbUpLeft, -1}, {ClimbDownLeft, -1}};
    ActionsProbability[WallStayRight]  = {{ClimbUpRight, -1}, {ClimbDownRight, -1}};
    ActionsProbability[ClimbDownLeft]  = {{WallStayLeft, 8}, {ClimbUpLeft, -1}};
    ActionsProbability[ClimbDownRight] = {{WallStayRight, 8}, {ClimbUpRight, -1}};
    ActionsProbability[RunFastLeft]    = {
        {RuntoStandLeft, 10}, {RunSlowlyLeft, -1}, {RunJump1Left, 3}, {RunJump2Left, 3}};
    ActionsProbability[RunFastRight] = {
        {RuntoStandRight, 10}, {RunSlowlyRight, -1}, {RunJump1Right, 3}, {RunJump2Right, 3}};
    ActionsProbability[RuntoStandLeft]              = {{StandFacingLeft, 1}};
    ActionsProbability[RuntoStandRight]             = {{StandFacingRight, 1}};
    ActionsProbability[RunJump1Left]                = {{LandStandLeft, 1}};
    ActionsProbability[RunJump1Right]               = {{LandStandRight, 1}};
    ActionsProbability[RunJump2Left]                = {{LandStandLeft, 1}};
    ActionsProbability[RunJump2Right]               = {{LandStandRight, 1}};
    ActionsProbability[DoubleJumpLeft]              = {{DoubleJumptoFallLeft, -1},
                                                       {DoubleJumptoMovingFallLeft, -1}};
    ActionsProbability[DoubleJumpRight]             = {{DoubleJumptoFallRight, -1},
                                                       {DoubleJumptoMovingFallRight, -1}};
    ActionsProbability[TripleJumpLeft]              = {{DoubleJumptoFallLeft, -1},
                                                       {DoubleJumptoMovingFallLeft, -1}};
    ActionsProbability[TripleJumpRight]             = {{DoubleJumptoFallRight, -1},
                                                       {DoubleJumptoMovingFallRight, -1}};
    ActionsProbability[DoubleJumptoFallLeft]        = {{FallLeft, 1}};
    ActionsProbability[DoubleJumptoFallRight]       = {{FallRight, 1}};
    ActionsProbability[DoubleJumptoMovingFallLeft]  = {{MovingFallLeft, 1}};
    ActionsProbability[DoubleJumptoMovingFallRight] = {{MovingFallRight, 1}};

    // 音效加载
    SoundMap[RunSlowlyLeft] = SoundMap[RunSlowlyRight] =
        SoundsDetail("Sound/stepSound/seinFootstepsRock", 5, 2);
    SoundMap[LandStandLeft] = SoundMap[LandStandRight] =
        SoundsDetail("Sound/land/seinLandsStone", 5, 1);
    SoundMap[Jump1Left] = SoundMap[Jump1Right] = SoundMap[Jump2Left] = SoundMap[Jump2Right] =
        SoundMap[RunJump1Left] = SoundMap[RunJump1Right] = SoundMap[RunJump2Left] =
            SoundMap[RunJump2Right] = SoundsDetail("Sound/jump/seinJumpsGrass", 5, 1);
    SoundMap[ClimbUpLeft] = SoundMap[ClimbUpRight] = SoundMap[ClimbDownLeft] =
        SoundMap[ClimbDownRight] = SoundsDetail("Sound/wallClimb/seinWallClimbStepsGrass", 5, 1);
    SoundMap[RunFastLeft]        = SoundMap[RunFastRight] =
        SoundsDetail("Sound/stepSound/seinFootstepsRock", 5, 10);
    SoundMap[DoubleJumpLeft] = SoundMap[DoubleJumpRight] =
        SoundsDetail("Sound/doubleJump/seinDoubleJumps", 5, 1);
    SoundMap[TripleJumpLeft] = SoundMap[TripleJumpRight] =
        SoundsDetail("Sound/doubleJump/seinDoubleJumps", 5, 1);

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
        dy = 12;
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
        dy = -(ActionsMap[Jump1Left].totalFrameNumber + 1) + curFrame * 2;
    }
    if (action == DoubleJumpLeft || action == DoubleJumpRight || action == TripleJumpLeft ||
        action == TripleJumpRight)
    {
        dx = vx;
        dy = -(ActionsMap[DoubleJumpLeft].totalFrameNumber + 1) + curFrame * 2;
    }
    if (action == FallLeft || action == FallRight || action == DoubleJumptoFallLeft ||
        action == DoubleJumptoFallRight)
    {
        dx = 0;
        dy = vy + 4;
    }
    if (action == MovingFallLeft || action == MovingFallRight ||
        action == DoubleJumptoMovingFallLeft || action == DoubleJumptoMovingFallRight)
    {
        dx = vx;
        dy = vy + 4;
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
