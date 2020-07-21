// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "mainwindow.h"
#include "Logger.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include <QDebug>

bool doDebug = false;

void messageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
        Logger *globalLogger = Logger::getInstance();
        if ((type!= QtMsgType::QtDebugMsg)|| doDebug)
            globalLogger->messageHandler(type, ctx, msg);
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application); 

    QApplication app(argc, argv);
    QApplication::setOrganizationName(COMPANY_NAME); //defined in CMakeList.txt
    QApplication::setApplicationName(PRODUCT_NAME); //defined in CMakeList.txt
    QApplication::setApplicationVersion(APP_VERSION); //defined in CMakeList.txt

    Logger *globalLogger = Logger::getInstance();
    globalLogger->Setup(QCoreApplication::applicationDirPath(), "Procurans.log");

    qInstallMessageHandler(messageHandler);
    qInfo(logInfo()) << PRODUCT_NAME << " activated. Version:" << APP_VERSION << " Folder:" << QApplication::applicationDirPath();

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    QCommandLineOption debugOption(QStringList() << "d" << "debug",
        QCoreApplication::translate("debug", "Activate debug messages in log file."));
    parser.addOption(debugOption);
    parser.process(app);
    doDebug = parser.isSet(debugOption);

    MainWindow mainWin;
    if (!parser.positionalArguments().isEmpty()){
        QString arg = parser.positionalArguments().first();
        qInfo(logInfo())  << "Called with argument: " << arg;
        mainWin.parseXMLFile(arg);
    }
    mainWin.setWindowIcon(QIcon(":/images/split.png"));
    mainWin.setWindowTitle(QString(PRODUCT_NAME) + " " + APP_VERSION);
    mainWin.show();
    int ret = app.exec();
    qInfo(logInfo()) << PRODUCT_NAME << " terminated. Exit code:" << ret;
    return ret;
}



