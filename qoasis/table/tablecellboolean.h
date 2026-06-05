// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLBOOLEAN_H
#define TABLECELLBOOLEAN_H

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellBoolean : public Tablecell
	{
	public:
		TablecellBoolean(bool value, int repeat, QString style = "");
		TablecellBoolean(bool value, QString style = "");
		TablecellBoolean(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		bool getBool() const;

		bool isEmpty() override;

		// implements Tablecell
		QString instanceCellType() override;

	private:
		static const QString kCellTypeAttribute;

		bool value_bool_;

		// implements Tag
		void readAttribute(QStringView name, QStringView value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLBOOLEAN_H
