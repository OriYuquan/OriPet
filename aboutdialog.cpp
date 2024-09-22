#include "aboutdialog.h"

#include <QFont>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    // 设置窗口标题
    setWindowTitle("关于");
    setWindowIcon(QIcon("Ori.icon"));
    setFixedSize(250, 200);

    setStyleSheet(R"(
        QDialog {
            background-color: rgba(43, 45, 48, 255);
            border-radius: 5px;
        }

        QLabel {
            color: white;
            font-weight: bold;
            font-size: 20px;
        }

        QPushButton {
            background-color: rgba(83, 85, 88, 0.7);
            color: white;
            border-radius: 5px;
            padding: 5px 10px;
        }

        QPushButton:hover {
            background-color: rgba(83, 85, 88, 1.0);
        }

        QPushButton:pressed {
            background-color: rgba(63, 65, 68, 1.0);
        }
    )");

    // 设置对话框布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 个人信息
    QLabel* infoLabel = new QLabel;
    infoLabel->setTextFormat(Qt::RichText);  // 启用富文本支持
    infoLabel->setText(
        "奥里桌宠<br><br>"
        "版本：test<br><br>"
        "程序开发: &nbsp;&nbsp;<a href=\"https://space.bilibili.com/504203036\" "
        "style=\"color:lightblue; font-weight:bold; text-decoration:underline;\">羽泉</a><br>"
        "素材处理: &nbsp;&nbsp;<a href=\"https://space.bilibili.com/1630447462\" "
        "style=\"color:lightblue; font-weight:bold; text-decoration:underline;\">无名</a><br>");
    infoLabel->setOpenExternalLinks(true);  // 启用外部链接点击
    layout->addWidget(infoLabel);

    // 添加按钮
    QPushButton* closeButton = new QPushButton("关闭");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);
}
