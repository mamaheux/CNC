#include "control/Cnc.h"
#include "control/models/SettingsModel.h"
#include "control/widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("mamaheux");
    QCoreApplication::setOrganizationDomain("mamaheux.com");
    QCoreApplication::setApplicationName("CNC - Control");

    auto settings = SettingsModel::loadOrDefault();
    auto cnc = new Cnc;

    MainWindow mainWindow(settings, cnc);
    mainWindow.setStyleSheet(
        "QWidget {font-size: 15pt;} "
        "QPushButton {qproperty-iconSize: 25px;} "
        "QGroupBox {font-weight: bold;} ");
    mainWindow.show();

    int code = a.exec();

    settings->save();
    delete settings;

    delete cnc;

    return code;
}
