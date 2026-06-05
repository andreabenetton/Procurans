// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLPERCENTAGE_H
#define TABLECELLPERCENTAGE_H

#include "tablecell.h"

namespace qoasis::table
{
	// Percentage cells share office:value with float, but office:value-type
	// is "percentage". The stored double is a unit-fraction: 0.5 means 50%.
	class TablecellPercentage : public Tablecell
	{
	public:
		TablecellPercentage(double fraction, int repeat, QString style = "");
		TablecellPercentage(double fraction, QString style = "");
		TablecellPercentage(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		double getPercentage() const;

		bool isEmpty() override;

		// implements Tablecell
		QString instanceCellType() override;

	private:
		static const QString kCellTypeAttribute;

		double value_fraction_;

		// implements Tag
		void readAttribute(QStringView name, QStringView value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLPERCENTAGE_H
