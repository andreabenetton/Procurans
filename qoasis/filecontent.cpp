// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>
#include <QList>
#include <QSaveFile>

#include "filecontent.h"
#include "office/documentcontent.h"

namespace qoasis {

    const QLatin1String FileContent::FileName = QLatin1String("content.xml");

    FileContent::FileContent(const QString& full_path) : FileXML(full_path)
    {

    }

    QLatin1String FileContent::InstanceFileName()
    {
        return FileContent::FileName;
    }

    QLatin1String FileContent::GetRootTag()
    {
        return DocumentContent::kTag;
    }

    bool FileContent::Create()
    {
        content_ = QSharedPointer<DocumentContent>(new DocumentContent());
        return true;
    }

    void FileContent::Read(QXmlStreamReader& reader)
    {
        content_ = QSharedPointer<DocumentContent>(new DocumentContent(reader));
    }

    void FileContent::Write(QXmlStreamWriter* writer)
    {
        content_->Write(writer);
    }

    QSharedPointer<DocumentContent> FileContent::GetContent()
    {
        return content_;
    }
}
