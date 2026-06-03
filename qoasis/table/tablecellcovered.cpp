// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellcovered.h"

namespace qoasis::table
{
	// Constants
	const QString TablecellCovered::kTag = QString("table:covered-table-cell");

	// Constructors
	TablecellCovered::TablecellCovered() : Tablecell()
	{
	}

	TablecellCovered::TablecellCovered(QXmlStreamReader& reader) : Tablecell()
	{
		Q_ASSERT(reader.qualifiedName() == TablecellCovered::kTag);
		Tag::read(reader);
	}

	// Static methods
	QSharedPointer<Tablecell> TablecellCovered::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == TablecellCovered::kTag);
		return QSharedPointer<Tablecell>(new TablecellCovered(reader));
	}

	// implements Tag
	QString TablecellCovered::instanceTag()
	{
		return kTag;
	}
}
