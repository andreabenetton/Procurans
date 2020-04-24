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

namespace qoasis
{
	class Tag
	{
	public:
		virtual ~Tag() = default;
		explicit Tag(const Tag& obj);

		virtual void write(QXmlStreamWriter* writer);
		virtual QLatin1String instanceTag();

	protected:
		Tag();
		Tag(QXmlStreamReader& reader);

		virtual void read(QXmlStreamReader& reader);

		virtual void readAttribute(QStringRef name, QStringRef value);
		virtual void readSubtag(QXmlStreamReader& reader);

		virtual void writeAttributes(QXmlStreamWriter* writer);
		virtual void writeSubtags(QXmlStreamWriter* writer);

		bool isStartElementNamed(QXmlStreamReader& xml, const QString& token_name);
		bool isNotEndElementNamed(QXmlStreamReader& xml, const QString& token_name);

	private:
		void loopToReadAttributes(QXmlStreamReader& reader);
		void loopToReadSubtag(QXmlStreamReader& reader);

		void writeStart(QXmlStreamWriter* writer);
		void writeEnd(QXmlStreamWriter* writer);

		QVector<QSharedPointer<Tag>> subtags_;
		QMap<QString, QString> attributes_;
	};
}
#endif // TAG_H
