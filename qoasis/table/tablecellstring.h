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
		static const QString kStringValueAttribute;

		// implements Tablecell
		QString instanceCellType() override;

	protected:
		// implements Tag
		void readAttribute(QStringView name, QStringView value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLSTRING_H
