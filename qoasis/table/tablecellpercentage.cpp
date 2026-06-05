// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "tablecellpercentage.h"

namespace qoasis::table
{
	const QString TablecellPercentage::kCellTypeValue = QString("percentage");
	const QString TablecellPercentage::kCellTypeAttribute = QString("office:value");

	TablecellPercentage::TablecellPercentage(double fraction, int repeat, QString style)
		: Tablecell(repeat, style), value_fraction_(fraction)
	{
	}

	TablecellPercentage::TablecellPercentage(double fraction, QString style)
		: TablecellPercentage(fraction, 1, style)
	{
	}

	TablecellPercentage::TablecellPercentage(QXmlStreamReader& reader)
		: value_fraction_(0.0)
	{
		Q_ASSERT(reader.qualifiedName() == TablecellPercentage::kTag);
		Tag::read(reader);
	}

	double TablecellPercentage::getPercentage() const
	{
		return value_fraction_;
	}

	bool TablecellPercentage::isEmpty()
	{
		return false;
	}

	// implements Tablecell
	QString TablecellPercentage::instanceCellType()
	{
		return kCellTypeValue;
	}

	// implements Tag
	void TablecellPercentage::readAttribute(QStringView name, QStringView value)
	{
		if (name == kCellTypeAttribute) {
			value_fraction_ = value.toDouble();
			return;
		}
		Tablecell::readAttribute(name, value);
	}

	void TablecellPercentage::writeAttributes(QXmlStreamWriter* writer)
	{
		writer->writeAttribute(
			kCellTypeAttribute,
			QString::number(value_fraction_, 'g', 15));
		Tablecell::writeAttributes(writer);
	}

	void TablecellPercentage::writeSubtags(QXmlStreamWriter* writer)
	{
		if (!_paragraphs.isEmpty() || !_valueText.isEmpty())
		{
			Tablecell::writeSubtags(writer);
			return;
		}
		// Programmatic synthesis: render the fraction as a percentage with
		// Italian-locale decimal comma, matching the float cell's idiom.
		const QString display =
			QString::number(value_fraction_ * 100.0, 'g', 15)
				.replace(QLatin1Char('.'), QLatin1Char(','))
			+ QLatin1Char('%');
		writer->writeTextElement(Tablecell::kTextPTag, display);
	}
}
