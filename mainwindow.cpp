#include "mainwindow.h"

#include <QCursor>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QPoint>
#include <QPushButton>
#include <QSlider>

#include "aboutdialog.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    debugLabel = new QLabel(this);
    debugLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    setCursor(Qt::PointingHandCursor);
    //  资源文件信息加载
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
    connect(behavior, SIGNAL(PlayerKeepSignal(Action)), player, SLOT(keepAction(Action)));
    connect(behavior,
            SIGNAL(SoundPlayerLoadNewActionSignal(Action)),
            soundPlayer,
            SLOT(loadAction(Action)));
    connect(behavior,
            SIGNAL(SoundPlayerPathPlay(QString, int)),
            liveSoundPlayer,
            SLOT(loadPathAndPlay(QString, int)));

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

    player->loadAction(StandFacingRight);
    soundPlayer->loadAction(StandFacingRight);
    behavior->loadAction(StandFacingRight);
    behavior->loadPos(SCREENWIDTHFIX + 100, SCREENHEIGHTFIX + SCREENHEIGHT - ORIHEIGHT);

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

    // 关于动作
    aboutAction = new QAction(tr("关于"), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutShowSlot()));

    // 控制动作
    controlAction = new QAction(tr("是否启用交互"), this);
    controlAction->setCheckable(true);  // 设置为可勾选
    controlAction->setChecked(true);    // 设置初始状态为勾选
    connect(controlAction, &QAction::toggled, [this](bool control) {
        if (control)
        {
            setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                           Qt::NoDropShadowWindowHint);
        }
        else
        {
            setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                           Qt::NoDropShadowWindowHint | Qt::WindowTransparentForInput);
        }
        show();
    });

    // 音量控制
    volumeAction = new QWidgetAction(this);
    // 创建一个小部件容器，用于放置滑块和标签
    QWidget*     volumeWidget = new QWidget(this);
    QVBoxLayout* mainLayout   = new QVBoxLayout(volumeWidget);
    // 创建一个标签用于显示当前音量
    QLabel* volumeLabel = new QLabel(tr("音量：") + QString::number(20));
    volumeLabel->setAlignment(Qt::AlignCenter);  // 让标签居中
    // 创建一个水平布局用于放置滑块及其两端的标签
    QHBoxLayout* sliderLayout = new QHBoxLayout();
    // 创建滑块并设置范围
    QSlider* volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);  // 设置音量范围
    volumeSlider->setValue(20);      // 默认值
    volumeSlider->setStyleSheet("QSlider::handle:horizontal {"
                                "    background: white;"  // 滑块的颜色
                                "}"
                                ""
                                "QSlider::handle:horizontal:pressed {"
                                "    background: #87ceeb;"  // 滑块被按下时的颜色
                                "}"
                                ""
                                "QSlider::sub-page:horizontal {"
                                "    background: lightblue;"  // 滑槽左侧（已滑动部分）的颜色
                                "}"
                                ""
                                "QSlider::add-page:horizontal {"
                                "    background: #cccccc;"  // 滑槽右侧（未滑动部分）的颜色
                                "}");
    // 创建滑块两端的标签，显示最小值和最大值
    QLabel* minLabel = new QLabel("0");
    QLabel* maxLabel = new QLabel("100");
    // 将最小值标签、滑块和最大值标签添加到水平布局
    sliderLayout->addWidget(minLabel);
    sliderLayout->addWidget(volumeSlider);
    sliderLayout->addWidget(maxLabel);
    // 将标签和滑块布局添加到主布局中
    mainLayout->addWidget(volumeLabel);
    mainLayout->addLayout(sliderLayout);
    // 连接滑块的 valueChanged 信号来更新标签的文本
    connect(volumeSlider, &QSlider::valueChanged, [volumeLabel](int value) {
        volumeLabel->setText(tr("音量：") + QString::number(value));
    });
    connect(volumeSlider, SIGNAL(valueChanged(int)), soundPlayer, SLOT(setVolume(int)));
    connect(volumeSlider, SIGNAL(valueChanged(int)), liveSoundPlayer, SLOT(setVolume(int)));
    // 将 volumeWidget 设置为默认小部件
    volumeAction->setDefaultWidget(volumeWidget);

    // 动作限制
    limitAction = new QAction(tr("是否启用动作限制"), this);
    limitAction->setCheckable(true);  // 设置为可勾选
    limitAction->setChecked(false);   // 设置初始状态为勾选
    connect(limitAction, SIGNAL(toggled(bool)), behavior, SLOT(setLimitable(bool)));

    // 基准设置
    baseSetAction = new QAction(tr("水平基准设置"), this);
    connect(baseSetAction, SIGNAL(triggered(bool)), this, SLOT(baseInputShowSlot()));

    // debug信息显示
    debugAction = new QAction(tr("调试信息显示"), this);
    debugAction->setCheckable(true);  // 设置为可勾选
    debugAction->setChecked(false);   // 设置初始状态为勾选
}

void MainWindow::createTrayMenu()
{
    // 创建托盘菜单并添加退出动作
    trayMenu = new QMenu(this);
    trayMenu->addAction(volumeAction);
    trayMenu->addAction(controlAction);
    trayMenu->addAction(limitAction);
    trayMenu->addAction(baseSetAction);
    // 调试信息显示
    trayMenu->addAction(debugAction);
    trayMenu->addAction(aboutAction);
    trayMenu->addAction(quitAction);
}

void MainWindow::createClickMenu()
{
    // 创建右键菜单并添加退出动作
    clickMenu = new QMenu(this);
    createActionMenu();
    clickMenu->addMenu(actionMenu);
    clickMenu->addAction(controlAction);
    clickMenu->addAction(limitAction);
    clickMenu->addAction(quitAction);
}

void MainWindow::createActionMenu()
{
    actionMenu = new QMenu(tr("动作"));
    actionMenu->setStyleSheet("QMenu::icon { width: 0px; }");
    QAction* actCuteAction = new QAction(tr("卖萌"), this);
    actionMenu->addAction(actCuteAction);
    connect(actCuteAction, &QAction::triggered, behavior, [=]() {
        if (behavior->getY() == behavior->BottomEdge)
        {
            Action actCute = ActionsMap[behavior->getAction()].transform ? StandLefttoActCuteRight
                                                                         : StandRighttoActCuteLeft;
            behavior->loadAction(actCute);
        }
    });

    //    QAction* sleepAction = new QAction(tr("睡觉"), this);
    //    actionMenu->addAction(sleepAction);
}

void MainWindow::actionMenuUpdate()
{
    if (behavior->getY() != behavior->BottomEdge)
        actionMenu->setEnabled(false);
    else
        actionMenu->setEnabled(true);
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

    // 更新动作菜单可选性
    actionMenuUpdate();

    // debug窗口是否显示
    if (debugAction->isChecked())
        debugMessageShow();
    else
        debugLabel->clear();
}

// 关于对话框
void MainWindow::aboutShowSlot()
{
    AboutDialog aboutDialog(this);
    aboutDialog.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    aboutDialog.setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |
                               Qt::FramelessWindowHint);
    aboutDialog.exec();
}

// 基准输入对话框
void MainWindow::baseInputShowSlot()
{
    QInputDialog dialog(this);

    dialog.setStyleSheet(R"(
                         QInputDialog {
                             background-color: rgba(43, 45, 48, 255);
                             border-radius: 5px;
                         }

                         QInputDialog QLabel {
                             color: white;
                             font-size: 20px;
                         }

                         QInputDialog QLineEdit {
                             background-color: rgba(43, 45, 48, 255);
                             color: black;
                             border-radius: 5px;
                         }

                         QInputDialog QPushButton {
                             background-color: rgba(83, 85, 88, 0.7);
                             color: white;
                         }
                     )");

    dialog.setWindowTitle(tr("水平基准修改"));
    dialog.setLabelText(tr("请输入水平基准值\n重启桌宠生效"));
    dialog.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    dialog.setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

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

    // 设置输入框的默认值、范围和步长
    dialog.setIntValue(value);
    dialog.setIntMinimum(0);
    dialog.setIntMaximum(100);
    dialog.setIntStep(1);

    dialog.setOkButtonText(tr("确定"));
    dialog.setCancelButtonText(tr("取消"));
    dialog.setInputMode(QInputDialog::IntInput);

    // 显示对话框并获取输入结果
    if (dialog.exec() == QDialog::Accepted)
    {
        int newValue = dialog.intValue();

        QFile file("baseMark.data");

        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            out << newValue;  // 将整数写入文件
        }
        // 关闭文件
        file.close();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (controlAction->isChecked())
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

void MainWindow::debugMessageShow()
{
    debugLabel->setText(behavior->debugMessage);
    debugLabel->resize(500, 500);
    debugLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0); color: white;");
    // 显示 QLabel
    debugLabel->move(2400, 1100);
    debugLabel->show();
}
