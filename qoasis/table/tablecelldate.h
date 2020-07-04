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

		// implements Tags
		void readAttribute(QStringRef name, QStringRef value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLDATE_H
