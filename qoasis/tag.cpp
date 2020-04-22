// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tag.h"

namespace qoasis {

    // Constructors
    Tag::Tag()
    {
        Initialize();
    }

    Tag::Tag(QXmlStreamReader& reader) : Tag()
    {
        Read(reader);
    }

    Tag::Tag(const Tag &obj)
    {
        Initialize();
        // deep copy on attributes
        QMapIterator<QString, QString> i(obj.attributes_);
        while (i.hasNext()) {
            i.next();
            attributes_.insert(i.key(), i.value());
        }

        // deep copy on subitems
        for (int i = 0; i < subtags_->length(); i++) {
            QSharedPointer<Tag> sub = obj.subtags_->at(i);
            if (!sub.isNull()) {
                subtags_->replace(i, QSharedPointer<Tag>(new Tag(*sub)));
            }
        }
    }

    Tag::~Tag()
    {
        delete subtags_;
    }

    // Methods
    void Tag::Initialize()
    {
        subtags_ = new QVector<QSharedPointer<Tag>>();
    }

    QLatin1String Tag::InstanceTag()
    {
        return QLatin1String("");
    }

    void Tag::Read(QXmlStreamReader& reader)
    {
        LoopToReadAttributes(reader);
        LoopToReadSubtag(reader);
    }

    void Tag::Write(QXmlStreamWriter* writer)
    {
        WriteStart(writer);
        WriteAttributes(writer);
        WriteSubtags(writer);
        WriteEnd(writer);
    }

    void Tag::WriteStart(QXmlStreamWriter* writer)
    {
        writer->writeStartElement(InstanceTag());
    }

    void Tag::WriteAttributes(QXmlStreamWriter* writer)
    {
        QMapIterator<QString, QString> i(attributes_);
        while (i.hasNext()) {
            i.next();
            writer->writeAttribute(i.key(), i.value());
        }
    }

    void Tag::WriteSubtags(QXmlStreamWriter* writer)
    {
        for (int i = 0; i < subtags_->length(); i++) {
            Tag* subtag = (*subtags_)[i].get();
            if (subtag != nullptr) {
                subtag->Write(writer);
            }
        }
    }

    void Tag::WriteEnd(QXmlStreamWriter* writer)
    {
        writer->writeEndElement();
    }

    void Tag::LoopToReadAttributes(QXmlStreamReader& reader)
    {
        for (auto& it : reader.attributes()) {
            ReadAttribute(it.qualifiedName(), it.value());
        }
    }

    void Tag::ReadAttribute(QStringRef attributename, QStringRef attributevalue)
    {
        attributes_.insert(attributename.toString(), attributevalue.toString());
    }

    void Tag::LoopToReadSubtag(QXmlStreamReader& reader)
    {
        do {
            reader.readNext();
            ReadSubtag(reader);
        } while (IsNotEndElementNamed(reader, InstanceTag()));
    }

    void Tag::ReadSubtag(QXmlStreamReader& reader)
    {
        subtags_->append(QSharedPointer<Tag>(new Tag(reader)));
    }

    bool Tag::IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.qualifiedName() == tokenName));
    }

    bool Tag::IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.qualifiedName() == tokenName));
    }
}
