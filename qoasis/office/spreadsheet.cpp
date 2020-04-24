// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "spreadsheet.h"
#include "../table/table.h"

namespace qoasis::office
{
	// Constants
	const QLatin1String Spreadsheet::kTag = QLatin1String("office:spreadsheet");

	// Constructors
	Spreadsheet::Spreadsheet() : Tag()
	{
		tables_.append(QSharedPointer<Table>(new Table));
	}

	Spreadsheet::Spreadsheet(QXmlStreamReader& reader): Tag()
	{
		Q_ASSERT(reader.qualifiedName() == Spreadsheet::kTag);
		Tag::read(reader);
	}

	Spreadsheet::Spreadsheet(const Spreadsheet& obj)
	{
		// deep copy on tables
		for (int i = 0; i < tables_.length(); i++)
		{
			QSharedPointer<Table> table = obj.tables_.at(i);
			if (!table.isNull())
			{
				tables_.replace(i, QSharedPointer<Table>(new Table(*table)));
			}
		}
	}

	// Static methods
	QSharedPointer<Tag> Spreadsheet::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Spreadsheet::kTag);
		return QSharedPointer<Tag>(new Spreadsheet(reader));
	}

	// Methods
	QSharedPointer<Table> Spreadsheet::getTable(QString& name)
	{
		for (auto& table : tables_)
		{
			if (!table.isNull())
			{
				if (name == (table->getName()))
				{
					return table;
				}
			}
		}
		return QSharedPointer<Table>(nullptr);
	}

	QSharedPointer<Table> Spreadsheet::getTable(int index)
	{
		Q_ASSERT(index >= 0);
		if ((index >= tables_.length()))
		{
			return QSharedPointer<Table>(nullptr);
		}
		return tables_[index];
	}

	// implements Tag
	QLatin1String Spreadsheet::instanceTag()
	{
		return Spreadsheet::kTag;
	}

	void Spreadsheet::readSubtag(QXmlStreamReader& reader)
	{
		if (isStartElementNamed(reader, Table::kTag))
		{
			tables_.append(QSharedPointer<Table>(new Table(reader)));
			return;
		}
		// Deserialize present but unsupported subtags
		Tag::readSubtag(reader);
	}

	void Spreadsheet::writeSubtags(QXmlStreamWriter* writer)
	{
		for (auto& table : tables_)
		{
			if (!table.isNull())
			{
				table->write(writer);
			}
		}
		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
	}
}
