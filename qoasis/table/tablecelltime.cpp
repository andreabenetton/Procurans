// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecelltime.h"

namespace qoasis::table
{
	const QString TablecellTime::kCellTypeValue = QString("time");
	const QString TablecellTime::kCellTypeAttribute =
		QString("office:time-value");

	TablecellTime::TablecellTime(QString duration, int repeat, QString style)
		: Tablecell(repeat, style), value_duration_(std::move(duration))
	{
	}

	TablecellTime::TablecellTime(QString duration, QString style)
		: TablecellTime(std::move(duration), 1, style)
	{
	}

	TablecellTime::TablecellTime(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == TablecellTime::kTag);
		Tag::read(reader);
	}

	QString TablecellTime::getDuration() const
	{
		return value_duration_;
	}

	bool TablecellTime::isEmpty()
	{
		return value_duration_.isEmpty();
	}

	// implements Tablecell
	QString TablecellTime::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellTime::readAttribute(QStringView name, QStringView value)
	{
		if (name == kCellTypeAttribute) {
			// ODF Part 1 §19.397: ISO 8601 duration. Preserve raw so PnYnMnD
			// + the T component (>24h is legal) round-trip without lossy
			// QTime conversion.
			value_duration_ = value.toString();
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellTime::writeAttributes(QXmlStreamWriter* writer)
	{
		if (!value_duration_.isEmpty()) {
			writer->writeAttribute(kCellTypeAttribute, value_duration_);
		}
		Tablecell::writeAttributes(writer);
	}

	void TablecellTime::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!_paragraphs.isEmpty() || !_valueText.isEmpty())
		{
			Tablecell::writeSubtags(writer);
			return;
		}
		if (!value_duration_.isEmpty()) {
			writer->writeTextElement(Tablecell::kTextPTag, value_duration_);
		}
	}
}
