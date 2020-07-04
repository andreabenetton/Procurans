// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLSTRING_H
#define TABLECELLSTRING_H

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellString : public Tablecell
	{
	public:
		TablecellString(QString text, int repeat, QString style = "");
		TablecellString(QString text, QString style = "");
		TablecellString(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		// implements Tablecell
		QString instanceCellType() override;
	};
}
#endif // TABLECELLSTRING_H
