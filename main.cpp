#include <QApplication>

#include "mainwindow.h"
#include "player.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // 创建样式表
    QString styleSheet = R"(
                         QMenu {
                             padding: 5px 0;
                             border-radius: 5px;
                             background-color: rgba(43, 45, 48, 255);
                         }

                         QMenu::item {
                             color: white;
                             background-color: transparent;
                         }

                         QMenu::item:hover,
                         QMenu::item:pressed,

                         QMenu::item:selected {
                             background-color: rgba(135, 206, 250, 0.7);
                         }

                         QMenu::item:disabled {
                             color: gray;
                         }

                         QMenu::item QLabel {
                             color: white;
                             font-weight: bold;
                             padding-left: 7px;
                             margin-bottom: 5px;
                             background-color: transparent;
                         }

    )";
    // 应用样式表
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}
