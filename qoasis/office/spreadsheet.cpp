// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "spreadsheet.h"
#include "../table/table.h"

namespace qoasis::office
{
	// Constants
	const QString Spreadsheet::kTag = QString("office:spreadsheet");

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
	QString Spreadsheet::instanceTag()
	{
		return Spreadsheet::kTag;
	}

	void Spreadsheet::readAttribute(QStringRef name, QStringRef value)
	{
		// Deserialize present but unsupported attributes
		// table:protection-key 19.697 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_protection-key
		// table:protection-key-digest-algorithm 19.698 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_protection-key-digest-algorithm
		// table:structure-protected 19.725 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_structure-protected
		Tag::readAttribute(name, value);
	}

	void Spreadsheet::readSubtag(QXmlStreamReader& reader)
	{
		// <table:table> 9.1.2 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table
		if (isStartElementNamed(reader, Table::kTag))
		{
			tables_.append(QSharedPointer<Table>(new Table(reader)));
			return;
		}
		// Deserialize present but unsupported subtags
		// <table:calculation-settings> 9.4.1 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_calculation-settings
		// <table:consolidation> 9.7 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_consolidation
		// <table:content-validations> 9.4.4 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_content-validations
		// <table:database-ranges> 9.4.14 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_database-ranges
		// <table:data-pilot-tables> 9.6.2 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_data-pilot-tables
		// <table:dde-links> 9.8http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_dde-links
		// <table:label-ranges> 9.4.10 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_label-ranges
		// <table:named-expressions> 9.4.11 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_named-expressions
		// <table:tracked-changes> 9.9.2 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_tracked-changes
		// <text:alphabetical-index-auto-mark-file> 8.8.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_alphabetical-index-auto-mark-file
		// <text:dde-connection-decls> 14.6.2 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_dde-connection-decls
		// <text:sequence-decls> 7.4.11 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_sequence-decls
		// <text:user-field-decls> 7.4.7 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_user-field-decls
		// <text:variable-decls> 7.4.2 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_variable-decls
		Tag::readSubtag(reader);
	}

	void Spreadsheet::writeAttributes(QXmlStreamWriter* writer)
	{
		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
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
		//Tag::writeSubtags(writer);
	}
}
