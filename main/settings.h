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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
//class QAction;
//class QMenu;
//class QTableView;
//class QXmlStreamReader;
class QSettings;
QT_END_NAMESPACE

class Settings
{
    public:
        static Settings& getInstance()
        {
            static Settings instance; // Guaranteed to be destroyed.
                                      // Instantiated on first use.
            return instance;
        }

        enum class Execute{elencofatture, mastrinifornitori, primanota, scadenziario, backupfiles};

        void load();
        void save();
        void restore(QMainWindow* window);
        void backup(QMainWindow* window);

        void setPath(QMainWindow* window, Execute p);
        QString getPath(Execute p);

        void toggleExecute(Execute p);
        bool isExecute(Execute p);

        Settings(Settings const&)        = delete;
        void operator=(Settings const&)  = delete;

    private:
        Settings();

        QSettings* settings;
        QSize mainwindowsize;
        QByteArray mainwindowstate;
        QPoint mainwindowposition;
        QString pathElencoFatture;
        QString pathMastriniFornitori;
        QString pathPrimaNota;
        QString pathScadenziario;

        bool executeElencoFatture;
        bool executeMastriniFornitori;
        bool executePrimaNota;
        bool executeScadenziario;
        bool executeBackupFiles;

        bool updated;
};

#endif // SETTINGS_H
