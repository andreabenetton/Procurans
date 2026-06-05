// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLTIME_H
#define TABLECELLTIME_H

#include "tablecell.h"

namespace qoasis::table
{
	class TablecellTime : public Tablecell
	{
	public:
		TablecellTime(QString duration, int repeat, QString style = "");
		TablecellTime(QString duration, QString style = "");
		TablecellTime(QXmlStreamReader& reader);

		static const QString kCellTypeValue;

		// Returns the raw ISO 8601 duration string (e.g. "PT12H30M0S").
		// Kept as QString rather than QTime because ODF allows durations
		// greater than 24 hours, which QTime can't represent.
		QString getDuration() const;

		bool isEmpty() override;

		// implements Tablecell
		QString instanceCellType() override;

	private:
		static const QString kCellTypeAttribute;

		QString value_duration_;

		// implements Tag
		void readAttribute(QStringView name, QStringView value) override;
		void writeAttributes(QXmlStreamWriter* writer) override;
		void writeSubtags(QXmlStreamWriter* writer) override;
	};
}
#endif // TABLECELLTIME_H
