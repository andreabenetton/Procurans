// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TAG_H
#define TAG_H

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

		virtual void write(QXmlStreamWriter* writer);
		virtual QString instanceTag();

		virtual bool isEmpty();

	protected:
		Tag();
		Tag(QXmlStreamReader& reader);

		virtual void read(QXmlStreamReader& reader);

		virtual void readAttribute(QStringRef name, QStringRef value);
		virtual void readNamespace(QStringRef name, QStringRef value);
		virtual void readSubtag(QXmlStreamReader& reader);

		virtual void writeAttributes(QXmlStreamWriter* writer);
		virtual void writeNamespaces(QXmlStreamWriter* writer);
		virtual void writeSubtags(QXmlStreamWriter* writer);

		bool isStartElementNamed(QXmlStreamReader& xml, const QString& token_name);
		//bool isNotEndElementNamed(QXmlStreamReader& xml, const QString& token_name);

		QString TokenTypeToString(int type);

	private:
		void loopToReadAttributes(QXmlStreamReader& reader);
		void loopToReadNamespaces(QXmlStreamReader& reader);
		void loopToReadSubtag(QXmlStreamReader& reader);

		void writeStart(QXmlStreamWriter* writer);
		void writeEnd(QXmlStreamWriter* writer);

		QString tag_;
		QMap<QString, QString> attributes_;
		QMap<QString, QString> namespaces_;
		QVector<QSharedPointer<Tag>> subtags_;
	};
}
#endif // TAG_H
