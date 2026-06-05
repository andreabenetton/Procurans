// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecelldate.h"

namespace qoasis::table
{
	const QString TablecellDate::kCellTypeValue = QString("date");
	const QString TablecellDate::kCellTypeAttribute = QString("office:date-value");

	TablecellDate::TablecellDate(QDate date, int repeat, QString style) : Tablecell(repeat, style)
	{
		value_date_ = date;
	}

	TablecellDate::TablecellDate(QDate date, QString style) : TablecellDate(date, 1, style) {}

	TablecellDate::TablecellDate(QXmlStreamReader& reader) 
	{
		Q_ASSERT(reader.qualifiedName() == TablecellDate::kTag);
		Tag::read(reader);
	}

	QDate TablecellDate::getDate() const
	{
		return value_date_;
	}

	// implements IRepeatable
	bool TablecellDate::isEmpty()
	{
		return false;
	}

	// implements Tablecell
	QString TablecellDate::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellDate::readAttribute(QStringView name, QStringView value)
	{
		if (name.toString() == kCellTypeAttribute) {
			// ODF allows either a date or a dateTime here. Capture the raw
			// string so any time/TZ component round-trips verbatim, and
			// also parse a QDate from the leading date for the typed
			// accessor's benefit.
			value_raw_ = value.toString();
			value_date_ = QDate::fromString(value_raw_.left(10), "yyyy-MM-dd");
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellDate::writeAttributes(QXmlStreamWriter* writer)
	{
		// Loaded-from-XML cells write back the raw input (preserves any
		// time/TZ component). Programmatic cells synthesise from QDate.
		if (!value_raw_.isEmpty()) {
			writer->writeAttribute(kCellTypeAttribute, value_raw_);
		}
		else if (!value_date_.isNull() && value_date_.isValid()) {
			writer->writeAttribute(kCellTypeAttribute,
			                       value_date_.toString("yyyy-MM-dd"));
		}
		Tablecell::writeAttributes(writer);
	}

	void TablecellDate::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!value_date_.isNull() && value_date_.isValid()) {
			writer->writeTextElement(Tablecell::kTextPTag, value_date_.toString("dd/MM/yyyy"));
		}
	}
}
