// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablerow.h"
#include "tablecell.h"

namespace qoasis::table
{
	// Constants
	const QString Tablerow::kTag = QString("table:table-row");
	const QString Tablerow::kRepeatAttribute = QString("table:number-rows-repeated");

	// Constructors
	Tablerow::Tablerow(int repeat, QString style, int array_size,
	                   QSharedPointer<Tablecell> cells[]) : Tag(), IStyleable(style), IRepeatable(repeat)
	{
		Q_ASSERT(array_size >= 0);
		cells_.append(array_size, cells);
	}

	Tablerow::Tablerow(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablerow::kTag);
		Tag::read(reader);
	}

	// Static methods
	QSharedPointer<Tablerow> Tablerow::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablerow::kTag);
		return QSharedPointer<Tablerow>(new Tablerow(reader));
	}

	QSharedPointer<Tablerow> Tablerow::placeholder(int repeat)
	{
		return QSharedPointer<Tablerow>(new Tablerow(repeat));
	}

	// Methods
	QSharedPointer<Tablecell> Tablerow::getCell(int index) const
	{
		Q_ASSERT(index >= 0);
		return cells_.at(index);
	}

	void Tablerow::removeCell(int index)
	{
		Q_ASSERT(index >= 0);
		cells_.remove(index);
	}

	void Tablerow::insertCell(int index, const QSharedPointer<Tablecell> cell)
	{
		cells_.insert(index, cell);
	}

	void Tablerow::appendCell(const QSharedPointer<Tablecell> cell)
	{
		cells_.append(cell);
	}

	void Tablerow::replaceCell(int index, const QSharedPointer<Tablecell> cell)
	{
		//if (cells_.at(index).isNull()) {
		//	const auto previous_base_index = scanBackwardForBaseOfRepeatedCells(index);
		//	const int last_of_previous_base = previous_base_index + cells_.at(previous_base_index)->getRepeat();
		//	const int index_offset = index - last_of_previous_base;
		//	
		//	// the replacement is in the repetition						
		//	if (index_offset <= 0) {
		//		// modify the repetition of the base cell
		//		cells_.at(previous_base_index)->offsetRepeatBy(index - previous_base_index);
		//		// if not the last of the repetition
		//		if (index_offset < 0) {
		//			// clone the original cell and put it on the right of the replacement
		//			QSharedPointer<Tablecell> clone_cell = cells_.at(previous_base_index);
		//			clone_cell->setRepeat(-index_offset);
		//			cells_.replace(index + 1, clone_cell);
		//		}
		//	} else {
		//		// the cell is after of a repeated cell
		//		// create an empty cell to fill the space between the last repetition of base cell and the replacement
		//		const auto empty_cell = QSharedPointer<Tablecell>(new TablecellEmpty(index_offset));
		//		cells_.replace(last_of_previous_base + 1, empty_cell);		
		//	}
		//	cells_.replace(index, cell);	
		//} else {
		//	if(cells_.at(index)->getRepeat() > 1) {
		//		cells_.at(index)->offsetRepeatBy(-1);
		//		cells_.insert(index, cell);
		//	} else {
		//		cells_.replace(index, cell);
		//	}
		//}		
	}

	void Tablerow::replaceCells(int start, int end, QSharedPointer<Tablecell> cells[])
	{
		Q_ASSERT(start >= 0);
		Q_ASSERT(end >= start);
		for (int i = start; i <= end; ++i) {
			replaceCell(i, cells[i]);
		}
	}

	void Tablerow::restyleFromRow(QSharedPointer<Tablerow> fromRow)
	{
		setStyle(fromRow->getStyle());
		int itemsFromRow = fromRow->getLastNonEmptyCell();
		int itemsToRow = getLastNonEmptyCell();
		
		for (int j = 0; j < (itemsFromRow<itemsToRow? itemsFromRow : itemsToRow) + 1; j++) {
			if (!fromRow->getCell(j).isNull()) {
				if (!getCell(j).isNull()) {
					getCell(j)->setStyle(fromRow->getCell(j)->getStyle());
				}
			}
		}
	}

	int Tablerow::getLastNonEmptyCell() const
	{
		return cells_.lastNotEmpty();
	}

	// implements IRepeatable
	QString Tablerow::repeatTag()
	{
		return kRepeatAttribute;
	}

	bool Tablerow::isEmpty()
	{
		// A row is empty if all its cells are empty
		return cells_.lastNotEmpty() < 0;
	}

	// implements Tag
	QString Tablerow::instanceTag()
	{
		return kTag;
	}

	void Tablerow::readAttribute(QStringRef name, QStringRef value)
	{
		// table:number-rows-repeated 19.677 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_number-rows-repeated
		if (name.toString() == repeatTag()) {
			readRepeat(value);
			return;
		}
		// table:style-name 19.726.15 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_style-name_element-table_table-row
		if (name.toString() == styleTag()) {
			readStyle(value);
			return;
		}
		// Deserialize present but unsupported attributes
		// table:default-cell-style-name 19.615 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_default-cell-style-name
		// table:visibility 19.749 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_visibility
		// xml:id 19.914 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-xml_id
		Tag::readAttribute(name, value);
	}

	void Tablerow::readSubtag(QXmlStreamReader& reader)
	{
		// <table:table-cell> 9.1.4 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_table-cell
		if (isStartElementNamed(reader, Tablecell::kTag)) {
			cells_.append(Tablecell::builder(reader));
			return;
		}
		// Deserialize present but unsupported subtags
		// <table:covered-table-cell> 9.1.5 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-table_covered-table-cell 
		Tag::readSubtag(reader);
	}

	void Tablerow::writeAttributes(QXmlStreamWriter* writer)
	{
		writeRepeat(writer);
		writeStyle(writer);

		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void Tablerow::writeSubtags(QXmlStreamWriter* writer)
	{
		cells_.write(writer);
		// Serialize present but unsupported subtags
		Tag::writeSubtags(writer);
	}
}
