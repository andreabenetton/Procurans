// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "odsfile.h"

#include <QDebug>
#include <QList>
#include <QSaveFile>
#include "logger.h"

void ODSFile::Load(const QString &full_path)
{
    QFile file(full_path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning(logWarning()) << "Failed to open file: " << file.fileName() << "error :" <<file.errorString();
        inbuffer = "";
        return;
    }

    QTextStream in(&file);
    inbuffer = in.readAll();

//    qInfo(logInfo()) << Tag() << " file loaded.";
}

bool ODSFile::Save(const QString &full_path)
{
    QSaveFile file(full_path);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);


    file.write(outbuffer.toUtf8());
    const bool ok = file.commit();

    if (!ok)
        qWarning(logWarning()) << "Failed to save file: " << full_path;

//    qInfo(logInfo()) << Tag() << " file saved.";

    return ok;
}

 bool ODSFile::IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName)
{
    return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.name() == tokenName));
}

 bool ODSFile::IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName)
 {
     return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.name() == tokenName));
 }

