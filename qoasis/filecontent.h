// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FILECONTENT_H
#define FILECONTENT_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "office/documentcontent.h"
#include "filexml.h"

using namespace qoasis::office;

namespace qoasis {

    class FileContent : public FileXML
    {
    public:
        FileContent(const QString& full_path);

        virtual bool Create();
        QSharedPointer<DocumentContent> GetContent();
        QLatin1String InstanceFileName();

        static const QLatin1String FileName;

    protected:
        QLatin1String GetRootTag();   
        virtual void Read(QXmlStreamReader& xml);
        virtual void Write(QXmlStreamWriter* xml);

        QSharedPointer<DocumentContent> content_;
    };
}
#endif // FILECONTENT_H
