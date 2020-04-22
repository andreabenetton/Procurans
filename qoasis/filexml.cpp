// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QList>
#include <QSaveFile>

#include "filexml.h"

namespace qoasis {

    FileXML::FileXML(const QString& full_path) : IFileable(full_path)
    {
    }

    bool FileXML::Load()
    {
        if (!Exist()) {
            qCritical() << "File not exists: " << qPrintable(full_path_);
            return false;
        }

        if (!full_path_.endsWith(InstanceFileName())){
            qCritical() << "File name not match specification: " << qPrintable(full_path_);
            return false;
        }

        QFile file(full_path_);

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qCritical() << "Failed to open file: " << qPrintable(file.fileName())
                        << "error :" << qPrintable(file.errorString());
            return false;
        }

        QXmlStreamReader reader(&file);

        do {
            reader.readNext();
            if (IsStartElementNamed(reader, GetRootTag())) {
                Read(reader);
            }
        } while (!reader.atEnd() && !reader.hasError());
        file.close();

        if (reader.hasError()) {
            qCritical() <<  "Error: Failed to parse XML in file "
                         << qPrintable(InstanceFileName()) << ": "
                         << qPrintable(reader.errorString());
            return false;
        }
        else if (file.error() != QFile::NoError) {
            qCritical() << "Cannot read file " << qPrintable(InstanceFileName())
                        << ": " << qPrintable(file.errorString());
            return false;
        }

        qInfo() << qPrintable(InstanceFileName()) << " file loaded.";
        return true;
    }

    bool FileXML::Save(const QString& full_path, bool overwriteprotected)
    {
        if (overwriteprotected && Exist()) {
            qCritical() << "File already exists: " << qPrintable(full_path_);
            return false;
        }

        QString save_path = (full_path=="" ? full_path_ : full_path);

        QFile file(save_path);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(2);
        writer.writeStartDocument();

        Write(&writer);

        writer.writeEndDocument();
        file.close();
        if (file.error() != QFile::NoError) {
          qCritical() << "Cannot write file " << qPrintable(InstanceFileName())
                   << ": " << qPrintable(file.errorString());
          return false;
        }

        qInfo() << qPrintable(InstanceFileName()) << " file saved.";
        return true;
    }

    bool FileXML::Save()
    {
        return FileXML::Save(full_path_);
    }

    bool FileXML::IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.name() == tokenName));
    }

    bool FileXML::IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.name() == tokenName));
    }

}
