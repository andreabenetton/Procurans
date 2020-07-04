// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "table.h"

namespace qoasis::table
{
	const QString Table::kTag = QString("table:table");
	const QString Table::kNameAttribute = QString("table:name");
	const QString Table::kPrintRangeAttribute = QString("table:print-ranges");

	// Constructors
	Table::Table(QString name, QString style) : Tag(), IStyleable(style), INameable(name)
	{
	}

	Table::Table(QXmlStreamReader& reader) : Tag(), IStyleable(), INameable()
	{
		QString tt = reader.qualifiedName().toString();
		Q_ASSERT(reader.qualifiedName() == Table::kTag);
		Table::read(reader);
	}


	// Static methods
	QSharedPointer<Table> Table::Builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Table::kTag);
		return QSharedPointer<Table>(new Table(reader));
	}

	QSharedPointer<Tablecolumn> Table::getColumn(int index) const
	{
		Q_ASSERT(index >= 0);
		return columns_.at(index);
	}

	QSharedPointer<Tablerow> Table::getRow(int index) const
	{
		Q_ASSERT(index >= 0);
		return rows_.at(index);
	}

	int Table::getLastNonEmptyRow() const
	{
		return rows_.lastNotEmpty();
	}

	void Table::removeRow(int index)
	{
		Q_ASSERT(index >= 0);
		rows_.remove(index);
	}

	void Table::removeEndingEmptyRows()
	{
		rows_.removeEndingEmpty();
	}

	void Table::insertRow(int index, QSharedPointer<Tablerow> row)
	{
		rows_.insert(index, row);
	}

	void Table::appendRow(QSharedPointer<Tablerow> row)
	{
		rows_.append(row);
	}

	// implements INameable
	QString Table::nameTag()
	{
		return kNameAttribute;
	}

	// implements Tag
	QString Table::instanceTag()
	{
		return kTag;
	}

	void Table::readAttribute(QStringRef name, QStringRef value)
	{
		// table:name 19.673.13 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_name_element-table_table
		if (name.toString() == nameTag()) {
			readName(value);
			return;
		}
		// table:style-name 19.726.16 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_style-name_element-table_table 
		if (name.toString() == styleTag()) {
			readStyle(value);
			return;
		}
		// table:print-ranges 19.694 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_print-ranges
		if (name.toString() == kPrintRangeAttribute) {
			print_ranges_ = value.toString();
			return;
		}
		// Deserialize present but unsupported attributes
		// table:is-sub-table 19.655 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_is-sub-table	
		// table:print 19.693 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_print
		// table:protected 19.696.4 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_protected_element-table_table
		// table:protection-key 19.697 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_protection-key
		// table:protection-key-digest-algorithm 19.698 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_protection-key-digest-algorithm
		// table:template-name 19.732 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_template-name
		// table:use-banding-columns-styles 19.736 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_use-banding-columns-styles
		// table:use-banding-rows-styles 19.737 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_use-banding-rows-styles
		// table:use-first-column-styles 19.738 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_use-first-column-styles
		// table:use-first-row-styles 19.739 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_use-first-row-styles
		// table:use-last-column-styles 19.740 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_use-last-column-styles
		// table:use-last-row-styles 19.741 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_use-last-row-styles 
		// xml:id 19.914 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-xml_id
		Tag::readAttribute(name, value);
	}

	void Table::readSubtag(QXmlStreamReader& reader)
	{
		// <table:table-column> 9.1.6 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-column
		if (isStartElementNamed(reader, Tablecolumn::kTag)) {
			columns_.append(Tablecolumn::builder(reader));
			return;
		}
		// <table:table-row> 9.1.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-row
		if (isStartElementNamed(reader, Tablerow::kTag)) {
			rows_.append(Tablerow::builder(reader));
			return;
		}
		// Deserialize present but unsupported subtags
		// <office:dde-source> 14.6.5 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_dde-source
		// <office:forms> 13.2 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-office_forms
		// <table:desc> 9.1.14 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_desc
		// <table:named-expressions> 9.4.11 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_named-expressions
		// <table:scenario> 9.2.7 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_scenario
		// <table:shapes> 9.2.8 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_shapes
		// <table:table-column-group> 9.1.10 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-column-group
		// <table:table-columns> 9.1.12 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-columns
		// <table:table-header-columns> 9.1.11 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-header-columns
		// <table:table-header-rows> 9.1.7 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-header-rows
		// <table:table-row-group> 9.1.9 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-row-group
		// <table:table-rows> 9.1.8 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-rows
		// <table:table-source> 9.2.6 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-source
		// <table:title> 9.1.13 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_title
		// <text:soft-page-break> 5.6 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_soft-page-break
		Tag::readSubtag(reader);
	}

	void Table::writeAttributes(QXmlStreamWriter* writer)
	{
		writeName(writer);
		writeStyle(writer);

		if (print_ranges_ != "") {
			writer->writeAttribute(kPrintRangeAttribute, print_ranges_);
		}
		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void Table::writeSubtags(QXmlStreamWriter* writer)
	{
		columns_.write(writer);
		rows_.write(writer);

		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
	}
}
