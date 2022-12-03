#include "control/Cnc.h"
#include "control/models/SettingsModel.h"
#include "control/models/GCodeModel.h"
#include "control/widgets/MainWindow.h"
#include "control/widgets/SmallMainWindow.h"

#include <QApplication>
#include <QCommandLineParser>

constexpr int SMALL_MAIN_WINDOW_WIDTH = 600;
constexpr int SMALL_MAIN_WINDOW_HEIGHT = 1024;

int runApp(bool small, bool fullscreen)
{
    auto settings = SettingsModel::loadOrDefault();
    auto gcodeModel = new GCodeModel;
    auto cnc = new Cnc(gcodeModel);

    QWidget* mainWindow;
    if (small)
    {
        mainWindow = new SmallMainWindow(settings, gcodeModel, cnc);
        mainWindow->setMinimumSize(SMALL_MAIN_WINDOW_WIDTH, SMALL_MAIN_WINDOW_HEIGHT);
        mainWindow->setMaximumSize(SMALL_MAIN_WINDOW_WIDTH, SMALL_MAIN_WINDOW_HEIGHT);
    }
    else
    {
        mainWindow = new MainWindow(settings, gcodeModel, cnc);
        mainWindow->setMinimumHeight(SMALL_MAIN_WINDOW_HEIGHT);
    }

    mainWindow->setStyleSheet("QWidget {font-size: 14pt;} "
                              "QPushButton {qproperty-iconSize: 23px;} "
                              "QGroupBox {font-weight: bold;} "
                              "QLCDNumber {min-height: 34px;} "
                              "QChartView {padding: 0px; margin: 0px;}");
    mainWindow->show();

    if (fullscreen)
    {
        mainWindow->setWindowState(Qt::WindowFullScreen);
    }

    int code = QApplication::exec();

    settings->save();
    delete mainWindow;
    delete cnc;
    delete gcodeModel;
    delete settings;

    return code;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("mamaheux");
    QCoreApplication::setOrganizationDomain("mamaheux.com");
    QCoreApplication::setApplicationName("CNC - Control");

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption fullscreenOption("fullscreen");
    parser.addOption(fullscreenOption);
    QCommandLineOption smallOption("small");
    parser.addOption(smallOption);
    parser.process(app);

    return runApp(parser.isSet(smallOption), parser.isSet(fullscreenOption));
}
