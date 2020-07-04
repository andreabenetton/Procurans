// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "documentcontent.h"

namespace qoasis::office
{
	// Constants
	const QString DocumentContent::kTag = QString("office:document-content");

	const QString DocumentContent::kNsOfficeAttribute = QString("xmlns:office");
	const QString DocumentContent::kNsDcAttribute = QString("xmlns:dc");
	const QString DocumentContent::kNsDrawAttribute = QString("xmlns:draw");
	const QString DocumentContent::kNsFoAttribute = QString("xmlns:fo");
	const QString DocumentContent::kNsNumberAttribute = QString("xmlns:number");
	const QString DocumentContent::kNsOfAttribute = QString("xmlns:of");
	const QString DocumentContent::kNsStyleAttribute = QString("xmlns:style");
	const QString DocumentContent::kNsSvgAttribute = QString("xmlns:svg");
	const QString DocumentContent::kNsTableAttribute = QString("xmlns:table");
	const QString DocumentContent::kNsTextAttribute = QString("xmlns:text");
	const QString DocumentContent::kNsXlinkAttribute = QString("xmlns:xlink");

	const QString DocumentContent::kVersionAttribute = QString("office:version");

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
	QString DocumentContent::instanceTag()
	{
		return DocumentContent::kTag;
	}

	void DocumentContent::readAttribute(QStringRef name, QStringRef value)
	{
		// office:version 19.386 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-office_version
		if (name.toString() == kVersionAttribute)
		{
			version_ = value.toString();
			return;
		}
		// Deserialize present but unsupported attributes
		// grddl:transformation 19.320 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-grddl_transformation
		Tag::readAttribute(name, value);
	}

	void DocumentContent::readSubtag(QXmlStreamReader& reader)
	{
		// <office:body> 3.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_body
		if (isStartElementNamed(reader, Body::kTag))
		{
			body_ = QSharedPointer<Body>(new Body(reader));
			return;
		}
		// Deserialize present but unsupported subtags
		// <office:automatic-styles> 3.15.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_automatic-styles
		// <office:font-face-decls> 3.14 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_font-face-decls
		// <office:scripts> 3.12 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_scripts
		Tag::readSubtag(reader);
	}

	void DocumentContent::writeAttributes(QXmlStreamWriter* writer)
	{
		writer->writeAttribute(kVersionAttribute, version_);

		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void DocumentContent::writeSubtags(QXmlStreamWriter* writer)
	{
		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
		if (body_ != nullptr)
		{
			body_->write(writer);
		}
	}
}
