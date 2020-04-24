// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellempty.h"

namespace qoasis::table
{
	// Constants
	const QLatin1String TablecellEmpty::kCellTypeValue = QLatin1String("");

	// Constructors
	TablecellEmpty::TablecellEmpty(int repeat, QString style) : Tablecell(repeat, style)
	{
	}

	TablecellEmpty::TablecellEmpty(QXmlStreamReader& reader) : Tablecell(reader)
	{
	}

	TablecellEmpty::TablecellEmpty(const TablecellEmpty& obj): Tablecell(obj)
	{
	}

	// Methods
	QLatin1String TablecellEmpty::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellEmpty::readAttribute(QStringRef name, QStringRef value)
	{
		Tablecell::readAttribute(name, value);
	}

	void TablecellEmpty::readSubtag(QXmlStreamReader& reader)
	{
	}

	void TablecellEmpty::writeAttributes(QXmlStreamWriter* writer)
	{
	}

	void TablecellEmpty::writeSubtags(QXmlStreamWriter* writer)
	{
	}
}
