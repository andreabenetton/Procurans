// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "documentcontent.h"

namespace qoasis::office {

    // Constants
    const QLatin1String DocumentContent::kTag = QLatin1String("office:document-content");

    const QLatin1String DocumentContent::kNsOfficeAttribute = QLatin1String("xmlns:office");
    const QLatin1String DocumentContent::kNsDcAttribute = QLatin1String("xmlns:dc");
    const QLatin1String DocumentContent::kNsDrawAttribute = QLatin1String("xmlns:draw");
    const QLatin1String DocumentContent::kNsFoAttribute = QLatin1String("xmlns:fo");
    const QLatin1String DocumentContent::kNsNumberAttribute = QLatin1String("xmlns:number");
    const QLatin1String DocumentContent::kNsOfAttribute = QLatin1String("xmlns:of");
    const QLatin1String DocumentContent::kNsStyleAttribute = QLatin1String("xmlns:style");
    const QLatin1String DocumentContent::kNsSvgAttribute = QLatin1String("xmlns:svg");
    const QLatin1String DocumentContent::kNsTableAttribute = QLatin1String("xmlns:table");
    const QLatin1String DocumentContent::kNsTextAttribute = QLatin1String("xmlns:text");
    const QLatin1String DocumentContent::kNsXlinkAttribute = QLatin1String("xmlns:xlink");

    const QLatin1String DocumentContent::kVersionAttribute = QLatin1String("office:version");

    // Constructors
    DocumentContent::DocumentContent() : Tag()
    {
        namespaces_.insert(kNsOfficeAttribute, "urn:oasis:names:tc:opendocument:xmlns:office:1.0");
        namespaces_.insert(kNsDcAttribute, "http://purl.org/dc/elements/1.1/");
        namespaces_.insert(kNsDrawAttribute, "urn:oasis:names:tc:opendocument:xmlns:drawing:1.0");
        namespaces_.insert(kNsFoAttribute, "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0");
        namespaces_.insert(kNsNumberAttribute, "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0");
        namespaces_.insert(kNsOfAttribute, "urn:oasis:names:tc:opendocument:xmlns:of:1.2");
        namespaces_.insert(kNsStyleAttribute, "urn:oasis:names:tc:opendocument:xmlns:style:1.0");
        namespaces_.insert(kNsSvgAttribute, "urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0");
        namespaces_.insert(kNsTableAttribute, "urn:oasis:names:tc:opendocument:xmlns:table:1.0");
        namespaces_.insert(kNsTextAttribute, "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
        namespaces_.insert(kNsXlinkAttribute, "http://www.w3.org/1999/xlink");

        version_ = "1.2";

        body_ = QSharedPointer<Body>(new Body());
    }

    DocumentContent::DocumentContent(QXmlStreamReader& reader) : Tag()
    {
        Q_ASSERT(reader.qualifiedName() == DocumentContent::kTag);
        read(reader);
    }

    DocumentContent::DocumentContent(const DocumentContent &obj)
    {
        body_ = QSharedPointer<Body>(new Body(*obj.body_));
    }

    // Static methods
    QSharedPointer<Tag> DocumentContent::builder(QXmlStreamReader& reader)
    {
        Q_ASSERT(reader.qualifiedName() == DocumentContent::kTag);
        return QSharedPointer<Tag>(new DocumentContent(reader));
    }

    // Methods
    QString DocumentContent::getVersion() const
    {
        return version_;
    }

    QSharedPointer<Body> DocumentContent::getBody() const
    {
        return body_;
    }

    // implements Tag
    QLatin1String DocumentContent::instanceTag()
    {
        return DocumentContent::kTag;
    }

    void DocumentContent::readAttribute(QStringRef name, QStringRef value)
    {
        if(name.split(':').first()=="xmlns") {
            namespaces_.insert(name.toString(), value.toString());
            return;
        }
        if(name==kVersionAttribute) {
            version_ = value.toString();
            return;
        }
        // Deserialize present but unsupported attributes
        Tag::readAttribute(name, value);
    }

    void DocumentContent::readSubtag(QXmlStreamReader& reader)
    {
        if (isStartElementNamed(reader, Body::kTag)) {
            body_ = QSharedPointer<Body>(new Body(reader));
            return;
        }
        // Deserialize present but unsupported subtags
        Tag::readSubtag(reader);
    }

    void DocumentContent::writeAttributes(QXmlStreamWriter* writer)
    {
        QMapIterator<QString, QString> i(namespaces_);
        while (i.hasNext()) {
            i.next();
            writer->writeAttribute(i.key(), i.value());
        }
        writer->writeAttribute(kVersionAttribute, version_);

        // Serialize present but unsupported attributes
        Tag::writeAttributes(writer);
    }

    void DocumentContent::writeSubtags(QXmlStreamWriter* writer)
    {
        if (body_ != nullptr) {
            body_->write(writer);
        }
        // Serialize present but unsupported subtags
        Tag::writeSubtags(writer);
    }
}
