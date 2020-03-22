// Copyright 2019 - 2019, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "FileAbstract.h"

#include <QDebug>
#include <QList>
#include <QSaveFile>

namespace Ods {

    void FileAbstract::Load(const QString& full_path)
    {
        QFile file(full_path);

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qWarning() << "Failed to open file: " << file.fileName() << "error :" << file.errorString();
            inbuffer = "";
            return;
        }

        QTextStream in(&file);
        inbuffer = in.readAll();

        //    qInfo(logInfo()) << Tag() << " file loaded.";
    }

    bool FileAbstract::Save(const QString& full_path)
    {
        QSaveFile file(full_path);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);


        file.write(outbuffer.toUtf8());
        const bool ok = file.commit();

        if (!ok)
            qWarning() << "Failed to save file: " << full_path;

        //    qInfo(logInfo()) << Tag() << " file saved.";

        return ok;
    }

    bool FileAbstract::IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.name() == tokenName));
    }

    bool FileAbstract::IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.name() == tokenName));
    }

}