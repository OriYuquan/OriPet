#ifndef ACTIONSDETAIL_H
#define ACTIONSDETAIL_H

#include <QMap>
#include <QString>

#define RATE 25
#define CONTROLTIME 50

// 添加枚举量 + 添加动作文件 + 添加音效文件 + 添加状态机转移（特殊转移状态函数） + 添加速度函数

// 规定所有动作的枚举量
enum Action
{
    StandFacingLeft,
    StandFacingRight,
    RunSlowlyLeft,
    RunSlowlyRight,
    Jump1Left,
    Jump1Right,
    Jump2Left,
    Jump2Right,
    LandStandLeft,
    LandStandRight,
    //    LandRunFastLeft,
    //    LandRunFastRight,
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
    RunJump1Left,
    RunJump1Right,
    RunJump2Left,
    RunJump2Right,
    DoubleJumpLeft,
    DoubleJumpRight,
    TripleJumpLeft,
    TripleJumpRight,
    DoubleJumptoFallLeft,
    DoubleJumptoFallRight,
    DoubleJumptoMovingFallLeft,
    DoubleJumptoMovingFallRight,
    FallLeft,
    FallRight,
    MovingFallLeft,
    MovingFallRight,

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

// 映射关系初始化
void ActionsDetailLoad();

// 位移函数
int ActionsDX(Action action, int x, int y, int vx, int vy, int curFrame, long long time);
int ActionsDY(Action action, int x, int y, int vx, int vy, int curFrame, long long time);

#endif  // ACTIONSDETAIL_H
