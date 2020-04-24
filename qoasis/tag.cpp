// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

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

	Tag::Tag(const Tag& obj)
	{
		// deep copy on attributes
		QMapIterator<QString, QString> i(obj.attributes_);
		while (i.hasNext())
		{
			i.next();
			attributes_.insert(i.key(), i.value());
		}
		// deep copy on subitems
		for (int i = 0; i < subtags_.length(); i++)
		{
			QSharedPointer<Tag> sub = obj.subtags_.at(i);
			if (!sub.isNull())
			{
				subtags_.replace(i, QSharedPointer<Tag>(new Tag(*sub)));
			}
		}
	}

	// Methods
	QLatin1String Tag::instanceTag()
	{
		return QLatin1String("");
	}

	void Tag::read(QXmlStreamReader& reader)
	{
		loopToReadAttributes(reader);
		loopToReadSubtag(reader);
	}

	void Tag::write(QXmlStreamWriter* writer)
	{
		writeStart(writer);
		writeAttributes(writer);
		writeSubtags(writer);
		writeEnd(writer);
	}

	void Tag::writeStart(QXmlStreamWriter* writer)
	{
		writer->writeStartElement(instanceTag());
	}

	void Tag::writeAttributes(QXmlStreamWriter* writer)
	{
		QMapIterator<QString, QString> i(attributes_);
		while (i.hasNext())
		{
			i.next();
			writer->writeAttribute(i.key(), i.value());
		}
	}

	void Tag::writeSubtags(QXmlStreamWriter* writer)
	{
		for (int i = 0; i < subtags_.length(); i++)
		{
			if (!subtags_[i].isNull())
			{
				subtags_[i]->write(writer);
			}
		}
	}

	void Tag::writeEnd(QXmlStreamWriter* writer)
	{
		writer->writeEndElement();
	}

	void Tag::loopToReadAttributes(QXmlStreamReader& reader)
	{
		for (auto& it : reader.attributes())
		{
			readAttribute(it.qualifiedName(), it.value());
		}
	}

	void Tag::readAttribute(const QStringRef name, const QStringRef value)
	{
		attributes_.insert(name.toString(), value.toString());
	}

	void Tag::loopToReadSubtag(QXmlStreamReader& reader)
	{
		do
		{
			reader.readNext();
			readSubtag(reader);
		}
		while (isNotEndElementNamed(reader, instanceTag()));
	}

	void Tag::readSubtag(QXmlStreamReader& reader)
	{
		subtags_.append(QSharedPointer<Tag>(new Tag(reader)));
	}

	bool Tag::isStartElementNamed(QXmlStreamReader& xml, const QString& token_name)
	{
		return xml.tokenType() == QXmlStreamReader::StartElement && xml.qualifiedName() == token_name;
	}

	bool Tag::isNotEndElementNamed(QXmlStreamReader& xml, const QString& token_name)
	{
		return !(xml.tokenType() == QXmlStreamReader::EndElement && xml.qualifiedName() == token_name);
	}
}
