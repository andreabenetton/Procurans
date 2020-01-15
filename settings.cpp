#include "settings.h"

#include <QtWidgets>

Settings::Settings()
{
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    executeElencoFatture = true;
    executeMastriniFornitori = true;
    executePrimaNota = true;
    executeScadenziario = true;

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
                updated = true;
            }
            break;
        case Execute::scadenziario:
            if(pathScadenziario != path)
            {
                pathScadenziario = path;
                updated = true;
            }
            break;
        case Execute::primanota:
            if(pathPrimaNota != path)
            {
                pathPrimaNota = path;
                updated = true;
            }
            break;
        case Execute::mastrinifornitori:
            if(pathMastriniFornitori != path)
            {
                pathMastriniFornitori = path;
                updated = true;
            }
            break;
        default:
          ;
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
    default:
      ;
    }
}

void Settings::toggleExecute(Execute p)
{
    switch (p)
    {
    case Execute::elencofatture:
        executeElencoFatture = !executeElencoFatture;
        break;
    case Execute::scadenziario:
        executeScadenziario = !executeScadenziario;
        break;
    case Execute::primanota:
        executePrimaNota = !executePrimaNota;
        break;
    case Execute::mastrinifornitori:
        executeMastriniFornitori = !executeMastriniFornitori;
        break;
    default:
      ;
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
    default:
      ;
    }
}




