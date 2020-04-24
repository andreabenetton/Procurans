// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLEMPTY_H
#define TABLECELLEMPTY_H

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellEmpty : public Tablecell
	{
	public:
		TablecellEmpty(int repeat = 1, QString style = "");
		TablecellEmpty(QXmlStreamReader& reader);
		TablecellEmpty(const TablecellEmpty& obj);

		static const QLatin1String kCellTypeValue;

		// implements Tablecell
		QLatin1String instanceCellType() override;

	protected:
		// implements Tags
		void readSubtag(QXmlStreamReader& reader) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
		void readAttribute(QStringRef name, QStringRef value) override;
	};
}
#endif // TABLECELLEMPTY_H
