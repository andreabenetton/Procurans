#include "settings.h"
#include "logger.h"

#include <QtWidgets>

Settings::Settings()
{

    QString appdir = QCoreApplication::applicationDirPath();
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
    mainwindowsize = settings->value("size", QSize(640, 480)).toSize();
    mainwindowposition = settings->value("position", QPoint(100, 100)).toPoint();
    mainwindowstate = settings->value("state", Qt::WindowMaximized).toByteArray();
    settings->endGroup();
    settings->beginGroup("odspaths");
    pathElencoFatture = (settings->value("elencofatture", ".")).toString();
    pathMastriniFornitori = (settings->value("mastrinifornitori", ".")).toString();
    pathPrimaNota = (settings->value("primanota", ".")).toString();
    pathScadenziario = (settings->value("scadenziario", ".")).toString();
    settings->endGroup();
    settings->beginGroup("othersettings");
    executeBackupFiles = (settings->value("backupfiles", ".")).toBool();
    settings->endGroup();
}

void Settings::save()
{
    if(updated){
        settings->beginGroup("mainwindow");
        settings->setValue("size", mainwindowsize);
        settings->setValue("position", mainwindowposition);
        settings->setValue("state", mainwindowstate);
        settings->endGroup();
        settings->beginGroup("odspaths");
        settings->setValue("elencofatture", pathElencoFatture);
        settings->setValue("mastrinifornitori", pathMastriniFornitori);
        settings->setValue("primanota", pathPrimaNota);
        settings->setValue("scadenziario", pathScadenziario);
        settings->endGroup();
        settings->beginGroup("othersettings");
        settings->setValue("backupfiles", executeBackupFiles);
        settings->endGroup();
    }
}

void Settings::restore(QMainWindow* window)
{
    window->resize(mainwindowsize);
    window->move(mainwindowposition);
    window->restoreState(mainwindowstate);
}

void Settings::backup(QMainWindow* window)
{
    if(mainwindowsize != window->size())
    {
        mainwindowsize = window->size();
        updated = true;
    }
    if(mainwindowposition != window->pos())
    {
        mainwindowposition = window->pos();
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
          QDir::currentPath());
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
        default:{
            qWarning(logWarning())  << "Failure to decode getPath";
            return ".";
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




