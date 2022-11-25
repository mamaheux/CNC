#include "control/widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.setStyleSheet(
        "QWidget {font-size: 15pt;} "
        "QPushButton {qproperty-iconSize: 25px;}");
    mainWindow.show();
    return a.exec();
}
