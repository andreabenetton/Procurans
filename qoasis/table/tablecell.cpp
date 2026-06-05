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
		// Unmodeled office:value-type — boolean, time, percentage, or any
		// future ODF/LO extension. Falling back to the base Tablecell keeps
		// the cell round-tripping as an opaque element: its attributes
		// (office:value-type + companion office:boolean-value /
		// office:time-value / office:value etc.) are preserved verbatim via
		// Tag::readAttribute, and its <text:p> display projection survives
		// too. The previous Q_ASSERT(false) here segfaulted release builds.
		return QSharedPointer<Tablecell>(new Tablecell(reader));
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

	void Tablecell::readAttribute(QStringView name, QStringView value)
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
			// Captured for the base-Tablecell fallback path (unmodeled
			// value-types like boolean / time / percentage). Typed subclasses
			// override instanceCellType() and the captured value is ignored
			// on write — see writeAttributes below.
			_valueType = value.toString();
			return;
		}
		// Deserialize present but unsupported attributes (calcext:* etc.)
		Tag::readAttribute(name, value);
	}

	void Tablecell::readSubtag(QXmlStreamReader& reader)
	{
		// <text:p> 5.1.3 http://docs.oasis-open.org/office/v1.2/os/OpenDocument-v1.2-os-part1.html#element-text_p
		// Build the <text:p> as a generic Tag so inline rich-text markup
		// (text:span, text:a, etc.) survives round-trip. The first paragraph's
		// flattened plain text still seeds _valueText so getText(), isEmpty()
		// and the typed subclasses (TablecellString in particular) keep
		// behaving like before.
		if (reader.qualifiedName() == kTextPTag) {
			QSharedPointer<Tag> para = Tag::buildGeneric(reader);
			if (_valueText.isEmpty()) {
				_valueText = para->plainText();
			}
			_paragraphs.append(para);
			return;
		}
		// Other cell children (e.g. <office:annotation>) preserved generically.
		Tag::readSubtag(reader);
	}

	void Tablecell::writeAttributes(QXmlStreamWriter* writer)
	{
		writeRepeat(writer);
		writeStyle(writer);
		QString celltype = instanceCellType();
		// Base Tablecell's instanceCellType() is empty; fall back to the
		// captured input value-type so unmodeled cell types (boolean / time /
		// percentage / future extensions) still round-trip.
		if (celltype.isEmpty()) {
			celltype = _valueType;
		}
		if (!celltype.isEmpty()) {
			writer->writeAttribute(Tablecell::kCellTypeAttribute, celltype);
		}
		// Serialize present but unsupported attributes
		Tag::writeAttributes(writer);
	}

	void Tablecell::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!_paragraphs.isEmpty()) {
			// Rich-text paragraphs read from the input survive verbatim,
			// preserving any text:span / text:a / etc. children.
			for (const QSharedPointer<Tag>& para : _paragraphs) {
				if (!para.isNull()) {
					para->write(writer);
				}
			}
		}
		else if (!(_valueText.isNull() || _valueText.isEmpty())) {
			// Programmatically-constructed cell with only plain display text.
			writer->writeTextElement(Tablecell::kTextPTag, _valueText);
		}
		// Other cell children preserved via base class.
		Tag::writeSubtags(writer);
	}
}
