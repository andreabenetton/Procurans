// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "body.h"

namespace qoasis::office
{
	// Constants
	const QString Body::kTag = QString("office:body");

	// Constructors
	Body::Body()
	{
		spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet());
	}

	Body::Body(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Body::kTag);
		read(reader);
	}

	// Static methods
	QSharedPointer<Tag> Body::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Body::kTag);
		return QSharedPointer<Tag>(new Body(reader));
	}

	// Methods  
	QSharedPointer<Spreadsheet> Body::getSpreadsheet()
	{
		return spreadsheet_;
	}

	// implements Tag
	QString Body::instanceTag()
	{
		return Body::kTag;
	}

	void Body::readSubtag(QXmlStreamReader& reader)
	{
		// <office:spreadsheet> 3.7 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_spreadsheet
		if (isStartElementNamed(reader, Spreadsheet::kTag))
		{
			spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet(reader));
			return;
		}
		// Deserialize present but unsupported subtags
		// <office:chart> 3.8 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_chart
		// <office:database> 12.1 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_database
		// <office:drawing> 3.5 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_drawing
		// <office:image> 3.9 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_image
		// <office:presentation> 3.6 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_presentation
		// <office:text> 3.4 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_text
		Tag::readSubtag(reader);
	}

	void Body::writeSubtags(QXmlStreamWriter* writer)
	{
		if (spreadsheet_ != nullptr)
		{
			spreadsheet_->write(writer);
		}
		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
	}
}
