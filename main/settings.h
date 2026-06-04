// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QHash>
#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QSettings;
QT_END_NAMESPACE

#define YOUR_UI_VERSION 1

class Settings
{
    public:
        static Settings& getInstance()
        {
            static Settings instance; // Guaranteed to be destroyed.
                                      // Instantiated on first use.
            return instance;
        }

        enum class Execute{elencofatture, mastrinifornitori, primanota, scadenziario, backupfiles, fattureelettroniche
        };

        void load();
        void save();
        void restore(QMainWindow* window);
        void backup(QMainWindow* window);

        void setPath(QMainWindow* window, Execute p);
        QString getPath(Execute p);

        void toggleExecute(Execute p);
        bool isExecute(Execute p);

        // IBAN -> bank display name. Persisted in Procurans.ini under
        // [bankaccounts]; seeded with a built-in default if absent.
        const QHash<QString, QString>& getBankAccounts() const;
        void setBankAccounts(const QHash<QString, QString>& accounts);

        Settings(Settings const&)        = delete;
        void operator=(Settings const&)  = delete;

    private:
        Settings();

        QSettings* settings;
        QByteArray mainwindowstate;
        QByteArray mainwindowgeometry;
        QString pathFattureElettroniche;
        QString pathElencoFatture;
        QString pathMastriniFornitori;
        QString pathPrimaNota;
        QString pathScadenziario;

        bool executeElencoFatture;
        bool executeMastriniFornitori;
        bool executePrimaNota;
        bool executeScadenziario;
        bool executeBackupFiles;

        QHash<QString, QString> bankAccounts;

        bool updated;
};

#endif // SETTINGS_H
