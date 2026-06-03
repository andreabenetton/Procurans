// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellstring.h"

namespace qoasis::table
{
	const QString TablecellString::kCellTypeValue = QString("string");
	const QString TablecellString::kStringValueAttribute = QString("office:string-value");

	TablecellString::TablecellString(QString text, int repeat, QString style) : Tablecell(repeat, style)
	{
		_valueText = text;
	}

	TablecellString::TablecellString(QString text, QString style) : TablecellString(text, 1, style){}

	TablecellString::TablecellString(QXmlStreamReader& reader) 
	{
		Q_ASSERT(reader.qualifiedName() == TablecellString::kTag);
		_valueText = "";
		Tag::read(reader);
	}

	// implements Tablecell
	QString TablecellString::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellString::readAttribute(QStringRef name, QStringRef value)
	{
		if (name.toString() == kStringValueAttribute) {
			_valueText = value.toString();
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellString::writeAttributes(QXmlStreamWriter* writer)
	{
		Tablecell::writeAttributes(writer);
		if (!_valueText.isEmpty()) {
			writer->writeAttribute(kStringValueAttribute, _valueText);
		}
	}
}
