﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QWidgetAction>

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
    QSystemTrayIcon* tray;        // 托盘图标
    QMenu*           trayMenu;    // 托盘菜单
    QMenu*           clickMenu;   // 右键菜单
    QMenu*           actionMenu;  // 右键动作菜单

    QAction*       quitAction;     // 退出动作
    QAction*       aboutAction;    // 关于
    QAction*       controlAction;  // 是否可以控制
    QAction*       mutedAction;    // 静音控制
    QWidgetAction* volumeAction;   // 音量控制
    QAction*       limitAction;    // 是否动作限制
    QAction*       baseSetAction;  // 基准设置

    // 动作设置
    QAction* actCuteAction;
    QAction *sitAction, *sitLongAction;
    QAction *sleepAction, *sleepLongAction;

    Player*      player;
    Behavior*    behavior;
    SoundPlayer* soundPlayer;
    SoundPlayer* liveSoundPlayer;

    void createActions();     // 创建动作
    void createActionMenu();  // 创建动作菜单
    void actionMenuUpdate();  // 动作菜单可选性更新
    void createTrayMenu();    // 创建托盘菜单
    void createClickMenu();   // 创建右键菜单

   protected:
    void timerEvent(QTimerEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);  // 处理右键菜单事件
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    // void mouseDoubleClickEvent(QMouseEvent* event);
    void focusOutEvent(QFocusEvent* event);

   public slots:
    void aboutShowSlot();
    void baseInputShowSlot();
    void quitSlot();
};

#endif  // MAINWINDOW_H
