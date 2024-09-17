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

    // 设置对话框布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 个人信息
    QLabel* infoLabel = new QLabel;
    infoLabel->setTextFormat(Qt::RichText);  // 启用富文本支持
    infoLabel->setText(
        "版本：test<br><br>"
        "程序开发: &nbsp;&nbsp;<a href=\"https://space.bilibili.com/504203036\">羽泉</a><br>"
        "素材处理: &nbsp;&nbsp;<a href=\"https://space.bilibili.com/1630447462\">无名</a><br>");
    infoLabel->setOpenExternalLinks(true);  // 启用外部链接点击
    QFont font("Segoe UI", 10);
    font.setBold(true);
    infoLabel->setFont(font);
    layout->addWidget(infoLabel);

    // 添加按钮
    QPushButton* closeButton = new QPushButton("关闭");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);
}
