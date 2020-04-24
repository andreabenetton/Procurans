// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "body.h"

namespace qoasis::office
{
	// Constants
	const QLatin1String Body::kTag = QLatin1String("office:body");

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

	Body::Body(const Body& obj)
	{
		spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet(*obj.spreadsheet_));
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
	QLatin1String Body::instanceTag()
	{
		return Body::kTag;
	}

	void Body::readSubtag(QXmlStreamReader& reader)
	{
		if (isStartElementNamed(reader, Spreadsheet::kTag))
		{
			spreadsheet_ = QSharedPointer<Spreadsheet>(new Spreadsheet(reader));
			return;
		}
		// Deserialize present but unsupported subtags
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
