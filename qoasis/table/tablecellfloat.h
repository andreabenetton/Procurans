// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLFLOAT_H
#define TABLECELLFLOAT_H

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellFloat : public Tablecell
	{
	public:
		TablecellFloat(double number, int repeat, QString style = "");
		TablecellFloat(double number, QString style = "");
		TablecellFloat(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		bool isEmpty() override;

		double getDouble() const;

		// implements Tablecell
		QString instanceCellType() override;

	private:
		static const QString kCellTypeAttribute;

		double value_number_;

		// implements Tag
		void readAttribute(QStringRef name, QStringRef value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLFLOAT_H
