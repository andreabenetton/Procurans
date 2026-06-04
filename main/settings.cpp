// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "settings.h"
#include "Logger.h"

#include <QtWidgets>

Settings::Settings()
{

    QString appdir = QApplication::applicationDirPath();
    QString inifilenameandpath = QDir(appdir).filePath("Procurans.ini");

    //settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    settings = new QSettings(inifilenameandpath, QSettings::IniFormat);

    executeElencoFatture = true;
    executeMastriniFornitori = true;
    executePrimaNota = true;
    executeScadenziario = true;
    executeBackupFiles = true;

    updated = false;
}

void Settings::load()
{
    settings->beginGroup("mainwindow");
    mainwindowgeometry = settings->value("geometry").toByteArray();
    mainwindowstate = settings->value("state", Qt::WindowMaximized).toByteArray();
    settings->endGroup();
    settings->beginGroup("odspaths");
    pathElencoFatture = (settings->value("elencofatture", QDir::currentPath())).toString();
    pathMastriniFornitori = (settings->value("mastrinifornitori", QDir::currentPath())).toString();
    pathPrimaNota = (settings->value("primanota", QDir::currentPath())).toString();
    pathScadenziario = (settings->value("scadenziario", QDir::currentPath())).toString();
    pathFattureElettroniche = (settings->value("fatturelettroniche", QDir::currentPath())).toString();
    settings->endGroup();
    settings->beginGroup("othersettings");
    executeBackupFiles = (settings->value("backupfiles", true)).toBool();
    settings->endGroup();

    settings->beginGroup("bankaccounts");
    const QStringList ibans = settings->childKeys();
    bankAccounts.clear();
    if (ibans.isEmpty())
    {
        // First run: seed with the historic in-code default. Mark `updated`
        // so save() will persist them, giving the user a populated section
        // in Procurans.ini they can edit.
        bankAccounts.insert(QStringLiteral("IT64U0503451861000000001728"),
                            QStringLiteral("BPM"));
        bankAccounts.insert(QStringLiteral("IT23P0503451861000000001817"),
                            QStringLiteral("BPM Fotovoltaico"));
        bankAccounts.insert(QStringLiteral("IT65Z0306951030615272528476"),
                            QStringLiteral("Intesa"));
        bankAccounts.insert(QStringLiteral("IT33U0843051030000000180277"),
                            QStringLiteral("CRAC"));
        updated = true;
    }
    else
    {
        for (const QString& iban : ibans)
        {
            bankAccounts.insert(iban, settings->value(iban).toString());
        }
    }
    settings->endGroup();
}

void Settings::save()
{
    if(updated){
        settings->beginGroup("mainwindow");
        settings->setValue("state", mainwindowstate);
        settings->setValue("geometry", mainwindowgeometry);
        settings->endGroup();
        settings->beginGroup("odspaths");
        settings->setValue("elencofatture", pathElencoFatture);
        settings->setValue("mastrinifornitori", pathMastriniFornitori);
        settings->setValue("primanota", pathPrimaNota);
        settings->setValue("scadenziario", pathScadenziario);
        settings->setValue("fatturelettroniche", pathFattureElettroniche);
        settings->endGroup();
        settings->beginGroup("othersettings");
        settings->setValue("backupfiles", executeBackupFiles);
        settings->endGroup();
        settings->beginGroup("bankaccounts");
        settings->remove(QString());        // wipe stale IBANs in the group
        for (auto it = bankAccounts.constBegin();
             it != bankAccounts.constEnd(); ++it)
        {
            settings->setValue(it.key(), it.value());
        }
        settings->endGroup();
    }
}

const QHash<QString, QString>& Settings::getBankAccounts() const
{
    return bankAccounts;
}

void Settings::setBankAccounts(const QHash<QString, QString>& accounts)
{
    if (bankAccounts != accounts)
    {
        bankAccounts = accounts;
        qInfo(logInfo()) << "Bank accounts updated, count=" << bankAccounts.size();
        updated = true;
    }
}

void Settings::restore(QMainWindow* window)
{
    window->restoreGeometry(mainwindowgeometry);
    window->restoreState(mainwindowstate, YOUR_UI_VERSION);
}

void Settings::backup(QMainWindow* window)
{
    if(mainwindowgeometry != window->saveGeometry())
    {
        mainwindowgeometry = window->saveGeometry();
        updated = true;
    }
    if(mainwindowstate != window->saveState())
    {
        mainwindowstate = window->saveState();
        updated = true;
    }
}

void Settings::setPath(QMainWindow* window, Execute p)
{
    QString path = QFileDialog::getExistingDirectory
    (
          window,
          "Seleziona cartella",
          Settings::getPath(p)
    );
    if (!path.isEmpty()) {
        switch (p)
        {
        case Execute::elencofatture:
            if(pathElencoFatture != path)
            {
                pathElencoFatture = path;
                qInfo(logInfo())  << "ElencoFatture path updated: " << pathElencoFatture;
                updated = true;
            }
            break;
        case Execute::scadenziario:
            if(pathScadenziario != path)
            {
                pathScadenziario = path;
                qInfo(logInfo())  << "Scadenziario path updated: " << pathScadenziario;
                updated = true;
            }
            break;
        case Execute::primanota:
            if(pathPrimaNota != path)
            {
                pathPrimaNota = path;
                qInfo(logInfo())  << "PrimaNota path updated: " << pathPrimaNota;
                updated = true;
            }
            break;
        case Execute::mastrinifornitori:
            if(pathMastriniFornitori != path)
            {
                pathMastriniFornitori = path;
                qInfo(logInfo())  << "MastriniFornitori path updated: " << pathMastriniFornitori;
                updated = true;
            }
            break;
        case Execute::fattureelettroniche:
            if (pathFattureElettroniche != path)
            {
                pathFattureElettroniche = path;
                qInfo(logInfo()) << "FattureElettroniche path updated: " << pathFattureElettroniche;
                updated = true;
            }
            break;
        default:
          qWarning(logWarning())  << "Failure to decode setPath";
        }
    }
}

QString Settings::getPath(Execute p)
{
    switch (p)
    {
        case Execute::elencofatture:
            return pathElencoFatture;
            break;
        case Execute::scadenziario:
            return pathScadenziario;
            break;
        case Execute::primanota:
            return pathPrimaNota;
            break;
        case Execute::mastrinifornitori:
            return pathMastriniFornitori;
            break;
        case Execute::fattureelettroniche:
            return pathFattureElettroniche;
            break;
        default:{
            qWarning(logWarning())  << "Failure to decode getPath, reverting to QDir::currentPath";
            return QDir::currentPath();
        }
    }
}

void Settings::toggleExecute(Execute p)
{
    switch (p)
    {
        case Execute::elencofatture:{
            executeElencoFatture = !executeElencoFatture;
            qInfo(logInfo())  << "Toggled ElencoFatture: " << executeElencoFatture;
            break;
        }
        case Execute::scadenziario:{
            executeScadenziario = !executeScadenziario;
            qInfo(logInfo())  << "Toggled Scadenziario: " << executeScadenziario;
            break;
        }
        case Execute::primanota:{
            executePrimaNota = !executePrimaNota;
            qInfo(logInfo())  << "Toggled PrimaNota: " << executePrimaNota;
            break;
        }
        case Execute::mastrinifornitori:{
            executeMastriniFornitori = !executeMastriniFornitori;
            qInfo(logInfo())  << "Toggled MastriniFornitori: " << executeMastriniFornitori;
            break;
        }
        case Execute::backupfiles:{
            executeBackupFiles = !executeBackupFiles;
            qInfo(logInfo())  << "Toggled BackupFiles: " << executeBackupFiles;
            break;
        }
        default:{
            qWarning(logWarning())  << "Failure to decode toggleExecute";
        }
    }
}

bool Settings::isExecute(Execute p)
{
    switch (p)
    {
        case Execute::elencofatture:
            return executeElencoFatture;
            break;
        case Execute::scadenziario:
            return executeScadenziario;
            break;
        case Execute::primanota:
            return executePrimaNota;
            break;
        case Execute::mastrinifornitori:
            return executeMastriniFornitori;
            break;
        case Execute::backupfiles:
            return executeBackupFiles;
            break;
        default:{
            qWarning(logWarning())  << "Failure to decode isExecute";
            return false;
        }
    }
}
