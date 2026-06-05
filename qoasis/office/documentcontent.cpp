// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "documentcontent.h"

namespace qoasis::office
{
	// Constants
	const QString DocumentContent::kTag = QString("office:document-content");

	const QString DocumentContent::kVersionAttribute = QString("office:version");

	namespace
	{
		// Baseline ODF 1.2 namespaces seeded on a from-scratch DocumentContent.
		// Tag::namespaces_ is URI->prefix, so the readNamespace call below
		// uses that order. The previous code declared an 'xmlns:office'-keyed
		// map on a duplicate field that no write path actually read, so a
		// freshly-created document emitted no namespace declarations at all.
		struct NsSeed { const char* uri; const char* prefix; };
		const NsSeed kSeeds[] = {
			{"urn:oasis:names:tc:opendocument:xmlns:office:1.0",          "office"},
			{"urn:oasis:names:tc:opendocument:xmlns:table:1.0",           "table"},
			{"urn:oasis:names:tc:opendocument:xmlns:text:1.0",            "text"},
			{"urn:oasis:names:tc:opendocument:xmlns:style:1.0",           "style"},
			{"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0", "fo"},
			{"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0",  "svg"},
			{"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0",       "number"},
			{"urn:oasis:names:tc:opendocument:xmlns:of:1.2",              "of"},
			{"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0",         "draw"},
			{"http://purl.org/dc/elements/1.1/",                          "dc"},
			{"http://www.w3.org/1999/xlink",                              "xlink"},
		};
	}

	// Constructors
	DocumentContent::DocumentContent() : Tag()
	{
		for (const NsSeed& s : kSeeds)
		{
			readNamespace(QString::fromUtf8(s.uri),
			              QString::fromUtf8(s.prefix));
		}

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

	void DocumentContent::readAttribute(QStringView name, QStringView value)
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
