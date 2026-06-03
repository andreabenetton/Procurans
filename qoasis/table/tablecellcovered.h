// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef TABLECELLCOVERED_H
#define TABLECELLCOVERED_H

#include "tablecell.h"

namespace qoasis::table
{
	/**
	 * \brief The <table:covered-table-cell> element represents the cell positions
	 *  covered by a preceding cell that spans them via number-columns-spanned /
	 *  number-rows-spanned. It carries no value of its own but participates in
	 *  the row's column-position math.
	 *  https://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_covered-table-cell
	 */
	class TablecellCovered : public Tablecell
	{
	public:
		TablecellCovered();
		TablecellCovered(QXmlStreamReader& reader);

		static QSharedPointer<Tablecell> builder(QXmlStreamReader& reader);

		static const QString kTag;

		// implements Tag
		QString instanceTag() override;
	};
}
#endif // TABLECELLCOVERED_H
