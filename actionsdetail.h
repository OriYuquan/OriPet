#ifndef ACTIONSDETAIL_H
#define ACTIONSDETAIL_H

#include <QMap>
#include <QSet>
#include <QString>

#define RATE 23
#define CONTROLTIME 100

// 添加枚举量 + 添加动作文件 + 添加音效文件 + 添加状态机转移（特殊转移状态函数） + 添加速度函数

// 规定所有动作的枚举量
enum Action
{
    StandFacingLeft,
    StandFacingRight,
    RunSlowlyLeft,
    RunSlowlyRight,

    Jump1LeftUp,
    Jump1RightUp,
    Jump1LeftDown,
    Jump1RightDown,

    Jump2LeftUp,
    Jump2RightUp,
    Jump2LeftDown,
    Jump2RightDown,

    LandStandLeft,
    LandStandRight,
    AgainstWallLeft,
    AgainstWallRight,
    ClimbUpLeft,
    ClimbUpRight,
    WallStayLeft,
    WallStayRight,
    ClimbDownLeft,
    ClimbDownRight,
    RunFastLeft,
    RunFastRight,
    RuntoStandLeft,
    RuntoStandRight,

    RunJump1LeftUp,
    RunJump1RightUp,
    RunJump1LeftDown,
    RunJump1RightDown,

    RunJump2LeftUp,
    RunJump2RightUp,
    RunJump2LeftDown,
    RunJump2RightDown,

    DoubleJumpLeftUp,
    DoubleJumpRightUp,
    DoubleJumpLeftDown,
    DoubleJumpRightDown,

    DoubleJumptoFallLeft,
    DoubleJumptoFallRight,
    DoubleJumptoMovingFallLeft,
    DoubleJumptoMovingFallRight,

    FallLeft,
    FallRight,
    MovingFallLeft,
    MovingFallRight,

    LandRunFastLeft,
    LandRunFastRight,

    WallJump1Left,
    WallJump1Right,
    WallJump2Left,
    WallJump2Right,
    WallLongJump1Left,
    WallLongJump1Right,
    WallLongJump2Left,
    WallLongJump2Right,

    TopStayLeft,
    TopStayRight,
    TopClimbLeft,
    TopClimbRight,

    FeatherLeft,
    FeatherRight,
    MovingFeatherLeft,
    MovingFeatherRight,

    DashBeginLeft,
    DashBeginRight,
    DashEndLeft,
    DashEndRight,
    AirDashLeft,
    AirDashRight,
    AirDashtoFallLeft,
    AirDashtoFallRight,

    WallJumpClimbLeft,
    WallJumpClimbRight,

    MouseHoldLeft,
    MouseHoldRight,
    FeatherAfterMouseLeft,
    FeatherAfterMouseRight,

    GetDownLeft,
    GetDownRight,
    GetDownWalkLeft,
    GetDownWalkRight,

    RunLeft,
    RunRight,

    WalkLeft,
    WalkRight,

    BashUpChargeLeft,
    BashUpChargeRight,
    BashUpLeft,
    BashUpRight,

    BashHorChargeLeft,
    BashHorChargeRight,
    BashHorLeft,
    BashHorRight,

    BashDownChargeLeft,
    BashDownChargeRight,
    BashDownLeft,
    BashDownRight,

    BashDiaUpChargeLeft,
    BashDiaUpChargeRight,
    BashDiaUpLeft,
    BashDiaUpRight,

    BashDiaDownChargeLeft,
    BashDiaDownChargeRight,
    BashDiaDownLeft,
    BashDiaDownRight,

    QuitStepLeft,
    QuitStepRight,

    LookUpListenLeft,
    LookUpListenRight,
    StandtoLookUpListenLeft,
    StandtoLookUpListenRight,

    LookUpLeft,
    LookUpRight,

    YawnLeft,
    YawnRight,

    ActCuteLeft,
    ActCuteRight,
    StandLefttoActCuteRight,
    StandRighttoActCuteLeft,

    None
};

// 每个动作文件地址与数量与是否反转
struct ActionsDetail
{
    QString path;
    int     totalFrameNumber;
    bool    transform;
    // 动作冷却时间
    int    leastTime;
    double keepPossiblity;
    // 是否接受控制
    bool control;
    bool completelyPlay;
    ActionsDetail(QString _path = "",
                  int     num   = 0,
                  bool    trans = false,
                  int     least = 70,
                  double  pos   = 0.5,
                  bool    con   = true,
                  bool    com   = true)
        : path(_path),
          totalFrameNumber(num),
          transform(trans),
          leastTime(least),
          keepPossiblity(pos),
          control(con),
          completelyPlay(com)
    {
    }
};

struct SoundsDetail
{
    QString path;
    int     totalFrameNumber;
    int     frequency;
    SoundsDetail(QString _path = "", int num = 0, int fre = 1)
        : path(_path), totalFrameNumber(num), frequency(fre)
    {
    }
};

// 映射关系
extern QMap<Action, ActionsDetail> ActionsMap;
extern QMap<Action, SoundsDetail>  SoundMap;
extern QMap<Action, Action>        ActionsMirror;

// 动作限制
extern QSet<Action> ActionLimit;

// 转移概率矩阵
extern QMap<Action, QMap<Action, double>> ActionsProbability;
extern QMap<Action, Action>               ActionsColdTrans;

// 冷却时间映射
extern QMap<Action, int> ActionsLeastTimes;

// 屏幕常量
extern int ORIWIDTH, ORIHEIGHT;
extern int SCREENWIDTHFIX, SCREENHEIGHTFIX;
extern int SCREENWIDTH, SCREENHEIGHT;
extern int PNGWIDTH, PNGHEIGHT;

// 猛击坐标
extern int originalMouseX;
extern int originalMousey;

// 映射关系初始化
void ActionsDetailLoad();

// 位移函数
int ActionsDX(Action    action,
              int       x,
              int       y,
              int       vx,
              int       vy,
              int       curFrame,
              long long time,
              int       mousex,
              int       mousey);
int ActionsDY(Action    action,
              int       x,
              int       y,
              int       vx,
              int       vy,
              int       curFrame,
              long long time,
              int       mousex,
              int       mousey);

#endif  // ACTIONSDETAIL_H
