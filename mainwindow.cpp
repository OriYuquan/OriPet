#include "mainwindow.h"

#include <QCursor>
#include <QDebug>
#include <QKeyEvent>
#include <QPoint>

#include "aboutdialog.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
    // 资源文件信息加载
    ActionsDetailLoad();

    // 设置窗口属性
    setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);
    // 任务栏宽度修正
    setGeometry(QRect(0, 0, SCREENWIDTH, SCREENHEIGHT - 100));

    // FPS设定为30
    startTimer(RATE);

    // 创建系统托盘图标
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon("Ori.ico"));
    tray->setToolTip(tr("奥里桌宠"));

    // 创建动作和菜单
    createActions();
    createTrayMenu();
    createClickMenu();

    // 连接托盘图标的右键菜单
    tray->setContextMenu(trayMenu);
    tray->show();  // 显示托盘图标

    // 播放器初始化
    player = new Player(this);
    // 音频播放器初始化
    soundPlayer = new SoundPlayer(this);
    // 实时音频播放
    liveSoundPlayer = new SoundPlayer(this);
    // 状态机初始化
    behavior = new Behavior(this);

    connect(behavior, SIGNAL(PlayerLoadNewActionSignal(Action)), player, SLOT(loadAction(Action)));
    connect(behavior, SIGNAL(PlayerNextPixSignal()), player, SLOT(pixUpdate()));
    connect(behavior, SIGNAL(PlayerMirrorSignal()), player, SLOT(mirrorAction()));
    connect(behavior,
            SIGNAL(SoundPlayerLoadNewActionSignal(Action)),
            soundPlayer,
            SLOT(loadAction(Action)));
    connect(behavior,
            SIGNAL(SoundPlayerPathPlay(QString, int)),
            liveSoundPlayer,
            SLOT(loadPathAndPlay(QString, int)));

    player->loadAction(StandFacingRight);
    soundPlayer->loadAction(StandFacingRight);
    behavior->loadAction(
        StandFacingRight, SCREENWIDTHFIX + 100, SCREENHEIGHTFIX + SCREENHEIGHT - ORIHEIGHT);

    player->move(behavior->getX(), behavior->getY());
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    // 创建退出动作并连接到关闭槽
    quitAction = new QAction(tr("退出"), this);
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    aboutAction = new QAction(tr("关于"), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutShowSlot()));
}

void MainWindow::createTrayMenu()
{
    // 创建托盘菜单并添加退出动作
    trayMenu = new QMenu(this);
    trayMenu->addAction(aboutAction);
    trayMenu->addAction(quitAction);
}

void MainWindow::createClickMenu()
{
    // 创建右键菜单并添加退出动作
    clickMenu = new QMenu(this);
    clickMenu->addAction(quitAction);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
    // 显示右键菜单
    clickMenu->exec(event->globalPos());
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    behavior->setMousePos(mousePos.x(), mousePos.y());
    player->update();
    soundPlayer->soundPlay(player->curFrame);
    behavior->actionUpdate(player->curFrame, player->timePlayed);
    player->move(behavior->getX(), behavior->getY());
}

// 关于对话框
void MainWindow::aboutShowSlot()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    behavior->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    behavior->keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    // 只有点击在桌宠上时才相应操作
    if (QApplication::widgetAt(event->globalPos()) == player)
        behavior->mousePressEvent(event);
}
void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    behavior->mouseReleaseEvent(event);
}
