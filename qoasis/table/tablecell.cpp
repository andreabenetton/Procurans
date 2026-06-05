// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecell.h"
#include "tablecellboolean.h"
#include "tablecellcurrency.h"
#include "tablecelldate.h"
#include "tablecellfloat.h"
#include "tablecellpercentage.h"
#include "tablecellstring.h"
#include "tablecelltime.h"

namespace qoasis::table
{
	// Constants
	const QString Tablecell::kTag = QString("table:table-cell");
	const QString Tablecell::kCellTypeAttribute = QString("office:value-type");
	const QString Tablecell::kCellTypeValue = QString("");
	const QString Tablecell::kRepeatAttribute = QString("table:number-columns-repeated");
	const QString Tablecell::kTextPTag = QString("text:p");

	namespace
	{
		const QString kColumnSpanAttribute =
			QStringLiteral("table:number-columns-spanned");
		const QString kRowSpanAttribute =
			QStringLiteral("table:number-rows-spanned");
		const QString kFormulaAttribute = QStringLiteral("table:formula");
	}

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
		if (type == TablecellBoolean::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellBoolean(reader));
		}
		if (type == TablecellTime::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellTime(reader));
		}
		if (type == TablecellPercentage::kCellTypeValue) {
			return QSharedPointer<Tablecell>(new TablecellPercentage(reader));
		}
		// Genuinely unknown office:value-type (future ODF extensions or
		// vendor-specific values). Fall back to base Tablecell which
		// preserves the attribute generically and rebuilds the cell as an
		// opaque element on save.
		return QSharedPointer<Tablecell>(new Tablecell(reader));
	}

	QSharedPointer<Tablecell> Tablecell::placeholder(int repeat)
	{
		return QSharedPointer<Tablecell>(new Tablecell(repeat));
	}

	int Tablecell::getColumnSpan() const
	{
		return _columnSpan;
	}

	void Tablecell::setColumnSpan(int span)
	{
		_columnSpan = span > 0 ? span : 1;
	}

	int Tablecell::getRowSpan() const
	{
		return _rowSpan;
	}

	void Tablecell::setRowSpan(int span)
	{
		_rowSpan = span > 0 ? span : 1;
	}

	QString Tablecell::getFormula() const
	{
		return _formula;
	}

	void Tablecell::setFormula(const QString& formula)
	{
		_formula = formula;
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
		// table:number-columns-spanned 19.677 / table:number-rows-spanned
		// 19.683 — typed via getColumnSpan/getRowSpan so callers can
		// introspect merges programmatically without reaching into the
		// generic attribute map.
		if (name == kColumnSpanAttribute) {
			_columnSpan = value.toInt();
			if (_columnSpan < 1) _columnSpan = 1;
			return;
		}
		if (name == kRowSpanAttribute) {
			_rowSpan = value.toInt();
			if (_rowSpan < 1) _rowSpan = 1;
			return;
		}
		// table:formula 19.642 — captured typed so consumers can read/edit
		// the formula without reaching into the generic attribute map.
		// Preserved verbatim including the of:= / oooc:= prefix.
		if (name == kFormulaAttribute) {
			_formula = value.toString();
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
		// Emit span attributes only when non-default so unmerged cells stay
		// byte-similar to LibreOffice's output (LO writes attributes only
		// when >1).
		if (_columnSpan > 1) {
			writer->writeAttribute(kColumnSpanAttribute,
			                       QString::number(_columnSpan));
		}
		if (_rowSpan > 1) {
			writer->writeAttribute(kRowSpanAttribute,
			                       QString::number(_rowSpan));
		}
		if (!_formula.isEmpty()) {
			writer->writeAttribute(kFormulaAttribute, _formula);
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
