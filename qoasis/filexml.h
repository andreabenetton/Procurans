// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FILEXML_H
#define FILEXML_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "ifileable.h"
#include "tag.h"

namespace qoasis {

    class FileXML : public IFileable
    {
    public:
        FileXML(const QString& full_path);

        virtual bool Load();
        virtual bool Save(const QString& full_path, bool overwriteprotected = true);
        virtual bool Save();
        virtual QLatin1String InstanceFileName() = 0;

    protected:
        virtual QLatin1String GetRootTag() = 0;
        virtual void Read(QXmlStreamReader& xml) = 0;
        virtual void Write(QXmlStreamWriter* xml) = 0;
        bool IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName);
        bool IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName);
    };
}
#endif // FILEXML_H
