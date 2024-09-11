#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QWidget>

#include "actionsdetail.h"
#include "behavior.h"
#include "player.h"
#include "soundplayer.h"

class MainWindow : public QWidget
{
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private:
    QSystemTrayIcon* tray;       // 托盘图标
    QMenu*           trayMenu;   // 托盘菜单
    QMenu*           clickMenu;  // 右键菜单

    QAction* quitAction;  // 退出动作

    Player*      player;
    Behavior*    behavior;
    SoundPlayer* soundPlayer;
    SoundPlayer* liveSoundPlayer;

    void createActions();    // 创建动作
    void createTrayMenu();   // 创建托盘菜单
    void createClickMenu();  // 创建右键菜单

   protected:
    void timerEvent(QTimerEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);  // 处理右键菜单事件
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    //    void mousePressEvent(QMouseEvent *event);
    //    void mouseDoubleClickEvent(QMouseEvent *event);
    //    void mouseReleaseEvent(QMouseEvent *event);
    //    void mouseMoveEvent(QMouseEvent *event);
};

#endif  // MAINWINDOW_H
