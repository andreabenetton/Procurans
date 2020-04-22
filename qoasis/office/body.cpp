// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "body.h"

namespace qoasis::office {

    // Constants
    const QLatin1String Body::kTag = QLatin1String("office:body");

    // Constructors
    Body::Body()
    {
        spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet());
    }

    Body::Body(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Body::kTag);
        Read(reader);
    }

    Body::Body(const Body &obj)
    {
        spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet(*obj.spreadsheet_));
    }

    // Static methods
    QSharedPointer<Tag> Body::Builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == Body::kTag);
        return QSharedPointer<Tag>(new Body(reader));
    }

    // Methods  
    QSharedPointer<Spreadsheet> Body::GetSpreadsheet()
    {
        return spreadsheet_;
    }

    // implements Tag
    QLatin1String Body::InstanceTag()
    {
        return Body::kTag;
    }

    void Body::ReadSubtag(QXmlStreamReader& reader)
    {
        if (IsStartElementNamed(reader, Spreadsheet::kTag)) {
            spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet(reader));
            return;
        }
        // Deserialize present but unsupported subtags
        Tag::ReadSubtag(reader);
    }

    void Body::WriteSubtags(QXmlStreamWriter* writer)
    {
        if (spreadsheet_ != nullptr) {
            spreadsheet_->Write(writer);
        }
        // Serialize present but unsupported subtags
        Tag::WriteSubtags(writer);
    }
}
