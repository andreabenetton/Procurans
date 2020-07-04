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
		return subtags_.isEmpty();
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
		for (int i = 0; i < subtags_.length(); i++) {
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

	void Tag::readNamespace(const QStringRef name, const QStringRef value)
	{
		namespaces_.insert(name.toString(), value.toString());
		qDebug() << "Namespace read in: " << instanceTag() << " - name:" << name.toString() << " value:" << value.toString();
	}

	void Tag::readAttribute(const QStringRef name, const QStringRef value)
	{
		attributes_.insert(name.toString(), value.toString());
		qDebug() << "Attribute read in: " << instanceTag() << " - name:" << name.toString() << " value:" << value.toString();
	}

	void Tag::loopToReadSubtag(QXmlStreamReader& reader)
	{
		QStringRef currentTokenName = reader.qualifiedName();
		while (!((reader.tokenType() == QXmlStreamReader::EndElement && (reader.qualifiedName() == currentTokenName)))) {
			reader.readNext();
			qDebug() << "Subtag read in: " << instanceTag() << " - XML Token read: " << TokenTypeToString(reader.tokenType()) << " - " << reader.name();
			if (reader.tokenType() == QXmlStreamReader::StartElement) {
				readSubtag(reader);
			}
		}
	}

	QString Tag::TokenTypeToString(int type)
	{
		switch (type) {
		case QXmlStreamReader::NoToken:
			return QString("NoToken");
		case QXmlStreamReader::Invalid:
			return QString("Invalid");
		case QXmlStreamReader::StartDocument:
			return QString("StartDocument");
		case QXmlStreamReader::EndDocument:
			return QString("EndDocument");
		case QXmlStreamReader::StartElement:
			return QString("StartElement");
		case QXmlStreamReader::EndElement:
			return QString("EndElement");
		case QXmlStreamReader::Characters:
			return QString("Characters");
		case QXmlStreamReader::Comment:
			return QString("Comment");
		case QXmlStreamReader::DTD:
			return QString("DTD");
		case QXmlStreamReader::EntityReference:
			return QString("EntityReference");
		case QXmlStreamReader::ProcessingInstruction:
			return QString("ProcessingInstruction");
		}
		return QString("Undecoded Token type Error");
	}

	void Tag::readSubtag(QXmlStreamReader& reader)
	{
		subtags_.append(QSharedPointer<Tag>(new Tag(reader)));
	}

	bool Tag::isStartElementNamed(QXmlStreamReader& xml, const QString& token_name)
	{
		return xml.tokenType() == QXmlStreamReader::StartElement && xml.qualifiedName() == token_name;
	}
}
