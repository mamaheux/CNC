#include "control/Cnc.h"
#include "control/models/SettingsModel.h"
#include "control/models/GCodeModel.h"
#include "control/widgets/MainWindow.h"
#include "control/widgets/SmallMainWindow.h"

#include <QApplication>
#include <QCommandLineParser>

constexpr int SMALL_MAIN_WINDOW_WIDTH = 600;
constexpr int SMALL_MAIN_WINDOW_HEIGHT = 986;

enum class WindowsShowStyle
{
    NORMAL,
    FULLSCREEN,
    MAXIMIZED
};

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s\n", localMsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s\n", localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s\n", localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", localMsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", localMsg.constData());
            break;
    }
}

int runApp(bool small, WindowsShowStyle showStyle, bool frameless, bool mockCnc)
{
    qInstallMessageHandler(messageHandler);

    auto settings = SettingsModel::loadOrDefault();
    auto gcodeModel = new GCodeModel;
    Cnc* cnc;
    if (mockCnc)
    {
        cnc = new CncMock(gcodeModel);
    }
    else
    {
        cnc = new SerialPortCnc(gcodeModel);
    }


    QWidget* mainWindow;
    if (small)
    {
        mainWindow = new SmallMainWindow(settings, gcodeModel, cnc);
        if (showStyle == WindowsShowStyle::NORMAL)
        {
            mainWindow->setMinimumSize(SMALL_MAIN_WINDOW_WIDTH, SMALL_MAIN_WINDOW_HEIGHT);
            mainWindow->setMaximumSize(SMALL_MAIN_WINDOW_WIDTH, SMALL_MAIN_WINDOW_HEIGHT);
        }
    }
    else
    {
        mainWindow = new MainWindow(settings, gcodeModel, cnc);
        mainWindow->setMinimumHeight(SMALL_MAIN_WINDOW_HEIGHT);
    }

    mainWindow->setStyleSheet("QWidget {font-size: 13pt;} "
                              "QPushButton {qproperty-iconSize: 20px;} "
                              "QGroupBox {font-weight: bold;} "
                              "QLCDNumber {min-height: 34px;} "
                              "QChartView {padding: 0px; margin: 0px;} "
                              "QSpinBox { border: 1px solid #ABABAB; border-radius: 3px;} "
                              "QSpinBox::up-button {"
                              "  subcontrol-origin: margin;"
                              "  subcontrol-position: center left;"
                              "  left: 1px;"
                              "  height: 24px;"
                              "  width: 32px;"
                              "}"
                              "QSpinBox::down-button {"
                              "  subcontrol-origin: margin;"
                              "  subcontrol-position: center right;"
                              "  right: 1px;"
                              "  height: 24px;"
                              "  width: 32px;"
                              "}");

    if (frameless)
    {
        mainWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    }
    switch (showStyle)
    {
        case WindowsShowStyle::FULLSCREEN:
            mainWindow->setWindowState(Qt::WindowFullScreen);
        case WindowsShowStyle::NORMAL:
            mainWindow->show();
            break;
        case WindowsShowStyle::MAXIMIZED:
            mainWindow->showMaximized();
            break;
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
    QCommandLineOption maximizedOption("maximized");
    parser.addOption(maximizedOption);
    QCommandLineOption smallOption("small");
    parser.addOption(smallOption);
    QCommandLineOption framelessOption("frameless");
    parser.addOption(framelessOption);
    QCommandLineOption mockCnc("mock_cnc");
    parser.addOption(mockCnc);
    parser.process(app);

    WindowsShowStyle showStyle = WindowsShowStyle::NORMAL;
    if (parser.isSet(fullscreenOption))
    {
        showStyle = WindowsShowStyle::FULLSCREEN;
    }
    else if (parser.isSet(maximizedOption))
    {
        showStyle = WindowsShowStyle::MAXIMIZED;
    }

    return runApp(parser.isSet(smallOption), showStyle, parser.isSet(framelessOption), parser.isSet(mockCnc));
}
