// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QSharedPointer>
#include <QMap>
#include <QVector>

namespace qoasis {

    class Tag
	{
	public:
        Tag(const Tag &obj);
        virtual ~Tag();

        virtual void Write(QXmlStreamWriter* writer);
        virtual QLatin1String InstanceTag();

	protected:
        Tag();
        Tag(QXmlStreamReader& reader);

        virtual void Read(QXmlStreamReader& reader);

        virtual void ReadAttribute(QStringRef attributename, QStringRef attributevalue);
        virtual void ReadSubtag(QXmlStreamReader& reader);

        virtual void WriteAttributes(QXmlStreamWriter* writer);
        virtual void WriteSubtags(QXmlStreamWriter* writer);

        bool IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName);
        bool IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName);

    private:
        void Initialize();

        void LoopToReadAttributes(QXmlStreamReader& reader);
        void LoopToReadSubtag(QXmlStreamReader& reader);

        void WriteStart(QXmlStreamWriter* writer);
        void WriteEnd(QXmlStreamWriter* writer);

        QVector<QSharedPointer<Tag>>* subtags_;
        QMap<QString, QString> attributes_;
    };
}
#endif // TAG_H
