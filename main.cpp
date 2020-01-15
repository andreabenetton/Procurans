/****************************************************************************
**
**    Procurans - An utility to import Italian electronic bill into odf files
**    Copyright (C) 2019  Andrea Benetton lnki5538@pm.me
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU Affero General Public License as published
**    by the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU Affero General Public License for more details.
**
**    You should have received a copy of the GNU Affero General Public License
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#define APP_VERSION "0.0.1.0"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("BlueTeam");
    QCoreApplication::setApplicationName("Procurans");
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    if (!parser.positionalArguments().isEmpty())
        mainWin.parseXMLFile(parser.positionalArguments().first());
    mainWin.setWindowIcon(QIcon(":/images/split.png"));
    mainWin.show();
    return app.exec();
}

