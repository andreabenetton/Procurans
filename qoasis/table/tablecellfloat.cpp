// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellfloat.h"

namespace qoasis::table
{
	const QString TablecellFloat::kCellTypeValue = QString("float");
	const QString TablecellFloat::kCellTypeAttribute = QString("office:value");

	TablecellFloat::TablecellFloat(double number, int repeat, QString style) : Tablecell(repeat, style)
	{
		value_number_ = number;
	}

	TablecellFloat::TablecellFloat(double number, QString style) : TablecellFloat(number, 1, style) {}

	TablecellFloat::TablecellFloat(QXmlStreamReader& reader) 
	{
		Q_ASSERT(reader.qualifiedName() == TablecellFloat::kTag);
		value_number_ = 0.0;
		Tag::read(reader);
	}


	double TablecellFloat::getDouble() const
	{
		return value_number_;
	}

	// implements IRepeatable
	bool TablecellFloat::isEmpty()
	{
		return false;
	}

	// implements Tablecell
	QString TablecellFloat::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellFloat::readAttribute(QStringRef name, QStringRef value)
	{
		if (name.toString() == kCellTypeAttribute) {
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
