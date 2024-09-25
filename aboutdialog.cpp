#include "aboutdialog.h"

#include <QFont>
#include <QLabel>
#include <QLayout>
#include <QMovie>
#include <QPushButton>
#include <QWidget>

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    // 设置窗口标题
    setWindowTitle("关于");
    setWindowIcon(QIcon("Ori.icon"));
    setFixedSize(290, 230);

    // 设置样式表
    setStyleSheet(R"(
                QDialog {
                    background-color: rgba(43, 45, 48, 255);
                    border-radius: 5px;
                }

                QLabel {
                    color: white;
                    font-weight: bold;
                    font-size: 22px;
                }

                QPushButton {
                    background-color: rgba(83, 85, 88, 0.7);
                    color: white;
                    border-radius: 5px;
                    padding: 5px 10px;
                }

                QPushButton:hover {
                    background-color: rgba(103, 105, 108, 0.9);
                    color: white;
                }

                QPushButton:pressed {
                    background-color: rgba(63, 65, 68, 1.0);
                }
            )");

    // 创建 GIF 背景
    QLabel* backgroundLabel = new QLabel(this);
    backgroundLabel->setFixedSize(size());  // 与窗口大小一致
    backgroundLabel->lower();               // 置于底层

    QMovie* movie = new QMovie("background.gif");
    movie->setParent(this);
    backgroundLabel->setMovie(movie);
    movie->setScaledSize(size());
    movie->start();

    // 创建一个用于调整亮度的遮罩层
    QWidget* brightnessOverlay = new QWidget(this);
    brightnessOverlay->setFixedSize(size());
    brightnessOverlay->setStyleSheet(
        "background-color: rgba(0, 0, 0, 90);");  // 使用黑色，50 表示半透明度

    // 设置对话框布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 个人信息
    QLabel* infoLabel = new QLabel(this);
    infoLabel->setTextFormat(Qt::RichText);  // 启用富文本支持
    infoLabel->setText(
        "奥里桌宠<br><br>"
        "Vtest<br><br>"
        "程序: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"https://space.bilibili.com/504203036\" "
        "style=\"color:lightblue; font-weight:bold; text-decoration:underline;\">羽泉</a><br>"
        "素材: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"https://space.bilibili.com/1630447462\" "
        "style=\"color:lightblue; font-weight:bold; text-decoration:underline;\">无名</a><br>");
    infoLabel->setOpenExternalLinks(true);  // 启用外部链接点击
    layout->addWidget(infoLabel);

    // 添加按钮
    QPushButton* closeButton = new QPushButton("关闭", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);

    setLayout(layout);
}
