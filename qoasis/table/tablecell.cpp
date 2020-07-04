// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecell.h"
#include "tablecellstring.h"
#include "tablecelldate.h"
#include "tablecellfloat.h"
#include "tablecellcurrency.h"

namespace qoasis::table
{
	// Constants
	const QString Tablecell::kTag = QString("table:table-cell");
	const QString Tablecell::kCellTypeAttribute = QString("office:value-type");
	const QString Tablecell::kCellTypeValue = QString("");
	const QString Tablecell::kRepeatAttribute = QString("table:number-columns-repeated");
	const QString Tablecell::kTextPTag = QString("text:p");
	const QString Tablecell::kCalcextValueType = QString("calcext:value-type");

	// Constructors
	Tablecell::Tablecell(int repeat, QString style) : IStyleable(style), IRepeatable(repeat)
	{
		_valueText = "";
	}

	Tablecell::Tablecell(QString style) : IStyleable(style), IRepeatable(1)
	{
		_valueText = "";
	}

	Tablecell::Tablecell(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablecell::kTag);
		Tag::read(reader);
	}

	// Static methods
	QSharedPointer<Tablecell> Tablecell::clone(QSharedPointer<Tablecell> obj)
	{
		Q_ASSERT(obj->instanceTag() == Tablecell::kTag);

		if (obj->instanceCellType() == TablecellString::kCellTypeValue) {
			const QSharedPointer<TablecellString> cell = qSharedPointerCast<TablecellString>(obj);
			return QSharedPointer<Tablecell>(new TablecellString(*cell));
		}
		if (obj->instanceCellType() == TablecellDate::kCellTypeValue) {
			const QSharedPointer<TablecellDate> cell = qSharedPointerCast<TablecellDate>(obj);
			return QSharedPointer<Tablecell>(new TablecellDate(*cell));
		}
		if (obj->instanceCellType() == TablecellFloat::kCellTypeValue) {
			const QSharedPointer<TablecellFloat> cell = qSharedPointerCast<TablecellFloat>(obj);
			return QSharedPointer<Tablecell>(new TablecellFloat(*cell));
		}
		if (obj->instanceCellType() == TablecellCurrency::kCellTypeValue) {
			const QSharedPointer<TablecellCurrency> cell = qSharedPointerCast<TablecellCurrency>(obj);
			return QSharedPointer<Tablecell>(new TablecellCurrency(*cell));
		}
		const QSharedPointer<Tablecell> cell = qSharedPointerCast<Tablecell>(obj);
		return QSharedPointer<Tablecell>(new Tablecell(*cell));
	}

	QSharedPointer<Tablecell> Tablecell::builder(QXmlStreamReader& reader)
	{
		Q_ASSERT(reader.qualifiedName() == Tablecell::kTag);

		if (!reader.attributes().hasAttribute(Tablecell::kCellTypeAttribute)) {
			return QSharedPointer<Tablecell>(new Tablecell(reader));
		}

		const QString type = reader.attributes().value(Tablecell::kCellTypeAttribute).toString();
		if (type.isEmpty()) {
			return QSharedPointer<Tablecell>(new Tablecell(reader));
		}
		if (type == TablecellString::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellString(reader));
		}
		if (type == TablecellDate::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellDate(reader));
		}
		if (type == TablecellFloat::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellFloat(reader));
		}
		if (type == TablecellCurrency::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellCurrency(reader));
		}
		Q_ASSERT(false);
		return {};
	}

	QSharedPointer<Tablecell> Tablecell::placeholder(int repeat)
	{
		return QSharedPointer<Tablecell>(new Tablecell(repeat));
	}

	// Methods
	QString Tablecell::getText() const
	{
		return _valueText;
	}

	QString Tablecell::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements IRepeatable
	QString Tablecell::repeatTag()
	{
		return kRepeatAttribute;
	}

	bool Tablecell::isEmpty()
	{	
		return (_valueText.isNull() || _valueText.isEmpty());
	}

	// implements Tag
	QString Tablecell::instanceTag()
	{
		return kTag;
	}

	void Tablecell::readAttribute(QStringRef name, QStringRef value)
	{
		// table:style-name 19.726.13 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_style-name_element-table_table-cell
		if (name.toString() == styleTag()) {
			readStyle(value);
			return;
		}
		// table:number-columns-repeated 19.675.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#attribute-table_number-columns-repeated_element-table_table-cell
		if (name.toString() == repeatTag()) {
			readRepeat(value);
			return;
		}

		if (name == Tablecell::kCellTypeAttribute) {
			return;
		}
		if (name == Tablecell::kCalcextValueType) {
			return;
		}
		// Deserialize present but unsupported attributes
		Tag::readAttribute(name, value);
	}

	void Tablecell::readSubtag(QXmlStreamReader& reader)
	{
		// <text:p> 5.1.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_p
		// only one p tag is supported, the last one if more than one present
		if (reader.qualifiedName() == kTextPTag) {
			_valueText = reader.readElementText(QXmlStreamReader::IncludeChildElements);
			return;
		}
		// Deserialize present but unsupported subtags
		//Tag::readSubtag(reader);
	}

	void Tablecell::writeAttributes(QXmlStreamWriter* writer)
	{
		writeRepeat(writer);
		writeStyle(writer);
		QString celltype = instanceCellType();
		if (celltype != "") {
			writer->writeAttribute(Tablecell::kCellTypeAttribute, celltype);
			writer->writeAttribute(Tablecell::kCalcextValueType, celltype);
		}
		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void Tablecell::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!(_valueText.isNull() || _valueText.isEmpty())) {
			writer->writeTextElement(Tablecell::kTextPTag, _valueText);
		}
		// Serialize present but unsupported subtags
		//Tag::writeSubtags(writer);
	}
}
