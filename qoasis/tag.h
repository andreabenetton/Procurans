// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TAG_H
#define TAG_H

#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QString>
#include <QXmlStreamReader>

namespace qoasis
{
	class Tag
	{
	public:
		virtual ~Tag() = default;

		virtual void write(QXmlStreamWriter* writer);
		virtual QString instanceTag();

		virtual bool isEmpty();

		QString plainText() const;

		static QSharedPointer<Tag> buildGeneric(QXmlStreamReader& reader);

	protected:
		Tag();
		Tag(QXmlStreamReader& reader);

		virtual void read(QXmlStreamReader& reader);

		virtual void readAttribute(QStringView name, QStringView value);
		virtual void readNamespace(QStringView name, QStringView value);
		virtual void readSubtag(QXmlStreamReader& reader);

		virtual void writeAttributes(QXmlStreamWriter* writer);
		virtual void writeNamespaces(QXmlStreamWriter* writer);
		virtual void writeSubtags(QXmlStreamWriter* writer);

		// Child elements in source order, skipping interleaved text
		// fragments. Subclasses use this when they need to reorder or
		// partition unknown subtags for an ODF-prescribed write order
		// (e.g. table:calculation-settings before table:table per ODF
		// Part 1 §9.1.2). Text fragments don't surface here because the
		// schemas that need ordering don't permit mixed content.
		QList<QSharedPointer<Tag>> childTags() const;

		bool isStartElementNamed(QXmlStreamReader& xml, const QString& token_name);
		//bool isNotEndElementNamed(QXmlStreamReader& xml, const QString& token_name);

	private:
		// Mixed-content child entry. ODF mixed content (e.g.
		// <text:p>a <text:span>bold</text:span> b</text:p>) requires
		// preserving the order of text fragments and child elements. The
		// earlier model held inline_text_ as one concatenated string and
		// subtags_ as a parallel vector, which destroyed the interleave on
		// round-trip (text-first, then all elements). Each Child here is
		// either a text fragment (tag is null) or a child Tag (text is
		// empty); written back in insertion order.
		struct Child
		{
			QString text;
			QSharedPointer<Tag> tag;
		};

		void loopToReadAttributes(QXmlStreamReader& reader);
		void loopToReadNamespaces(QXmlStreamReader& reader);
		void loopToReadSubtag(QXmlStreamReader& reader);

		void writeStart(QXmlStreamWriter* writer);
		void writeEnd(QXmlStreamWriter* writer);

		QString tag_;
		QMap<QString, QString> attributes_;
		QMap<QString, QString> namespaces_;
		QList<Child> children_;
	};
}
#endif // TAG_H
