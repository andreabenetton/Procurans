// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QDebug>

#include "tag.h"

namespace qoasis
{
	// Constructors
	Tag::Tag()
	{
	}

	Tag::Tag(QXmlStreamReader& reader)
	{
		read(reader);
	}

	// Methods
	QString Tag::instanceTag()
	{
		return tag_;
	}

	bool Tag::isEmpty()
	{
		return children_.isEmpty();
	}

	QSharedPointer<Tag> Tag::buildGeneric(QXmlStreamReader& reader)
	{
		return QSharedPointer<Tag>(new Tag(reader));
	}

	QString Tag::plainText() const
	{
		QString out;
		for (const auto& c : children_)
		{
			if (c.tag.isNull())
			{
				out += c.text;
			}
			else
			{
				out += c.tag->plainText();
			}
		}
		return out;
	}

	QList<QSharedPointer<Tag>> Tag::childTags() const
	{
		QList<QSharedPointer<Tag>> out;
		out.reserve(children_.size());
		for (const Child& c : children_)
		{
			if (!c.tag.isNull()) out.append(c.tag);
		}
		return out;
	}

	void Tag::read(QXmlStreamReader& reader)
	{
		tag_ = reader.qualifiedName().toString();
		loopToReadAttributes(reader);
		loopToReadNamespaces(reader);
		loopToReadSubtag(reader);
	}

	void Tag::write(QXmlStreamWriter* writer)
	{
		writeStart(writer);
		writeNamespaces(writer);
		writeAttributes(writer);
		// children_ carries the original text/element interleave;
		// writeSubtags emits both in source order.
		writeSubtags(writer);
		writeEnd(writer);
	}

	void Tag::writeStart(QXmlStreamWriter* writer)
	{
		writer->writeStartElement(instanceTag());
	}

	void Tag::writeNamespaces(QXmlStreamWriter* writer)
	{
		QMapIterator<QString, QString> i(namespaces_);
		while (i.hasNext()) {
			i.next();
			writer->writeNamespace(i.key(), i.value());
		}
	}

	void Tag::writeAttributes(QXmlStreamWriter* writer)
	{
		QMapIterator<QString, QString> i(attributes_);
		while (i.hasNext()) {
			i.next();
			writer->writeAttribute(i.key(), i.value());
		}
	}

	void Tag::writeSubtags(QXmlStreamWriter* writer)
	{
		for (const Child& c : children_) {
			if (c.tag.isNull())
			{
				if (!c.text.isEmpty()) writer->writeCharacters(c.text);
			}
			else
			{
				c.tag->write(writer);
			}
		}
	}

	void Tag::writeEnd(QXmlStreamWriter* writer)
	{
		writer->writeEndElement();
	}

	void Tag::loopToReadNamespaces(QXmlStreamReader& reader)
	{
		for (auto& it : reader.namespaceDeclarations()) {
			readNamespace(it.namespaceUri(), it.prefix());
		}
	}

	void Tag::loopToReadAttributes(QXmlStreamReader& reader)
	{
		for (auto& it : reader.attributes()) {
			readAttribute(it.qualifiedName(), it.value());
		}
	}

	void Tag::readNamespace(const QStringView name, const QStringView value)
	{
		namespaces_.insert(name.toString(), value.toString());
	}

	void Tag::readAttribute(const QStringView name, const QStringView value)
	{
		attributes_.insert(name.toString(), value.toString());
	}

	void Tag::loopToReadSubtag(QXmlStreamReader& reader)
	{
		// Materialise the loop sentinel into a QString — a QStringView would
		// be a borrowed reference into the reader's internal buffer and gets
		// invalidated by readNext() on the very next iteration.
		const QString currentTokenName = reader.qualifiedName().toString();
		while (!((reader.tokenType() == QXmlStreamReader::EndElement && (reader.qualifiedName() == currentTokenName)))) {
			reader.readNext();
			if (reader.tokenType() == QXmlStreamReader::StartElement) {
				readSubtag(reader);
			}
			else if (reader.tokenType() == QXmlStreamReader::Characters && !reader.isWhitespace()) {
				// Preserve text fragments in source order alongside child
				// elements so mixed content round-trips lossless. Whitespace-
				// only fragments are skipped to keep pretty-printed input
				// from accumulating indentation noise.
				children_.append(Child{ reader.text().toString(), {} });
			}
		}
	}

	void Tag::readSubtag(QXmlStreamReader& reader)
	{
		children_.append(Child{ {}, QSharedPointer<Tag>(new Tag(reader)) });
	}

	bool Tag::isStartElementNamed(QXmlStreamReader& xml, const QString& token_name)
	{
		return xml.tokenType() == QXmlStreamReader::StartElement && xml.qualifiedName() == token_name;
	}
}
