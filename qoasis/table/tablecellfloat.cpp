// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellfloat.h"

namespace qoasis::table
{
	const QLatin1String TablecellFloat::kCellTypeValue = QLatin1String("float");
	const QLatin1String TablecellFloat::kCellTypeAttribute = QLatin1String("office:value");

	TablecellFloat::TablecellFloat(double number, int repeat) : Tablecell(repeat)
	{
		value_number_ = number;
	}

	TablecellFloat::TablecellFloat(QXmlStreamReader& reader) : Tablecell(reader)
	{
		value_number_ = 0.0;

		read(reader);
	}

	TablecellFloat::TablecellFloat(const TablecellFloat& obj): Tablecell(obj)
	{
		value_number_ = obj.value_number_;
	}

	double TablecellFloat::getDouble() const
	{
		return value_number_;
	}

	// implements Tablecell
	QLatin1String TablecellFloat::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellFloat::readAttribute(QStringRef name, QStringRef value)
	{
		if (name == kCellTypeAttribute)
		{
			value_number_ = value.toFloat();
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellFloat::writeAttributes(QXmlStreamWriter* writer)
	{
		Tablecell::writeAttributes(writer);
		writer->writeAttribute(kCellTypeAttribute, QString::number(value_number_, 'f', 2));
	}

	void TablecellFloat::writeSubtags(QXmlStreamWriter* writer)
	{
		writer->writeTextElement(Tablecell::kTextPTag, QString::number(value_number_, 'f', 2).replace(".", ","));
	}
}
