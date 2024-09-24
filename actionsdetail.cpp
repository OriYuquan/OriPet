#include "actionsdetail.h"

#include <QCursor>
#include <QDataStream>
#include <QDebug>
#include <QFile>
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
QSet<Action>                       ActionLimit;

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
        // 从设置文件里读取基准数据
        QFile file("baseMark.data");
        int   value = 25;
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream in(&file);
            in >> value;  // 从文件中读取一个整数
        }

        // 关闭文件
        file.close();

        // 获取屏幕分辨率
        SCREENWIDTH  = screen->size().width() + 52;
        SCREENHEIGHT = screen->size().height() + value;
    }

    // 路径、图片数量、冷却时间、重复概率、能否被控制。控制时是否需要完整播放后转移
    ActionsMap[StandFacingLeft]  = ActionsDetail("Source/Stand/stand_", 41, true, 0, 0.3);
    ActionsMap[StandFacingRight] = ActionsDetail("Source/Stand/stand_", 41, false, 0, 0.3);
    ActionsMap[RunSlowlyLeft]    = ActionsDetail("Source/RunSlowly/ori25-", 21, true, 200, 0.8);
    ActionsMap[RunSlowlyRight]   = ActionsDetail("Source/RunSlowly/ori25-", 21, false, 200, 0.8);

    ActionsMap[Jump1LeftUp]    = ActionsDetail("Source/Jump1Up/JumpIdle1_", 16, true, 0, 0.0);
    ActionsMap[Jump1RightUp]   = ActionsDetail("Source/Jump1Up/JumpIdle1_", 16, false, 0, 0.0);
    ActionsMap[Jump1LeftDown]  = ActionsDetail("Source/Jump1Down/JumpIdle1_", 15, true, 0, 0.0);
    ActionsMap[Jump1RightDown] = ActionsDetail("Source/Jump1Down/JumpIdle1_", 15, false, 0, 0.0);

    ActionsMap[Jump2LeftUp]    = ActionsDetail("Source/Jump2Up/ori30-", 16, true, 0, 0.0);
    ActionsMap[Jump2RightUp]   = ActionsDetail("Source/Jump2Up/ori30-", 16, false, 0, 0.0);
    ActionsMap[Jump2LeftDown]  = ActionsDetail("Source/Jump2Down/ori30-", 15, true, 0, 0.0);
    ActionsMap[Jump2RightDown] = ActionsDetail("Source/Jump2Down/ori30-", 15, false, 0, 0.0);

    ActionsMap[LandStandLeft]    = ActionsDetail("Source/LandStand/ori12-", 41, true, 0, 0.0);
    ActionsMap[LandStandRight]   = ActionsDetail("Source/LandStand/ori12-", 41, false, 0, 0.0);
    ActionsMap[AgainstWallLeft]  = ActionsDetail("Source/AgainstWall/ori31-", 30, true, 0, 0.5);
    ActionsMap[AgainstWallRight] = ActionsDetail("Source/AgainstWall/ori31-", 30, false, 0, 0.5);
    ActionsMap[ClimbUpLeft] =
        ActionsDetail("Source/ClimbUp/ClimbUp_", 22, true, 100, 0.8, true, false);
    ActionsMap[ClimbUpRight] =
        ActionsDetail("Source/ClimbUp/ClimbUp_", 22, false, 100, 0.8, true, false);
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

    ActionsMap[RunJump1LeftUp]    = ActionsDetail("Source/RunJump1Up/jump1_", 16, true, 0, 0.0);
    ActionsMap[RunJump1RightUp]   = ActionsDetail("Source/RunJump1Up/jump1_", 16, false, 0, 0.0);
    ActionsMap[RunJump1LeftDown]  = ActionsDetail("Source/RunJump1Down/jump1_", 15, true, 0, 0.0);
    ActionsMap[RunJump1RightDown] = ActionsDetail("Source/RunJump1Down/jump1_", 15, false, 0, 0.0);

    ActionsMap[RunJump2LeftUp]    = ActionsDetail("Source/RunJump2Up/ori28-", 16, true, 0, 0.0);
    ActionsMap[RunJump2RightUp]   = ActionsDetail("Source/RunJump2Up/ori28-", 16, false, 0, 0.0);
    ActionsMap[RunJump2LeftDown]  = ActionsDetail("Source/RunJump2Down/ori28-", 15, true, 0, 0.0);
    ActionsMap[RunJump2RightDown] = ActionsDetail("Source/RunJump2Down/ori28-", 15, false, 0, 0.0);

    ActionsMap[DoubleJumpLeftUp] =
        ActionsDetail("Source/DoubleJumpUp/doubleJump_", 15, true, 0, 0.0);
    ActionsMap[DoubleJumpRightUp] =
        ActionsDetail("Source/DoubleJumpUp/doubleJump_", 15, false, 0, 0.0);
    ActionsMap[DoubleJumpLeftDown] =
        ActionsDetail("Source/DoubleJumpDown/doubleJump_", 14, true, 0, 0.0);
    ActionsMap[DoubleJumpRightDown] =
        ActionsDetail("Source/DoubleJumpDown/doubleJump_", 14, false, 0, 0.0);

    ActionsMap[DoubleJumptoFallLeft] =
        ActionsDetail("Source/DoubleJumptofFall/doubleJumpToFallIdle_", 5, true, 0, 0.0);
    ActionsMap[DoubleJumptoFallRight] =
        ActionsDetail("Source/DoubleJumptofFall/doubleJumpToFallIdle_", 5, false, 0, 0.0);
    ActionsMap[DoubleJumptoMovingFallLeft] =
        ActionsDetail("Source/DoubleJumptoMovinfFall/doubleJumpToFall_", 5, true, 0, 0.0);
    ActionsMap[DoubleJumptoMovingFallRight] =
        ActionsDetail("Source/DoubleJumptoMovinfFall/doubleJumpToFall_", 5, false, 0, 0.0);

    ActionsMap[FallLeft]        = ActionsDetail("Source/Fall/FallIdle_", 13, true, 0, -0.93);
    ActionsMap[FallRight]       = ActionsDetail("Source/Fall/FallIdle_", 13, false, 0, -0.93);
    ActionsMap[MovingFallLeft]  = ActionsDetail("Source/MovingFall/Fall_", 35, true, 0, -0.93);
    ActionsMap[MovingFallRight] = ActionsDetail("Source/MovingFall/Fall_", 35, false, 0, -0.93);

    ActionsMap[LandRunFastLeft]   = ActionsDetail("Source/LandRunFast/ori45-", 7, true, 0, 0.0);
    ActionsMap[LandRunFastRight]  = ActionsDetail("Source/LandRunFast/ori45-", 7, false, 0, 0.0);
    ActionsMap[WallJump1Left]     = ActionsDetail("Source/WallJump1/WallJump1_", 26, true, 0, 0.0);
    ActionsMap[WallJump1Right]    = ActionsDetail("Source/WallJump1/WallJump1_", 26, false, 0, 0.0);
    ActionsMap[WallJump2Left]     = ActionsDetail("Source/WallJump2/WallJump2_", 36, true, 0, 0.0);
    ActionsMap[WallJump2Right]    = ActionsDetail("Source/WallJump2/WallJump2_", 36, false, 0, 0.0);
    ActionsMap[WallLongJump1Left] = ActionsDetail("Source/WallLongJump1/ori41-", 30, false, 0, 0.0);
    ActionsMap[WallLongJump1Right] = ActionsDetail("Source/WallLongJump1/ori41-", 30, true, 0, 0.0);
    ActionsMap[WallLongJump2Left] =
        ActionsDetail("Source/WallLongJump2/WallJumpAway2_", 30, false, 0, 0.0);
    ActionsMap[WallLongJump2Right] =
        ActionsDetail("Source/WallLongJump2/WallJumpAway2_", 30, true, 0, 0.0);

    ActionsMap[TopStayLeft]  = ActionsDetail("Source/TopStay/ori49-", 158, true, 0, 0.1);
    ActionsMap[TopStayRight] = ActionsDetail("Source/TopStay/ori49-", 158, false, 0, 0.1);
    ActionsMap[TopClimbLeft] =
        ActionsDetail("Source/TopClimb/ori50-", 22, true, 0, 0.7, true, false);
    ActionsMap[TopClimbRight] =
        ActionsDetail("Source/TopClimb/ori50-", 22, false, 0, 0.7, true, false);

    ActionsMap[FeatherLeft] = ActionsDetail("Source/Feather/ori16-", 37, true, 0, 0.8, true, false);
    ActionsMap[FeatherRight] =
        ActionsDetail("Source/Feather/ori16-", 37, false, 0, 0.8, true, false);
    ActionsMap[MovingFeatherLeft] =
        ActionsDetail("Source/MovingFeather/ori17-", 21, true, 0, 0.8, true, false);
    ActionsMap[MovingFeatherRight] =
        ActionsDetail("Source/MovingFeather/ori17-", 21, false, 0, 0.8, true, false);

    ActionsMap[DashBeginLeft]  = ActionsDetail("Source/DashBegin/Dash", 11, true, 0, 0.0, false);
    ActionsMap[DashBeginRight] = ActionsDetail("Source/DashBegin/Dash", 11, false, 0, 0.0, false);
    ActionsMap[DashEndLeft]    = ActionsDetail("Source/DashEnd/Dash", 3, true, 0, 0.0, true, false);
    ActionsMap[DashEndRight] = ActionsDetail("Source/DashEnd/Dash", 3, false, 0, 0.0, true, false);

    ActionsMap[WallJumpClimbLeft] =
        ActionsDetail("Source/WallJumpClimb/WallClimbJumpToIdle_", 11, true, 0, 0.6, false);
    ActionsMap[WallJumpClimbRight] =
        ActionsDetail("Source/WallJumpClimb/WallClimbJumpToIdle_", 11, false, 0, 0.6, false);

    ActionsMap[MouseHoldLeft] =
        ActionsDetail("Source/MouseHoid/ori19-", 50, true, 0, 0.0, true, false);
    ActionsMap[MouseHoldRight] =
        ActionsDetail("Source/MouseHoid/ori19-", 50, false, 0, 0.0, true, false);

    ActionsMap[FeatherAfterMouseLeft]  = ActionsDetail("Source/Feather/ori16-", 37, true, 0, 0.8);
    ActionsMap[FeatherAfterMouseRight] = ActionsDetail("Source/Feather/ori16-", 37, false, 0, 0.8);

    ActionsMap[AirDashLeft]  = ActionsDetail("Source/AirDash/AirDash", 13, true, 0, 0.0, false);
    ActionsMap[AirDashRight] = ActionsDetail("Source/AirDash/AirDash", 13, false, 0, 0.0, false);
    ActionsMap[AirDashtoFallLeft] =
        ActionsDetail("Source/AirDashtoFall/AirDashToFall_", 5, true, 0, 0.0);
    ActionsMap[AirDashtoFallRight] =
        ActionsDetail("Source/AirDashtoFall/AirDashToFall_", 5, false, 0, 0.0);

    ActionsMap[GetDownLeft] =
        ActionsDetail("Source/GetDown/ori2-", 29, true, 300, 0.6, true, false);
    ActionsMap[GetDownRight] =
        ActionsDetail("Source/GetDown/ori2-", 29, false, 300, 0.6, true, false);
    ActionsMap[GetDownWalkLeft] =
        ActionsDetail("Source/GetDownWalk/ori3-", 36, true, 0, 0.8, true, false);
    ActionsMap[GetDownWalkRight] =
        ActionsDetail("Source/GetDownWalk/ori3-", 36, false, 0, 0.8, true, false);

    ActionsMap[RunLeft]  = ActionsDetail("Source/Run/ori26-", 21, true, 1000, 0.5);
    ActionsMap[RunRight] = ActionsDetail("Source/Run/ori26-", 21, false, 1000, 0.5);

    ActionsMap[WalkLeft]  = ActionsDetail("Source/Walk/walk_", 30, true, 100, 0.8);
    ActionsMap[WalkRight] = ActionsDetail("Source/Walk/walk_", 30, false, 100, 0.8);

    ActionsMap[BashUpChargeLeft] =
        ActionsDetail("Source/BashUpCharge/BashChargeUpwards_", 90, true, 0, 0.0, true, false);
    ActionsMap[BashUpChargeRight] =
        ActionsDetail("Source/BashUpCharge/BashChargeUpwards_", 90, false, 0, 0.0, true, false);
    ActionsMap[BashUpLeft]  = ActionsDetail("Source/BashUp/BashUpwards_", 30, true, 0, 0.0, false);
    ActionsMap[BashUpRight] = ActionsDetail("Source/BashUp/BashUpwards_", 30, false, 0, 0.0, false);

    ActionsMap[BashHorChargeLeft] =
        ActionsDetail("Source/BashHorCharge/BashChargeHorizontal_", 90, true, 0, 0.0, true, false);
    ActionsMap[BashHorChargeRight] =
        ActionsDetail("Source/BashHorCharge/BashChargeHorizontal_", 90, false, 0, 0.0, true, false);
    ActionsMap[BashHorLeft] =
        ActionsDetail("Source/BashHor/BashHorizontal_", 30, true, 0, 0.0, false);
    ActionsMap[BashHorRight] =
        ActionsDetail("Source/BashHor/BashHorizontal_", 30, false, 0, 0.0, false);

    ActionsMap[BashDownChargeLeft] =
        ActionsDetail("Source/BashDownCharge/BashChargeDownwards_", 90, true, 0, 0.0, true, false);
    ActionsMap[BashDownChargeRight] =
        ActionsDetail("Source/BashDownCharge/BashChargeDownwards_", 90, false, 0, 0.0, true, false);
    ActionsMap[BashDownLeft] =
        ActionsDetail("Source/BashDown/BashDownwards_", 30, true, 0, 0.0, false);
    ActionsMap[BashDownRight] =
        ActionsDetail("Source/BashDown/BashDownwards_", 30, false, 0, 0.0, false);

    ActionsMap[BashDiaUpChargeLeft] = ActionsDetail(
        "Source/BashDiaUpCharge/BashChargeDiagonalup_", 90, true, 0, 0.0, true, false);
    ActionsMap[BashDiaUpChargeRight] = ActionsDetail(
        "Source/BashDiaUpCharge/BashChargeDiagonalup_", 90, false, 0, 0.0, true, false);
    ActionsMap[BashDiaUpLeft] =
        ActionsDetail("Source/BashDiaUp/BashDiagonalUp_", 30, true, 0, 0.0, false);
    ActionsMap[BashDiaUpRight] =
        ActionsDetail("Source/BashDiaUp/BashDiagonalUp_", 30, false, 0, 0.0, false);

    ActionsMap[BashDiaDownChargeLeft] = ActionsDetail(
        "Source/BashDiaDownCharge/BashChargeDiagonalDown_", 90, true, 0, 0.0, true, false);
    ActionsMap[BashDiaDownChargeRight] = ActionsDetail(
        "Source/BashDiaDownCharge/BashChargeDiagonalDown_", 90, false, 0, 0.0, true, false);
    ActionsMap[BashDiaDownLeft] =
        ActionsDetail("Source/BashDiaDown/BashDiagonalDown_", 30, true, 0, 0.0, false);
    ActionsMap[BashDiaDownRight] =
        ActionsDetail("Source/BashDiaDown/BashDiagonalDown_", 30, false, 0, 0.0, false);

    ActionsMap[QuitStepLeft]  = ActionsDetail("Source/QuitStep/ori38-", 96, true, 0, 0.8);
    ActionsMap[QuitStepRight] = ActionsDetail("Source/QuitStep/ori38-", 96, false, 0, 0.8);

    ActionsMap[LookUpListenLeft] = ActionsDetail("Source/LookUpListen/ori21-", 50, true, 700, 0.85);
    ActionsMap[LookUpListenRight] =
        ActionsDetail("Source/LookUpListen/ori21-", 50, false, 700, 0.85);
    ActionsMap[StandtoLookUpListenLeft] =
        ActionsDetail("Source/StandtoLookUpListen/ori24-", 21, true, 0, 0.0);
    ActionsMap[StandtoLookUpListenRight] =
        ActionsDetail("Source/StandtoLookUpListen/ori24-", 21, false, 0, 0.0);

    ActionsMap[LookUpLeft] = ActionsDetail("Source/LookUp/ori22-", 30, true, 200, 0.9, true, false);
    ActionsMap[LookUpRight] =
        ActionsDetail("Source/LookUp/ori22-", 30, false, 200, 0.9, true, false);

    ActionsMap[YawnLeft]  = ActionsDetail("Source/Yawn/ori44-", 68, true, 1000, 0.0);
    ActionsMap[YawnRight] = ActionsDetail("Source/Yawn/ori44-", 68, false, 1000, 0.0);

    ActionsMap[ActCuteLeft]  = ActionsDetail("Source/ActCute/Cute1Idle_", 105, true, 0, 0.5);
    ActionsMap[ActCuteRight] = ActionsDetail("Source/ActCute/Cute1Idle_", 105, false, 0, 0.5);
    ActionsMap[StandLefttoActCuteRight] =
        ActionsDetail("Source/StandtoActCute/StandToCute1_", 85, true, 2000, 0.0);
    ActionsMap[StandRighttoActCuteLeft] =
        ActionsDetail("Source/StandtoActCute/StandToCute1_", 85, false, 2000, 0.0);

    // 动作限制集合
    ActionLimit = {Jump1LeftUp,       Jump1RightUp,

                   Jump2LeftUp,       Jump2RightUp,

                   RunJump1LeftUp,    RunJump1RightUp,

                   RunJump2LeftUp,    RunJump2RightUp,

                   DoubleJumpLeftUp,  DoubleJumpRightUp,

                   LandRunFastLeft,   LandRunFastRight,

                   WallJump1Left,     WallJump1Right,     WallJump2Left,     WallJump2Right,
                   WallLongJump1Left, WallLongJump1Right, WallLongJump2Left, WallLongJump2Right,

                   DashBeginLeft,     DashBeginRight,     AirDashLeft,       AirDashRight,

                   WallJumpClimbLeft, WallJumpClimbRight};

    // 操作冷却时的转移映射
    ActionsColdTrans[StandFacingLeft]  = StandFacingLeft;
    ActionsColdTrans[StandFacingRight] = StandFacingRight;
    ActionsColdTrans[RunSlowlyLeft]    = StandFacingLeft;
    ActionsColdTrans[RunSlowlyRight]   = StandFacingRight;

    ActionsColdTrans[Jump1LeftUp]    = Jump1LeftDown;
    ActionsColdTrans[Jump1RightUp]   = Jump1RightDown;
    ActionsColdTrans[Jump1LeftDown]  = LandStandLeft;
    ActionsColdTrans[Jump1RightDown] = LandStandRight;

    ActionsColdTrans[Jump2LeftUp]    = Jump2LeftDown;
    ActionsColdTrans[Jump2RightUp]   = Jump2RightDown;
    ActionsColdTrans[Jump2LeftDown]  = LandStandLeft;
    ActionsColdTrans[Jump2RightDown] = LandStandRight;

    ActionsColdTrans[LandStandLeft]    = StandFacingLeft;
    ActionsColdTrans[LandStandRight]   = StandFacingRight;
    ActionsColdTrans[AgainstWallLeft]  = AgainstWallLeft;
    ActionsColdTrans[AgainstWallRight] = AgainstWallRight;
    ActionsColdTrans[ClimbUpLeft]      = WallStayLeft;
    ActionsColdTrans[ClimbUpRight]     = WallStayRight;
    ActionsColdTrans[WallStayLeft]     = WallStayLeft;
    ActionsColdTrans[WallStayRight]    = WallStayRight;
    ActionsColdTrans[ClimbDownLeft]    = WallStayLeft;
    ActionsColdTrans[ClimbDownRight]   = WallStayRight;
    ActionsColdTrans[RunFastLeft]      = RuntoStandLeft;
    ActionsColdTrans[RunFastRight]     = RuntoStandRight;
    ActionsColdTrans[RuntoStandLeft]   = StandFacingLeft;
    ActionsColdTrans[RuntoStandRight]  = StandFacingRight;

    ActionsColdTrans[RunJump1LeftUp]    = RunJump1LeftDown;
    ActionsColdTrans[RunJump1RightUp]   = RunJump1RightDown;
    ActionsColdTrans[RunJump1LeftDown]  = LandStandLeft;
    ActionsColdTrans[RunJump1RightDown] = LandStandRight;

    ActionsColdTrans[RunJump2LeftUp]    = RunJump2LeftDown;
    ActionsColdTrans[RunJump2RightUp]   = RunJump2RightDown;
    ActionsColdTrans[RunJump2LeftDown]  = LandStandLeft;
    ActionsColdTrans[RunJump2RightDown] = LandStandRight;

    ActionsColdTrans[DoubleJumpLeftUp]    = DoubleJumpLeftDown;
    ActionsColdTrans[DoubleJumpRightUp]   = DoubleJumpRightDown;
    ActionsColdTrans[DoubleJumpLeftDown]  = DoubleJumptoFallLeft;
    ActionsColdTrans[DoubleJumpRightDown] = DoubleJumptoFallRight;

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

    ActionsColdTrans[WallJump1Left]      = FallLeft;
    ActionsColdTrans[WallJump1Right]     = FallRight;
    ActionsColdTrans[WallJump2Left]      = FallLeft;
    ActionsColdTrans[WallJump2Right]     = FallRight;
    ActionsColdTrans[WallLongJump1Left]  = MovingFallRight;
    ActionsColdTrans[WallLongJump1Right] = MovingFallLeft;
    ActionsColdTrans[WallLongJump2Left]  = MovingFallRight;
    ActionsColdTrans[WallLongJump2Right] = MovingFallLeft;

    ActionsColdTrans[TopStayLeft]        = TopStayLeft;
    ActionsColdTrans[TopStayRight]       = TopStayRight;
    ActionsColdTrans[TopClimbLeft]       = TopStayLeft;
    ActionsColdTrans[TopClimbRight]      = TopStayRight;
    ActionsColdTrans[FeatherLeft]        = FallLeft;
    ActionsColdTrans[FeatherRight]       = FallRight;
    ActionsColdTrans[MovingFeatherLeft]  = MovingFallLeft;
    ActionsColdTrans[MovingFeatherRight] = MovingFallRight;

    ActionsColdTrans[DashBeginLeft]  = DashEndLeft;
    ActionsColdTrans[DashBeginRight] = DashEndRight;
    ActionsColdTrans[DashEndLeft]    = StandFacingLeft;
    ActionsColdTrans[DashEndRight]   = StandFacingRight;

    ActionsColdTrans[WallJumpClimbLeft]  = WallStayLeft;
    ActionsColdTrans[WallJumpClimbRight] = WallStayRight;

    ActionsColdTrans[MouseHoldLeft]          = FeatherAfterMouseLeft;
    ActionsColdTrans[MouseHoldRight]         = FeatherAfterMouseRight;
    ActionsColdTrans[FeatherAfterMouseLeft]  = FeatherAfterMouseLeft;
    ActionsColdTrans[FeatherAfterMouseRight] = FeatherAfterMouseRight;

    ActionsColdTrans[AirDashLeft]        = AirDashtoFallLeft;
    ActionsColdTrans[AirDashRight]       = AirDashtoFallRight;
    ActionsColdTrans[AirDashtoFallLeft]  = MovingFallLeft;
    ActionsColdTrans[AirDashtoFallRight] = MovingFallRight;

    ActionsColdTrans[GetDownLeft]      = StandFacingLeft;
    ActionsColdTrans[GetDownRight]     = StandFacingRight;
    ActionsColdTrans[GetDownWalkLeft]  = StandFacingLeft;
    ActionsColdTrans[GetDownWalkRight] = StandFacingRight;

    ActionsColdTrans[RunLeft]  = StandFacingLeft;
    ActionsColdTrans[RunRight] = StandFacingRight;

    ActionsColdTrans[WalkLeft]  = StandFacingLeft;
    ActionsColdTrans[WalkRight] = StandFacingRight;

    ActionsColdTrans[BashUpChargeLeft]  = BashUpLeft;
    ActionsColdTrans[BashUpChargeRight] = BashUpRight;
    ActionsColdTrans[BashUpLeft]        = FallLeft;
    ActionsColdTrans[BashUpRight]       = FallRight;

    ActionsColdTrans[BashHorChargeLeft]  = BashHorLeft;
    ActionsColdTrans[BashHorChargeRight] = BashHorRight;
    ActionsColdTrans[BashHorLeft]        = FallLeft;
    ActionsColdTrans[BashHorRight]       = FallRight;

    ActionsColdTrans[BashDownChargeLeft]  = BashDownLeft;
    ActionsColdTrans[BashDownChargeRight] = BashDownRight;
    ActionsColdTrans[BashDownLeft]        = FallLeft;
    ActionsColdTrans[BashDownRight]       = FallRight;

    ActionsColdTrans[BashDiaUpChargeLeft]  = BashDiaUpLeft;
    ActionsColdTrans[BashDiaUpChargeRight] = BashDiaUpRight;
    ActionsColdTrans[BashDiaUpLeft]        = FallLeft;
    ActionsColdTrans[BashDiaUpRight]       = FallRight;

    ActionsColdTrans[BashDiaDownChargeLeft]  = BashDiaDownLeft;
    ActionsColdTrans[BashDiaDownChargeRight] = BashDiaDownRight;
    ActionsColdTrans[BashDiaDownLeft]        = FallLeft;
    ActionsColdTrans[BashDiaDownRight]       = FallRight;

    ActionsColdTrans[QuitStepLeft]  = StandFacingLeft;
    ActionsColdTrans[QuitStepRight] = StandFacingRight;

    ActionsColdTrans[LookUpListenLeft]         = StandFacingLeft;
    ActionsColdTrans[LookUpListenRight]        = StandFacingRight;
    ActionsColdTrans[StandtoLookUpListenLeft]  = StandFacingLeft;
    ActionsColdTrans[StandtoLookUpListenRight] = StandFacingRight;

    ActionsColdTrans[LookUpLeft]  = StandFacingLeft;
    ActionsColdTrans[LookUpRight] = StandFacingRight;

    ActionsColdTrans[YawnLeft]  = StandFacingLeft;
    ActionsColdTrans[YawnRight] = StandFacingRight;

    ActionsColdTrans[ActCuteLeft]             = ActCuteLeft;
    ActionsColdTrans[ActCuteRight]            = ActCuteRight;
    ActionsColdTrans[StandLefttoActCuteRight] = ActCuteRight;
    ActionsColdTrans[StandRighttoActCuteLeft] = ActCuteLeft;

    // 状态机的转移函数
    ActionsProbability[StandFacingLeft]  = {{StandFacingRight, 1},
                                            {RunSlowlyLeft, -1},
                                            {RunSlowlyRight, 1},
                                            {Jump1LeftUp, 3},
                                            {Jump2LeftUp, 3},
                                            {RunFastLeft, -1},
                                            {RunFastRight, 1},
                                            {GetDownLeft, 3},
                                            {RunLeft, -1},
                                            {WalkLeft, -1},
                                            {WalkRight, -1},
                                            {QuitStepLeft, 2},
                                            {StandtoLookUpListenLeft, -1},
                                            {LookUpLeft, -1},
                                            {YawnLeft, -1},
                                            {StandLefttoActCuteRight, 5}};
    ActionsProbability[StandFacingRight] = {{StandFacingLeft, 1},
                                            {RunSlowlyLeft, 1},
                                            {RunSlowlyRight, -1},
                                            {Jump1RightUp, 3},
                                            {Jump2RightUp, 3},
                                            {RunFastLeft, 1},
                                            {RunFastRight, -1},
                                            {GetDownRight, 3},
                                            {RunRight, -1},
                                            {WalkLeft, -1},
                                            {WalkRight, -1},
                                            {QuitStepRight, 2},
                                            {StandtoLookUpListenRight, -1},
                                            {LookUpRight, -1},
                                            {YawnRight, -1},
                                            {StandRighttoActCuteLeft, 5}};
    ActionsProbability[RunSlowlyLeft]    = {{StandFacingLeft, 20},
                                            {StandFacingRight, 1},
                                            {RunSlowlyRight, -1},
                                            {RunFastLeft, -1},
                                            {RunFastRight, 1},
                                            {RunLeft, -1},
                                            {WalkLeft, 2}};
    ActionsProbability[RunSlowlyRight]   = {{StandFacingLeft, 1},
                                            {StandFacingRight, 20},
                                            {RunSlowlyLeft, -1},
                                            {RunFastLeft, 1},
                                            {RunFastRight, -1},
                                            {RunRight, -1},
                                            {WalkRight, 2}};

    ActionsProbability[Jump1LeftUp] = {
        {Jump1LeftDown, 6}, {DoubleJumpLeftUp, -1}, {FeatherLeft, 1}, {AirDashLeft, -1}};
    ActionsProbability[Jump1RightUp] = {
        {Jump1RightDown, 6}, {DoubleJumpRightUp, -1}, {FeatherRight, 1}, {AirDashRight, -1}};
    ActionsProbability[Jump1LeftDown]  = {{LandStandLeft, 1}};
    ActionsProbability[Jump1RightDown] = {{LandStandRight, 1}};

    ActionsProbability[Jump2LeftUp] = {
        {Jump2LeftDown, 6}, {DoubleJumpLeftUp, -1}, {FeatherLeft, 1}, {AirDashLeft, -1}};
    ActionsProbability[Jump2RightUp] = {
        {Jump2RightDown, 6}, {DoubleJumpRightUp, -1}, {FeatherRight, 1}, {AirDashRight, -1}};
    ActionsProbability[Jump2LeftDown]  = {{LandStandLeft, 1}};
    ActionsProbability[Jump2RightDown] = {{LandStandRight, 1}};

    ActionsProbability[LandStandLeft]   = {{StandFacingLeft, 3},
                                           {StandFacingRight, 1},
                                           {RunSlowlyLeft, -1},
                                           {RunSlowlyRight, -1},
                                           {Jump1LeftUp, 1},
                                           {Jump2LeftUp, 1}};
    ActionsProbability[LandStandRight]  = {{StandFacingRight, 3},
                                           {StandFacingLeft, 1},
                                           {RunSlowlyLeft, -1},
                                           {RunSlowlyRight, -1},
                                           {Jump1RightUp, 1},
                                           {Jump2RightUp, 1}};
    ActionsProbability[AgainstWallLeft] = {
        {StandFacingLeft, 5}, {StandFacingRight, 1}, {RunSlowlyRight, -1}, {ClimbUpLeft, 7}};
    ActionsProbability[AgainstWallRight] = {
        {StandFacingLeft, 1}, {StandFacingRight, 5}, {RunSlowlyLeft, -1}, {ClimbUpRight, 7}};
    ActionsProbability[ClimbUpLeft]     = {{WallStayLeft, 8},
                                           {ClimbDownLeft, 1},
                                           {WallJump1Left, 1},
                                           {WallJump2Left, 1},
                                           {WallLongJump1Left, 2},
                                           {WallLongJump2Left, 2},
                                           {WallJumpClimbLeft, 1},
                                           {AirDashRight, -1}};
    ActionsProbability[ClimbUpRight]    = {{WallStayRight, 8},
                                           {ClimbDownRight, 1},
                                           {WallJump1Right, 1},
                                           {WallJump2Right, 1},
                                           {WallLongJump1Right, 2},
                                           {WallLongJump2Right, 2},
                                           {WallJumpClimbRight, 1},
                                           {AirDashLeft, -1}};
    ActionsProbability[WallStayLeft]    = {{ClimbUpLeft, -1},
                                           {ClimbDownLeft, -1},
                                           {WallJump1Left, 1},
                                           {WallJump2Left, 1},
                                           {WallLongJump1Left, 2},
                                           {WallLongJump2Left, 2},
                                           {WallJumpClimbLeft, 2},
                                           {TopClimbRight, -1},
                                           {AirDashRight, -1}};
    ActionsProbability[WallStayRight]   = {{ClimbUpRight, -1},
                                           {ClimbDownRight, -1},
                                           {WallJump1Right, 1},
                                           {WallJump2Right, 1},
                                           {WallLongJump1Right, 2},
                                           {WallLongJump2Right, 2},
                                           {WallJumpClimbRight, 2},
                                           {TopClimbLeft, -1},
                                           {AirDashLeft, -1}};
    ActionsProbability[ClimbDownLeft]   = {{WallStayLeft, 8},
                                           {ClimbUpLeft, 1},
                                           {WallJump1Left, 1},
                                           {WallJump2Left, 1},
                                           {WallLongJump1Left, 2},
                                           {WallLongJump2Left, 2},
                                           {WallJumpClimbLeft, 1},
                                           {AirDashRight, -1}};
    ActionsProbability[ClimbDownRight]  = {{WallStayRight, 8},
                                           {ClimbUpRight, 1},
                                           {WallJump1Right, 1},
                                           {WallJump2Right, 1},
                                           {WallLongJump1Right, 2},
                                           {WallLongJump2Right, 2},
                                           {WallJumpClimbRight, 1},
                                           {AirDashLeft, -1}};
    ActionsProbability[RunFastLeft]     = {{RuntoStandLeft, 10},
                                           {RunFastRight, 1},
                                           {RunSlowlyLeft, -1},
                                           {RunJump1LeftUp, 4},
                                           {RunJump2LeftUp, 4},
                                           {DashBeginLeft, 1},
                                           {RunLeft, -1},
                                           {WalkLeft, 1}};
    ActionsProbability[RunFastRight]    = {{RuntoStandRight, 10},
                                           {RunFastLeft, 1},
                                           {RunSlowlyRight, -1},
                                           {RunJump1RightUp, 4},
                                           {RunJump2RightUp, 4},
                                           {DashBeginRight, 1},
                                           {RunRight, -1},
                                           {WalkRight, 1}};
    ActionsProbability[RuntoStandLeft]  = {{StandFacingLeft, 1}};
    ActionsProbability[RuntoStandRight] = {{StandFacingRight, 1}};

    ActionsProbability[RunJump1LeftUp] = {
        {RunJump1LeftDown, 5}, {DoubleJumpLeftUp, -1}, {AirDashLeft, -1}};
    ActionsProbability[RunJump1RightUp] = {
        {RunJump1RightDown, 5}, {DoubleJumpRightUp, -1}, {AirDashRight, -1}};
    ActionsProbability[RunJump1LeftDown]  = {{LandStandLeft, 1}, {LandRunFastLeft, 3}};
    ActionsProbability[RunJump1RightDown] = {{LandStandRight, 1}, {LandRunFastRight, 3}};

    ActionsProbability[RunJump2LeftUp] = {
        {RunJump2LeftDown, 5}, {DoubleJumpLeftUp, -1}, {AirDashLeft, -1}};
    ActionsProbability[RunJump2RightUp] = {
        {RunJump2RightDown, 5}, {DoubleJumpRightUp, -1}, {AirDashRight, -1}};
    ActionsProbability[RunJump2LeftDown]  = {{LandStandLeft, 1}, {LandRunFastLeft, 3}};
    ActionsProbability[RunJump2RightDown] = {{LandStandRight, 1}, {LandRunFastRight, 3}};

    ActionsProbability[DoubleJumpLeftUp]    = {{DoubleJumpLeftDown, 6},
                                               {DoubleJumpLeftUp, -1},
                                               {FeatherLeft, -1},
                                               {MovingFeatherLeft, -1},
                                               {AirDashLeft, -1}};
    ActionsProbability[DoubleJumpRightUp]   = {{DoubleJumpRightDown, 6},
                                               {DoubleJumpRightUp, -1},
                                               {FeatherRight, -1},
                                               {MovingFeatherRight, -1},
                                               {AirDashRight, -1}};
    ActionsProbability[DoubleJumpLeftDown]  = {{DoubleJumptoFallLeft, -1},
                                               {DoubleJumptoMovingFallLeft, -1},
                                               {FeatherLeft, -1},
                                               {MovingFeatherLeft, -1},
                                               {AirDashLeft, -1}};
    ActionsProbability[DoubleJumpRightDown] = {{DoubleJumptoFallRight, -1},
                                               {DoubleJumptoMovingFallRight, -1},
                                               {FeatherRight, -1},
                                               {MovingFeatherRight, -1},
                                               {AirDashRight, -1}};

    ActionsProbability[DoubleJumptoFallLeft]        = {{FallLeft, 1}};
    ActionsProbability[DoubleJumptoFallRight]       = {{FallRight, 1}};
    ActionsProbability[DoubleJumptoMovingFallLeft]  = {{MovingFallLeft, 1}};
    ActionsProbability[DoubleJumptoMovingFallRight] = {{MovingFallRight, 1}};
    ActionsProbability[LandRunFastLeft]             = {{RunFastLeft, 1}};
    ActionsProbability[LandRunFastRight]            = {{RunFastRight, 1}};

    ActionsProbability[WallJump1Left]     = {{FallLeft, 7},
                                             {DoubleJumpRightUp, -1},
                                             {FeatherLeft, 2},
                                             {AirDashLeft, -1},
                                             {AirDashRight, 1}};
    ActionsProbability[WallJump1Right]    = {{FallRight, 7},
                                             {DoubleJumpLeftUp, -1},
                                             {FeatherRight, 2},
                                             {AirDashLeft, 1},
                                             {AirDashRight, -1}};
    ActionsProbability[WallJump2Left]     = {{FallLeft, 7},
                                             {DoubleJumpRightUp, -1},
                                             {FeatherLeft, 2},
                                             {AirDashLeft, -1},
                                             {AirDashRight, 1}};
    ActionsProbability[WallJump2Right]    = {{FallRight, 7},
                                             {DoubleJumpLeftUp, -1},
                                             {FeatherRight, 2},
                                             {AirDashLeft, 1},
                                             {AirDashRight, -1}};
    ActionsProbability[WallLongJump1Left] = {
        {MovingFallRight, 5}, {DoubleJumpRightUp, -1}, {MovingFeatherRight, 3}, {AirDashRight, -1}};
    ActionsProbability[WallLongJump1Right] = {
        {MovingFallLeft, 5}, {DoubleJumpLeftUp, -1}, {MovingFeatherLeft, 3}, {AirDashLeft, -1}};
    ActionsProbability[WallLongJump2Left] = {
        {MovingFallRight, 5}, {DoubleJumpRightUp, -1}, {MovingFeatherRight, 3}, {AirDashRight, -1}};
    ActionsProbability[WallLongJump2Right] = {
        {MovingFallLeft, 5}, {DoubleJumpLeftUp, -1}, {MovingFeatherLeft, 3}, {AirDashLeft, -1}};
    ActionsProbability[TopStayLeft]  = {{TopClimbLeft, -1},
                                        {TopClimbRight, 1},
                                        {TopStayRight, 2},
                                        {FallLeft, 4},
                                        {FallRight, 1},
                                        {ClimbDownLeft, -1}};
    ActionsProbability[TopStayRight] = {{TopClimbLeft, 1},
                                        {TopClimbRight, -1},
                                        {TopStayLeft, 2},
                                        {FallLeft, 1},
                                        {FallRight, 4},
                                        {ClimbDownRight, -1}};
    ActionsProbability[TopClimbLeft] = {
        {TopStayLeft, 6}, {TopStayRight, 1}, {TopClimbRight, 1}, {MovingFallLeft, 10}};
    ActionsProbability[TopClimbRight] = {
        {TopStayLeft, 1}, {TopStayRight, 6}, {TopStayLeft, 1}, {MovingFallRight, 10}};

    ActionsProbability[FallLeft]  = {{DoubleJumpLeftUp, -1}, {FeatherLeft, -1}, {AirDashLeft, -1}};
    ActionsProbability[FallRight] = {
        {DoubleJumpRightUp, -1}, {FeatherRight, -1}, {AirDashRight, -1}};
    ActionsProbability[MovingFallLeft] = {
        {DoubleJumpLeftUp, -1}, {FeatherLeft, -1}, {MovingFeatherLeft, -1}, {AirDashLeft, -1}};
    ActionsProbability[MovingFallRight] = {
        {DoubleJumpRightUp, -1}, {FeatherRight, -1}, {MovingFeatherRight, -1}, {AirDashRight, -1}};

    ActionsProbability[FeatherLeft]  = {{FallLeft, 7}, {DoubleJumpLeftUp, -1}, {AirDashLeft, -1}};
    ActionsProbability[FeatherRight] = {
        {FallRight, 7}, {DoubleJumpRightUp, -1}, {AirDashRight, -1}};
    ActionsProbability[MovingFeatherLeft] = {
        {MovingFallLeft, 7}, {DoubleJumpLeftUp, -1}, {AirDashLeft, -1}};
    ActionsProbability[MovingFeatherRight] = {
        {MovingFallRight, 7}, {DoubleJumpRightUp, -1}, {AirDashRight, -1}};

    ActionsProbability[DashBeginLeft]  = {{DashEndLeft, 4}, {DashBeginLeft, 2}, {RunFastLeft, -1}};
    ActionsProbability[DashBeginRight] = {
        {DashEndRight, 4}, {DashBeginRight, 2}, {RunFastRight, -1}};
    ActionsProbability[DashEndLeft]  = {{StandFacingLeft, 1}};
    ActionsProbability[DashEndRight] = {{StandFacingRight, 1}};

    ActionsProbability[WallJumpClimbLeft]  = {{WallStayLeft, 1},
                                              {ClimbUpLeft, -1},
                                              {ClimbDownLeft, 1},
                                              {WallJump1Left, 1},
                                              {WallJump2Left, 1},
                                              {WallLongJump1Left, 2},
                                              {WallLongJump2Left, 2}};
    ActionsProbability[WallJumpClimbRight] = {{WallStayRight, 1},
                                              {ClimbUpRight, -1},
                                              {ClimbDownRight, 1},
                                              {WallJump1Right, 1},
                                              {WallJump2Right, 1},
                                              {WallLongJump1Right, 2},
                                              {WallLongJump2Right, 2}};

    ActionsProbability[MouseHoldLeft]          = {{FeatherAfterMouseLeft, 1}};
    ActionsProbability[MouseHoldRight]         = {{FeatherAfterMouseRight, 1}};
    ActionsProbability[FeatherAfterMouseLeft]  = {{FallLeft, 7}, {DoubleJumpLeftUp, -1}};
    ActionsProbability[FeatherAfterMouseRight] = {{FallRight, 7}, {DoubleJumpRightUp, -1}};

    ActionsProbability[AirDashLeft] = {
        {AirDashtoFallLeft, 8}, {DoubleJumpLeftUp, -1}, {MovingFeatherLeft, 2}};
    ActionsProbability[AirDashRight] = {
        {AirDashtoFallRight, 8}, {DoubleJumpRightUp, -1}, {MovingFeatherRight, 2}};
    ActionsProbability[AirDashtoFallLeft] = {
        {MovingFallLeft, 10}, {DoubleJumpLeftUp, -1}, {MovingFeatherLeft, 2}};
    ActionsProbability[AirDashtoFallRight] = {
        {MovingFallRight, 10}, {DoubleJumpRightUp, -1}, {MovingFeatherRight, 2}};
    ActionsProbability[GetDownLeft]      = {{StandFacingLeft, 12},
                                            {StandFacingRight, 2},
                                            {RunSlowlyLeft, 2},
                                            {Jump1LeftUp, 1},
                                            {Jump2LeftUp, 1},
                                            {RunFastLeft, 1},
                                            {GetDownWalkLeft, -1},
                                            {GetDownWalkRight, 1}};
    ActionsProbability[GetDownRight]     = {{StandFacingLeft, 2},
                                            {StandFacingRight, 12},
                                            {RunSlowlyRight, 2},
                                            {Jump1RightUp, 1},
                                            {Jump2RightUp, 1},
                                            {RunFastRight, 1},
                                            {GetDownWalkLeft, 1},
                                            {GetDownWalkRight, -1}};
    ActionsProbability[GetDownWalkLeft]  = {{GetDownLeft, 5},
                                            {GetDownRight, 1},
                                            {StandFacingLeft, 6},
                                            {StandFacingRight, 1},
                                            {RunSlowlyLeft, -1},
                                            {RunFastLeft, -1}};
    ActionsProbability[GetDownWalkRight] = {{GetDownLeft, 1},
                                            {GetDownRight, 5},
                                            {StandFacingLeft, 1},
                                            {StandFacingRight, 6},
                                            {RunSlowlyRight, -1},
                                            {RunFastRight, -1}};

    ActionsProbability[RunLeft]  = {{StandFacingLeft, 20},
                                    {StandFacingRight, 1},
                                    {RunSlowlyLeft, 5},
                                    {RunSlowlyRight, 1},
                                    {RunFastLeft, 10},
                                    {RunFastRight, 1},
                                    {WalkLeft, 3},
                                    {WalkRight, 1}};
    ActionsProbability[RunRight] = {{StandFacingLeft, 1},
                                    {StandFacingRight, 20},
                                    {RunSlowlyLeft, 1},
                                    {RunSlowlyRight, 5},
                                    {RunFastLeft, 1},
                                    {RunFastRight, 10},
                                    {WalkLeft, 1},
                                    {WalkRight, 3}};

    ActionsProbability[WalkLeft]  = {{StandFacingLeft, 20},
                                     {StandFacingRight, 1},
                                     {RunSlowlyLeft, 10},
                                     {RunSlowlyRight, 1},
                                     {RunFastLeft, 10},
                                     {RunFastRight, 1},
                                     {RunLeft, 2}};
    ActionsProbability[WalkRight] = {{StandFacingLeft, 1},
                                     {StandFacingRight, 20},
                                     {RunSlowlyLeft, 1},
                                     {RunSlowlyRight, 10},
                                     {RunFastLeft, 1},
                                     {RunFastRight, 10},
                                     {RunRight, 2}};

    ActionsProbability[BashUpChargeLeft]  = {{BashUpLeft, 1}};
    ActionsProbability[BashUpChargeRight] = {{BashUpRight, 1}};
    ActionsProbability[BashUpLeft]        = {{FallLeft, 1}};
    ActionsProbability[BashUpRight]       = {{FallRight, 1}};

    ActionsProbability[BashHorChargeLeft]  = {{BashHorLeft, 1}};
    ActionsProbability[BashHorChargeRight] = {{BashHorRight, 1}};
    ActionsProbability[BashHorLeft]        = {{FallLeft, 1}};
    ActionsProbability[BashHorRight]       = {{FallRight, 1}};

    ActionsProbability[BashDownChargeLeft]  = {{BashDownLeft, 1}};
    ActionsProbability[BashDownChargeRight] = {{BashDownRight, 1}};
    ActionsProbability[BashDownLeft]        = {{FallLeft, 1}};
    ActionsProbability[BashDownRight]       = {{FallRight, 1}};

    ActionsProbability[BashDiaUpChargeLeft]  = {{BashDiaUpLeft, 1}};
    ActionsProbability[BashDiaUpChargeRight] = {{BashDiaUpRight, 1}};
    ActionsProbability[BashDiaUpLeft]        = {{FallLeft, 1}};
    ActionsProbability[BashDiaUpRight]       = {{FallRight, 1}};

    ActionsProbability[BashDiaDownChargeLeft]  = {{BashDiaDownLeft, 1}};
    ActionsProbability[BashDiaDownChargeRight] = {{BashDiaDownRight, 1}};
    ActionsProbability[BashDiaDownLeft]        = {{FallLeft, 1}};
    ActionsProbability[BashDiaDownRight]       = {{FallRight, 1}};

    ActionsProbability[QuitStepLeft]  = {{StandFacingLeft, 20},
                                         {StandFacingRight, 1},
                                         {RunSlowlyRight, -1},
                                         {RunFastLeft, -1},
                                         {RunFastRight, 1},
                                         {WalkLeft, 2}};
    ActionsProbability[QuitStepRight] = {{StandFacingLeft, 20},
                                         {StandFacingRight, 2},
                                         {RunSlowlyLeft, -1},
                                         {RunFastLeft, 1},
                                         {RunFastRight, -1},
                                         {WalkRight, 2}};

    // LookUpListenLeft和 LookUpListenRight作为一般动作的转移基准
    ActionsProbability[LookUpListenLeft]         = {{StandFacingLeft, 18},
                                                    {StandFacingRight, 1},
                                                    {RunSlowlyLeft, -1},
                                                    {RunSlowlyRight, 1},
                                                    {Jump1LeftUp, 3},
                                                    {Jump2LeftUp, 3},
                                                    {RunFastLeft, -1},
                                                    {RunFastRight, 1},
                                                    {GetDownLeft, 1},
                                                    {WalkLeft, -1},
                                                    {WalkRight, -1},
                                                    {QuitStepLeft, 2}};
    ActionsProbability[LookUpListenRight]        = {{StandFacingLeft, 1},
                                                    {StandFacingRight, 18},
                                                    {RunSlowlyLeft, 1},
                                                    {RunSlowlyRight, -1},
                                                    {Jump1RightUp, 3},
                                                    {Jump2RightUp, 3},
                                                    {RunFastLeft, 1},
                                                    {RunFastRight, -1},
                                                    {GetDownRight, 1},
                                                    {WalkLeft, -1},
                                                    {WalkRight, -1},
                                                    {QuitStepRight, 2}};
    ActionsProbability[StandtoLookUpListenLeft]  = {{LookUpListenLeft, 1}};
    ActionsProbability[StandtoLookUpListenRight] = {{LookUpListenRight, 1}};

    ActionsProbability[LookUpLeft]  = ActionsProbability[LookUpListenLeft];
    ActionsProbability[LookUpRight] = ActionsProbability[LookUpListenRight];

    ActionsProbability[YawnLeft]  = ActionsProbability[LookUpListenLeft];
    ActionsProbability[YawnRight] = ActionsProbability[LookUpListenRight];

    ActionsProbability[StandLefttoActCuteRight] = {{ActCuteRight, 1}};
    ActionsProbability[StandRighttoActCuteLeft] = {{ActCuteLeft, 1}};
    ActionsProbability[ActCuteLeft]             = ActionsProbability[LookUpListenLeft];
    ActionsProbability[ActCuteRight]            = ActionsProbability[LookUpListenRight];

    // 音效加载
    SoundMap[RunSlowlyLeft] = SoundMap[RunSlowlyRight] = SoundMap[GetDownWalkLeft] =
        SoundMap[GetDownWalkRight] = SoundMap[RunLeft] = SoundMap[RunRight] = SoundMap[WalkLeft] =
            SoundMap[WalkRight] = SoundMap[QuitStepLeft] = SoundMap[QuitStepRight] =
                SoundsDetail("Sound/stepSound/seinFootstepsRock", 5, 2);
    SoundMap[LandStandLeft] = SoundMap[LandStandRight] = SoundMap[LandRunFastLeft] =
        SoundMap[LandRunFastRight] = SoundsDetail("Sound/land/seinLandsStone", 5, 1);

    SoundMap[Jump1LeftUp] = SoundMap[Jump1RightUp] = SoundMap[Jump2LeftUp] =
        SoundMap[Jump2RightUp] = SoundMap[RunJump1LeftUp] = SoundMap[RunJump1RightUp] =
            SoundMap[RunJump2LeftUp]                      = SoundMap[RunJump2RightUp] =
                SoundsDetail("Sound/jump/seinJumpsGrass", 5, 1);

    SoundMap[ClimbUpLeft] = SoundMap[ClimbUpRight] = SoundMap[ClimbDownLeft] =
        SoundMap[ClimbDownRight] = SoundMap[TopClimbLeft] = SoundMap[TopClimbRight] =
            SoundsDetail("Sound/wallClimb/seinWallClimbStepsGrass", 5, 1);
    SoundMap[RunFastLeft] = SoundMap[RunFastRight] =
        SoundsDetail("Sound/stepSound/seinFootstepsRock", 5, 10);
    SoundMap[DoubleJumpLeftUp] = SoundMap[DoubleJumpRightUp] =
        SoundsDetail("Sound/doubleJump/seinDoubleJumps", 5, 1);

    SoundMap[WallJump1Left] = SoundMap[WallJump1Right] = SoundMap[WallJump2Left] =
        SoundMap[WallJump2Right] = SoundMap[WallLongJump1Left] = SoundMap[WallLongJump1Right] =
            SoundMap[WallLongJump2Left]                        = SoundMap[WallLongJump2Right] =
                SoundMap[WallJumpClimbLeft]                    = SoundMap[WallJumpClimbRight] =
                    SoundsDetail("Sound/wallJump/seinWallJumps", 5, 1);
    SoundMap[FeatherLeft] = SoundMap[FeatherRight] = SoundMap[FeatherAfterMouseLeft] =
        SoundMap[FeatherAfterMouseRight] = SoundsDetail("Sound/feather/oriGlideStart", 3, 0);
    SoundMap[MovingFeatherLeft]          = SoundMap[MovingFeatherRight] =
        SoundsDetail("Sound/movingFeather/oriGlideMoveLeftRight", 5, 0);
    SoundMap[DashBeginLeft] = SoundMap[DashBeginRight] = SoundMap[AirDashLeft] =
        SoundMap[AirDashRight]                         = SoundsDetail("Sound/dash/oriDash", 5, 1);

    SoundMap[BashUpChargeLeft] = SoundMap[BashUpChargeRight] = SoundMap[BashHorChargeLeft] =
        SoundMap[BashHorChargeRight]                         = SoundMap[BashDownChargeLeft] =
            SoundMap[BashDownChargeRight]                    = SoundMap[BashDiaUpChargeLeft] =
                SoundMap[BashDiaUpChargeRight]               = SoundMap[BashDiaDownChargeLeft] =
                    SoundMap[BashDiaDownChargeRight] =
                        SoundsDetail("Sound/bashCharge/oriBashStart", 1, 1);

    SoundMap[BashUpLeft] = SoundMap[BashUpRight] = SoundMap[BashHorLeft] = SoundMap[BashHorRight] =
        SoundMap[BashDownLeft] = SoundMap[BashDownRight] = SoundMap[BashDiaUpLeft] =
            SoundMap[BashDiaUpRight] = SoundMap[BashDiaDownLeft] = SoundMap[BashDiaDownRight] =
                SoundsDetail("Sound/bash/oriBashEnd", 3, 1);

    SoundMap[YawnLeft] = SoundMap[YawnRight] = SoundsDetail("Sound/idle/seinIdleYawn", 1, 1);

    SoundMap[MouseHoldLeft] = SoundMap[MouseHoldRight] =
        SoundsDetail("Sound/mouseHold/mouseHold", 6, 0);

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

int bashBeginMouseX, bashBeginMouseY;
int originalMouseX, originalMousey;

// 位移函数
pair<int, int> ActionsMovement(Action    action,
                               int       x,
                               int       y,
                               int       vx,
                               int       vy,
                               int       curFrame,
                               long long time,
                               int       mousex,
                               int       mousey)
{
    int dx = 0, dy = 0;
    if (action == RunSlowlyLeft || action == RunSlowlyRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 8;
        dy = 0;
    }
    if (action == Jump1LeftUp || action == Jump1RightUp || action == Jump2LeftUp ||
        action == Jump2RightUp)
    {
        dx = 0;
        dy = -(ActionsMap[Jump1LeftUp].totalFrameNumber +
               ActionsMap[Jump1LeftDown].totalFrameNumber + 1) +
             curFrame * 2;
    }
    if (action == Jump1LeftDown || action == Jump1RightDown || action == Jump2LeftDown ||
        action == Jump2RightDown)
    {
        dx = 0;
        dy = -(ActionsMap[Jump1LeftUp].totalFrameNumber +
               ActionsMap[Jump1LeftDown].totalFrameNumber + 1) +
             (curFrame + ActionsMap[Jump1LeftUp].totalFrameNumber) * 2;
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
    if (action == RunFastLeft || action == RunFastRight || action == LandRunFastLeft ||
        action == LandRunFastLeft)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 18;
        dy = 0;
    }
    if (action == RunJump1LeftUp || action == RunJump1RightUp || action == RunJump2LeftUp ||
        action == RunJump2RightUp)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 18;
        dy = -(ActionsMap[RunJump1LeftUp].totalFrameNumber +
               ActionsMap[RunJump1LeftDown].totalFrameNumber + 1) +
             curFrame * 2;
    }
    if (action == RunJump1LeftDown || action == RunJump1RightDown || action == RunJump2LeftDown ||
        action == RunJump2RightDown)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 18;
        dy = -(ActionsMap[RunJump1LeftUp].totalFrameNumber +
               ActionsMap[RunJump1LeftDown].totalFrameNumber + 1) +
             (curFrame + ActionsMap[RunJump1LeftUp].totalFrameNumber) * 2;
    }
    if (action == DoubleJumpLeftUp || action == DoubleJumpRightUp)
    {
        dx = vx;
        dy = -(ActionsMap[DoubleJumpLeftUp].totalFrameNumber +
               ActionsMap[DoubleJumpLeftDown].totalFrameNumber + 1) +
             curFrame * 2;
    }
    if (action == DoubleJumpLeftDown || action == DoubleJumpRightDown)
    {
        dx = vx;
        dy = -(ActionsMap[DoubleJumpLeftUp].totalFrameNumber +
               ActionsMap[DoubleJumpLeftDown].totalFrameNumber + 1) +
             (curFrame + ActionsMap[DoubleJumpLeftUp].totalFrameNumber) * 2;
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
    if (action == WallJump1Left || action == WallJump1Right)
    {
        if (action == WallJump1Left)
        {
            dx = (ActionsMap[action].totalFrameNumber - curFrame + 5) / 2;
        }
        else
        {
            dx = -(ActionsMap[action].totalFrameNumber - curFrame + 5) / 2;
        }
        dy = -18 + time * 2;
    }
    if (action == WallJump2Left || action == WallJump2Right)
    {
        if (action == WallJump2Left)
        {
            dx = (ActionsMap[action].totalFrameNumber - curFrame + 5) / 2 *
                 ActionsMap[WallJump1Left].totalFrameNumber /
                 ActionsMap[WallJump2Left].totalFrameNumber;
        }
        else
        {
            dx = -(ActionsMap[action].totalFrameNumber - curFrame + 5) / 2 *
                 ActionsMap[WallJump1Left].totalFrameNumber /
                 ActionsMap[WallJump2Left].totalFrameNumber;
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
    if (action == FeatherLeft || action == FeatherRight)
    {
        dx = vx * 0.9f;
        dy = vy * 0.8f + 2;
    }
    if (action == MovingFeatherLeft || action == MovingFeatherRight)
    {
        dx = vx;
        dy = vy * 0.8f + 2;
    }
    if (action == DashBeginLeft || action == DashBeginRight)
    {
        int dashVertor[14] = {60, 45, 40, 40, 35, 35, 35, 20, 15, 10, 3, 3, 0, 0};
        dx                 = (ActionsMap[action].transform ? -1 : 1) * dashVertor[curFrame - 1];
        dy                 = 0;
    }
    if (action == DashEndLeft || action == DashEndRight)
    {
        int dashVertor[14] = {60, 45, 40, 40, 35, 35, 35, 20, 15, 10, 3, 3, 0, 0};
        dx                 = (ActionsMap[action].transform ? -1 : 1) *
             dashVertor[curFrame - 1 + ActionsMap[DashBeginLeft].totalFrameNumber];
        dy = 0;
    }
    if (action == WallJumpClimbLeft || action == WallJumpClimbRight)
    {
        dx = (ActionsMap[action].transform ? 1 : -1) *
             ((ActionsMap[WallJumpClimbLeft].totalFrameNumber + 1) - 2 * curFrame);
        dy = -25 + curFrame;
    }
    if (action == MouseHoldLeft || action == MouseHoldRight)
    {
        dx = mousex - x - 2;
        dy = mousey - y;
    }
    if (action == FeatherAfterMouseLeft || action == FeatherAfterMouseRight)
    {
        dx = vx * 0.92f;
        dy = vy * 0.8f + 2;
    }
    if (action == AirDashLeft || action == AirDashRight)
    {
        int dashVertor[13] = {70, 60, 40, 40, 30, 30, 30, 30, 20, 15, 15, 15, 15};
        dx                 = (ActionsMap[action].transform ? -1 : 1) * dashVertor[curFrame - 1];
        dy                 = 0;
    }
    if (action == AirDashtoFallLeft || action == AirDashtoFallRight)
    {
        int dashVertor[5] = {12, 12, 12, 12, 12};
        dx                = (ActionsMap[action].transform ? -1 : 1) * dashVertor[curFrame - 1];
        dy                = vy + 2;
    }
    if (action == GetDownWalkLeft || action == GetDownWalkRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 2;
        dy = 0;
    }
    if (action == RunLeft || action == RunRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 12;
        dy = 0;
    }
    if (action == WalkLeft || action == WalkRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * 4;
        dy = 0;
    }

    if (action == BashUpChargeLeft || action == BashUpChargeRight || action == BashDownChargeLeft ||
        action == BashDownChargeRight || action == BashHorChargeLeft ||
        action == BashHorChargeRight || action == BashDiaUpChargeLeft ||
        action == BashDiaUpChargeRight || action == BashDiaDownChargeLeft ||
        action == BashDiaDownChargeRight)
    {
        if (curFrame == 1)
        {
            QPoint pos      = QCursor::pos();
            bashBeginMouseX = pos.x();
            bashBeginMouseY = pos.y();
            originalMouseX  = mousex;
            originalMousey  = mousey;
        }
        else
        {
            QCursor::setPos(bashBeginMouseX, bashBeginMouseY);
        }
    }

    if (action == BashUpLeft || action == BashUpRight)
    {
        dx = 0;
        dy = -(120 / curFrame + 4 - curFrame * 0.35);

        bashBeginMouseX += 0;
        bashBeginMouseY += (30 / curFrame);
        QCursor::setPos(bashBeginMouseX, bashBeginMouseY);
    }
    if (action == BashHorLeft || action == BashHorRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * (120 / curFrame + 4);
        dy = curFrame * 0.7;

        bashBeginMouseX += -(ActionsMap[action].transform ? -1 : 1) * (30 / curFrame);
        bashBeginMouseY += 0;
        QCursor::setPos(bashBeginMouseX, bashBeginMouseY);
    }
    if (action == BashDownLeft || action == BashDownRight)
    {
        dx = 0;
        dy = (120 / curFrame + 4 + curFrame * 0.35);

        bashBeginMouseX += 0;
        bashBeginMouseY += -(30 / curFrame);
        QCursor::setPos(bashBeginMouseX, bashBeginMouseY);
    }
    if (action == BashDiaUpLeft || action == BashDiaUpRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * (120 / curFrame + 5) * 0.72;
        dy = -(120 / curFrame + 4 - curFrame * 0.35) * 0.72;

        bashBeginMouseX += -(ActionsMap[action].transform ? -1 : 1) * (30 / curFrame) * 0.72;
        bashBeginMouseY += (30 / curFrame) * 0.72;
        QCursor::setPos(bashBeginMouseX, bashBeginMouseY);
    }
    if (action == BashDiaDownLeft || action == BashDiaDownRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * (120 / curFrame + 5) * 0.72;
        dy = (120 / curFrame + 4 + curFrame * 0.9) * 0.72;

        bashBeginMouseX += -(ActionsMap[action].transform ? -1 : 1) * (30 / curFrame) * 0.72;
        bashBeginMouseY += -(30 / curFrame) * 0.72;
        QCursor::setPos(bashBeginMouseX, bashBeginMouseY);
    }
    if (action == QuitStepLeft || action == QuitStepRight)
    {
        dx = (ActionsMap[action].transform ? -1 : 1) * (curFrame % 2 ? 1 : 2);
        dy = 0;
    }

    return {dx, dy};
}

int ActionsDX(Action    action,
              int       x,
              int       y,
              int       vx,
              int       vy,
              int       curFrame,
              long long time,
              int       mousex,
              int       mousey)
{
    return ActionsMovement(action, x, y, vx, vy, curFrame, time, mousex, mousey).first;
}
int ActionsDY(Action    action,
              int       x,
              int       y,
              int       vx,
              int       vy,
              int       curFrame,
              long long time,
              int       mousex,
              int       mousey)
{
    return ActionsMovement(action, x, y, vx, vy, curFrame, time, mousex, mousey).second;
}
