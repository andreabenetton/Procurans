// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecolumn.h"

namespace qoasis::table
{
	// Constants
	const QString Tablecolumn::kTag = QString("table:table-column");
	const QString Tablecolumn::kRepeatAttribute = QString("table:number-columns-repeated");
	const QString Tablecolumn::kDefaultCellStyleAttribute = QString("table:default-cell-style-name");

	// Constructors
	Tablecolumn::Tablecolumn(int repeat, QString style, QString default_cell_style) : Tag(), IStyleable(style),
	                                                                                  IRepeatable(repeat)
	{
		default_cell_style_ = default_cell_style;
	}

	Tablecolumn::Tablecolumn(QXmlStreamReader& reader) : Tag(), IStyleable(""), IRepeatable(1)
	{
		Q_ASSERT(reader.qualifiedName() == Tablecolumn::kTag);
		Tag::read(reader);
	}

	// Static methods
	QSharedPointer<Tablecolumn> Tablecolumn::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablecolumn::kTag);
		return QSharedPointer<Tablecolumn>(new Tablecolumn(reader));
	}

	QSharedPointer<Tablecolumn> Tablecolumn::placeholder(int repeat)
	{
		return QSharedPointer<Tablecolumn>(new Tablecolumn(repeat));
	}

	// Methods
	QString Tablecolumn::getDefaultCellStyle() const
	{
		return default_cell_style_;
	}

	void Tablecolumn::setDefaultCellStyle(QString style)
	{
		default_cell_style_ = style;
	}

	// implements IRepeatable
	QString Tablecolumn::repeatTag()
	{
		return kRepeatAttribute;
	}

	bool Tablecolumn::isEmpty()
	{
		return true;
	}

	// implements Tag
	QString Tablecolumn::instanceTag()
	{
		return kTag;
	}

	void Tablecolumn::readAttribute(QStringRef name, QStringRef value)
	{
		// table:number-columns-repeated 19.675.4 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_number-columns-repeated_element-table_table-column
		if (name == repeatTag()) {
			readRepeat(value);
			return;
		}
		// table:style-name 19.726.14 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_style-name_element-table_table-column
		if (name == styleTag()) {
			readStyle(value);
			return;
		}
		// table:default-cell-style-name 19.615 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_default-cell-style-name
		if (name == kDefaultCellStyleAttribute) {
			default_cell_style_ = value.toString();
			return;
		}
		// Deserialize present but unsupported attributes
		// table:visibility 19.749 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_visibility
		// xml:id 19.914 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-xml_id
		Tag::readAttribute(name, value);
	}

	void Tablecolumn::writeAttributes(QXmlStreamWriter* writer)
	{
		writeRepeat(writer);
		writeStyle(writer);
		if (default_cell_style_ != "") {
			writer->writeAttribute(Tablecolumn::kDefaultCellStyleAttribute, default_cell_style_);
		}
		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}
}
