// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellstring.h"

namespace qoasis::table
{
	const QString TablecellString::kCellTypeValue = QString("string");

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
}
