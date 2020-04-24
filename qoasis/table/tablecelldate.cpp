// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecelldate.h"

namespace qoasis::table
{
	const QLatin1String TablecellDate::kCellTypeValue = QLatin1String("date");
	const QLatin1String TablecellDate::kCellTypeAttribute = QLatin1String("office:date-value");

	TablecellDate::TablecellDate(QDate date, int repeat) : Tablecell(repeat)
	{
		value_date_ = date;
	}

	TablecellDate::TablecellDate(QXmlStreamReader& reader) : Tablecell(reader)
	{
		read(reader);
	}

	TablecellDate::TablecellDate(const TablecellDate& obj): Tablecell(obj)
	{
		value_date_ = obj.value_date_;
	}

	QDate TablecellDate::getDate() const
	{
		return value_date_;
	}

	// implements Tablecell
	QLatin1String TablecellDate::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellDate::readAttribute(QStringRef name, QStringRef value)
	{
		if (name == kCellTypeAttribute)
		{
			value_date_ = QDate::fromString(value.toString(), "yyyy-MM-dd");
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellDate::writeAttributes(QXmlStreamWriter* writer)
	{
		if (!value_date_.isNull() && value_date_.isValid())
		{
			writer->writeAttribute(kCellTypeAttribute, value_date_.toString("yyyy-MM-dd"));
		}
		Tag::writeAttributes(writer);
	}

	void TablecellDate::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!value_date_.isNull() && value_date_.isValid())
		{
			writer->writeTextElement(Tablecell::kTextPTag, value_date_.toString("dd/MM/yyyy"));
		}
	}
}
