// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellboolean.h"

namespace qoasis::table
{
	const QString TablecellBoolean::kCellTypeValue = QString("boolean");
	const QString TablecellBoolean::kCellTypeAttribute =
		QString("office:boolean-value");

	TablecellBoolean::TablecellBoolean(bool value, int repeat, QString style)
		: Tablecell(repeat, style), value_bool_(value)
	{
	}

	TablecellBoolean::TablecellBoolean(bool value, QString style)
		: TablecellBoolean(value, 1, style)
	{
	}

	TablecellBoolean::TablecellBoolean(QXmlStreamReader& reader)
		: value_bool_(false)
	{
		Q_ASSERT(reader.qualifiedName() == TablecellBoolean::kTag);
		Tag::read(reader);
	}

	bool TablecellBoolean::getBool() const
	{
		return value_bool_;
	}

	bool TablecellBoolean::isEmpty()
	{
		return false;
	}

	// implements Tablecell
	QString TablecellBoolean::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellBoolean::readAttribute(QStringView name, QStringView value)
	{
		if (name == kCellTypeAttribute) {
			// ODF Part 1 §19.378: xsd:boolean ("true"/"false" or "1"/"0").
			const QString v = value.toString();
			value_bool_ = (v == QStringLiteral("true") || v == QStringLiteral("1"));
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellBoolean::writeAttributes(QXmlStreamWriter* writer)
	{
		writer->writeAttribute(
			kCellTypeAttribute,
			value_bool_ ? QStringLiteral("true") : QStringLiteral("false"));
		Tablecell::writeAttributes(writer);
	}

	void TablecellBoolean::writeSubtags(QXmlStreamWriter* writer)
	{
		// Cells loaded from XML preserve their input <text:p> via the base
		// class' children mechanism. For programmatic construction, emit a
		// neutral lowercase boolean literal — the spec doesn't mandate a
		// display projection, and LibreOffice will reformat per locale on
		// next open.
		if (!_paragraphs.isEmpty() || !_valueText.isEmpty())
		{
			Tablecell::writeSubtags(writer);
			return;
		}
		writer->writeTextElement(
			Tablecell::kTextPTag,
			value_bool_ ? QStringLiteral("true") : QStringLiteral("false"));
	}
}
