// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef DOCUMENTCONTENT_H
#define DOCUMENTCONTENT_H

#include <QMap>

#include "../tag.h"

#include "body.h"

namespace qoasis::office {

    class DocumentContent : public Tag
    {
    public:
        DocumentContent();
        DocumentContent(QXmlStreamReader& reader);
        DocumentContent(const DocumentContent &obj);

        static QSharedPointer<Tag> Builder(QXmlStreamReader& reader);

        static const QLatin1String kTag;

        static const QLatin1String kNsOfficeAttribute;
        static const QLatin1String kNsDcAttribute;
        static const QLatin1String kNsDrawAttribute;
        static const QLatin1String kNsFoAttribute;
        static const QLatin1String kNsNumberAttribute;
        static const QLatin1String kNsOfAttribute;
        static const QLatin1String kNsStyleAttribute;
        static const QLatin1String kNsSvgAttribute;
        static const QLatin1String kNsTableAttribute;
        static const QLatin1String kNsTextAttribute;
        static const QLatin1String kNsXlinkAttribute;

        static const QLatin1String kVersionAttribute;

        QString GetVersion();
        QSharedPointer<Body> GetBody();

        // implements Tag
        virtual QLatin1String InstanceTag();

    protected:
        // implements Tag
        virtual void ReadSubtag(QXmlStreamReader& reader);
        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);

    private:
        QSharedPointer<Body> body_;
        QMap<QString, QString> namespaces_;
        QString version_;
    };
}
#endif // DOCUMENTCONTENT_H
