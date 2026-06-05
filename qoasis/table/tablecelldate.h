// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLDATE_H
#define TABLECELLDATE_H

#include <QDateTime>

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellDate : public Tablecell
	{
	public:
		TablecellDate(QDate date, int repeat, QString style = "");
		TablecellDate(QDate date, QString style = "");
		TablecellDate(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		QDate getDate() const;

		bool isEmpty() override;

		// implements Tablecell
		QString instanceCellType() override;

	private:
		static const QString kCellTypeAttribute;

		QDate value_date_;
		// Raw input string for office:date-value. ODF allows xsd:dateTime
		// (yyyy-MM-ddThh:mm:ss[Z|±hh:mm]) in addition to plain xsd:date —
		// preserving the original avoids lossy QDate round-trip when the
		// host application carried a timestamp portion. Empty for cells
		// constructed via the typed QDate ctors.
		QString value_raw_;

		// implements Tags
		void readAttribute(QStringView name, QStringView value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLDATE_H
